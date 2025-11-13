#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/module.h>

#include <linux/seq_file.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#include <media/tegracam_core.h>

#include "fpga_max96724.h"

#include "fpga_max9295_mode_tbls.h"

#define MAX9295_ALTER_ADDR_BASE 0x20

struct fpga_max9295 {
	struct i2c_client	*i2c_client;
	const struct i2c_device_id *id;
	struct v4l2_subdev	*subdev;
	struct device		*dser_dev;
	struct camera_common_data	*s_data;
	struct tegracam_device		*tc_dev;
	struct gmsl_link_ctx    g_ctx;
	u32 def_addr;
	u32 act_addr;
	u32 des_link;
};

static const struct regmap_config sensor_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
	.use_single_rw = true,
#else
	.use_single_read = true,
	.use_single_write = true,
#endif
};

static struct mutex serdes_lock__;

const struct of_device_id fpga_max9295_of_match[] = {
	{ .compatible = "Connor,fpga_max9295",},
	{ },
};
MODULE_DEVICE_TABLE(of, fpga_max9295_of_match);

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
};

static int fpga_max9295_read_reg(struct camera_common_data *s_data,
				u16 addr, u8 *val)
{
	int err = 0;
	u32 reg_val = 0;
	struct device *dev = s_data->dev;

	err = regmap_read(s_data->regmap, addr, &reg_val);
	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err){
		*val = reg_val & 0xFF;
		dev_info(dev,"fpga:fpga_max9295:r addr(0x%04x)<-0x%02x\n",addr,*val);
	}
	if (err)
		dev_err(dev, "fpga:fpga_max9295: i2c read failed, addr=0x%x\n", addr);

	return err;
}

static int fpga_max9295_write_reg(struct camera_common_data *s_data,
				u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	struct i2c_client *client = to_i2c_client(s_data->dev);
	dev_info(dev, "I2C addr: 0x%02x\n", client->addr);


	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "fpga:%s:fpga_max9295:i2c write failed, 0x%x = %x\n",
			__func__, addr, val);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"fpga:fpga_max9295:w addr(0x%04x)->0x%02x\n",addr,val);
	return err;
}

static int iic_write(struct i2c_client* client, u8 slaveaddr, u16 regaddr, u8 data)
{
	struct i2c_msg msg[2];
    u8 buf[3];
	int ret;

	msg[0].addr = slaveaddr;
    msg[0].flags = 0;
    msg[0].buf = buf;
    msg[0].len = sizeof(buf);
 
    buf[0] = regaddr>>8;
    buf[1] = regaddr&0xff;
	buf[2] = data;
    ret = i2c_transfer(client->adapter, msg, 1);
	usleep_range(100, 110);
	if(ret==1){ //send ok
		dev_info(&client->dev,"fpga:fpga_max9295:w slave=0x%02x addr(0x%04x)->0x%02x\n",slaveaddr,regaddr,data);
		return 0;
	}else{
		dev_err(&client->dev, "fpga:%s:fpga_max9295:i2c write failed, slave=0x%02x addr(0x%04x)->0x%02x\n",
			__func__, slaveaddr, regaddr, data);
		return -1;
	}
}

static int iic_read(struct i2c_client* client, u8 slaveaddr, u16 regaddr, u8 *data)
{
	struct i2c_msg msg[2];
    u8 buf[3];
	int ret;

	msg[0].addr = slaveaddr;
	msg[0].flags = 0;
	msg[0].buf = buf;
	msg[0].len = 2;

	buf[0] = regaddr>>8;
    buf[1] = regaddr&0xff;

	msg[1].addr = slaveaddr;
	msg[1].buf = buf;
	msg[1].len = 1;
	msg[1].flags = I2C_M_RD;

	ret = i2c_transfer(client->adapter, msg, 2);
	if(ret==2){
		*data = buf[0];
		dev_info(&client->dev,"fpga:fpga_max9295:r slave=0x%02x addr(0x%04x)<-0x%02x\n",slaveaddr, regaddr,*data);
		return 0;
	}else{
		dev_err(&client->dev, "fpga: %s fpga_max9295: i2c read failed, slave=0x%02x, addr=0x%04x\n", __func__, slaveaddr, regaddr);
		return -1;
	}
}

static int fpga_max9295_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;

	dev_info(dev, "fpga: %s\n", __func__);
	if (pdata && pdata->power_on) {
		err = pdata->power_on(pw);
		if (err)
			dev_err(dev, "%s failed.\n", __func__);
		else
			pw->state = SWITCH_ON;
		return err;
	}

	pw->state = SWITCH_ON;

	return 0;
}

static int fpga_max9295_power_off(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;

	dev_info(dev, "fpga: %s\n", __func__);

	if (pdata && pdata->power_off) {
		err = pdata->power_off(pw);
		if (!err)
			goto power_off_done;
		else
			dev_err(dev, "%s failed.\n", __func__);
		return err;
	}

power_off_done:
	pw->state = SWITCH_OFF;

	return 0;
}

static int fpga_max9295_power_get(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;

	pw->state = SWITCH_OFF;

	return 0;
}

static int fpga_max9295_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;

	if (unlikely(!pw))
		return -EFAULT;

	return 0;
}

static int fpga_max9295_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	dev_info(tc_dev->dev,"fpga: %s\n",__func__);
	return 0;
}

static struct tegracam_ctrl_ops fpga_max9295_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_group_hold = fpga_max9295_set_group_hold,
};

static struct camera_common_pdata *fpga_max9295_parse_dt(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct device_node *node = dev->of_node;
	struct camera_common_pdata *board_priv_pdata;
	const struct of_device_id *match;

	if (!node)
		return NULL;

	match = of_match_device(fpga_max9295_of_match, dev);
	if (!match) {
		dev_err(dev, "Failed to find matching dt id\n");
		return NULL;
	}

	board_priv_pdata = devm_kzalloc(dev, sizeof(*board_priv_pdata), GFP_KERNEL);

	return board_priv_pdata;
}

static int fpga_max9295_set_mode(struct tegracam_device *tc_dev)
{
	dev_info(tc_dev->dev,"fpga:fpga_max9295 set mode\n");
	return 0;
}

static int fpga_max9295_start_streaming(struct tegracam_device *tc_dev)
{
	struct fpga_max9295 *priv = (struct fpga_max9295 *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	mutex_lock(&serdes_lock__);
	dev_info(dev,"fpga: fpga_max9295 start streaming - enter\n");

	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0330, 0x00);	//设置串化器为1x4 模式。
	//iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0332, 0xee);	//映射
	//iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0333, 0xe4);
	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0331, 0x33);	//lanes
	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0311, 0x40);	//start port b
	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0308, 0x64);	//select port b

	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0318, 0x5e);
	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0319, 0x5e);
	iic_write(priv->i2c_client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x0002, 0x43);

	err = fpga_max96724_start_streaming(priv->dser_dev, &priv->g_ctx);
	if (err)
		goto exit;

	dev_info(dev,"fpga: fpga_max9295 start streaming - exit\n");
	mutex_unlock(&serdes_lock__);
	return 0;

exit:
	mutex_unlock(&serdes_lock__);
	if(err){
		dev_err(dev, "fpga: %s: error setting stream\n", __func__);
	}
	dev_info(dev,"fpga: fpga_max9295 start streaming - exit\n");
	return err;
}

static int fpga_max9295_stop_streaming(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct fpga_max9295 *priv = (struct fpga_max9295 *)tegracam_get_privdata(tc_dev);

	dev_info(dev,"fpga:fpga_max9295 stop streaming\n");
	/* disable serdes streaming */
	fpga_max96724_stop_streaming(priv->dser_dev, &priv->g_ctx);

	return 0;
}

static int fpga_max9295_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s:\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops fpga_max9295_subdev_internal_ops = {
	.open = fpga_max9295_open,
};

static struct camera_common_sensor_ops fpga_max9295_common_ops = {
	.numfrmfmts = ARRAY_SIZE(fpga_max9295_frmfmt),
	.frmfmt_table = fpga_max9295_frmfmt,
	.power_on = fpga_max9295_power_on,
	.power_off = fpga_max9295_power_off,
	.write_reg = fpga_max9295_write_reg,
	.read_reg = fpga_max9295_read_reg,
	.parse_dt = fpga_max9295_parse_dt,
	.power_get = fpga_max9295_power_get,
	.power_put = fpga_max9295_power_put,
	.set_mode = fpga_max9295_set_mode,
	.start_streaming = fpga_max9295_start_streaming,
	.stop_streaming = fpga_max9295_stop_streaming,
};

static int fpga_max9295_board_setup(struct fpga_max9295 *priv)
{
	struct device *dev = &priv->i2c_client->dev;
	struct device_node *node = dev->of_node;
	struct device_node *dser_node;
	struct i2c_client *dser_i2c = NULL;
	struct device_node *gmsl;
	int value = 0xFFFF;
	const char *str_value1[2],*str_value;
	int  i;
	int err;

	err = of_property_read_u32(node, "reg", &priv->act_addr);
	if (err < 0) {
		dev_err(dev, "fpga: reg not found\n");
		goto error;
	}

	err = of_property_read_u32(node, "def-addr",
					&priv->def_addr);
	if (err < 0) {
		dev_err(dev, "fpga: def-addr not found\n");
		goto error;
	}

	priv->g_ctx.ser_dev = dev;

	dser_node = of_parse_phandle(node, "nvidia,gmsl-dser-device", 0);
	if (dser_node == NULL) {
		dev_err(dev,
			"fpga: missing %s handle\n",	"nvidia,gmsl-dser-device");
		err = -EINVAL;
		goto error;
	}

	dser_i2c = of_find_i2c_device_by_node(dser_node);
	of_node_put(dser_node);

	if (dser_i2c == NULL) {
		dev_err(dev, "fpga: missing deserializer dev handle\n");
		err = -EINVAL;
		goto error;
	}
	if (dser_i2c->dev.driver == NULL) {
		dev_err(dev, "fpga: missing deserializer driver\n");
		err = -EINVAL;
		goto error;
	}

	priv->dser_dev = &dser_i2c->dev;
	priv->g_ctx.des_dev =  &dser_i2c->dev;

	err = of_property_read_string(node, "des-link", &str_value);
    if (err < 0) {
        dev_err(dev, "fpga: des-link property is not found\n");
        return -EINVAL;
    }
    dev_info(dev, "fpga: link is %s\n",str_value);
 
    priv->des_link = str_value[0]-'A';
	priv->g_ctx.des_link = priv->des_link;

	/* populate g_ctx from DT */
	gmsl = of_get_child_by_name(node, "gmsl-link");
	if (gmsl == NULL) {
		dev_err(dev, "fpga: missing gmsl-link device node\n");
		err = -EINVAL;
		goto error;
	}

	err = of_property_read_u32(gmsl, "num-ser-lanes", &value);
	if (err < 0) {
		dev_err(dev, "fpga: No num-lanes info\n");
		goto error;
	}
	priv->g_ctx.num_ser_csi_lanes = value;

	priv->g_ctx.num_streams =
			of_property_count_strings(gmsl, "streams");
	if (priv->g_ctx.num_streams <= 0) {
		dev_err(dev, "fpga: No streams found\n");
		err = -EINVAL;
		goto error;
	}

	for (i = 0; i < priv->g_ctx.num_streams; i++) {
		of_property_read_string_index(gmsl, "streams", i,
						&str_value1[i]);
		if (!str_value1[i]) {
			dev_err(dev, "fpga: invalid stream info\n");
			goto error;
		}
		if (!strcmp(str_value1[i], "raw12")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_RAW_12;
		} else if (!strcmp(str_value1[i], "rgb888")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_RGB_8;
		} else if (!strcmp(str_value1[i], "ued-u1")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_UED_U1;
		} else if (!strcmp(str_value1[i], "yuv16")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_YUV16;
		}  else {
			dev_err(dev, "fpga: invalid stream data type\n");
			goto error;
		}
	}

	priv->g_ctx.sensor_dev = dev;

	return 0;

error:
	dev_err(dev, "fpga: board setup failed\n");
	if(err==0) err=-EINVAL;
	return err;
}

static int fpga_max9295_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct device *dev = &client->dev;
	struct device_node *node = dev->of_node;
	struct tegracam_device *tc_dev;
	struct fpga_max9295 *priv;
	int err;
	u8 val;

    dev_info(dev, "fpga: probing v4l2 sensor @0x%x.\n",client->addr);

    if (!IS_ENABLED(CONFIG_OF) || !node)
		return -EINVAL;

    priv = devm_kzalloc(dev, sizeof(struct fpga_max9295), GFP_KERNEL);
	if (!priv) {
		dev_err(dev, "fpga: unable to allocate memory!\n");
		return -ENOMEM;
	}

	tc_dev = devm_kzalloc(dev, sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "fpga_max9295", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &fpga_max9295_common_ops;
	tc_dev->v4l2sd_internal_ops = &fpga_max9295_subdev_internal_ops;
	tc_dev->tcctrl_ops = &fpga_max9295_ctrl_ops;

	err = fpga_max9295_board_setup(priv);
	if (err) {
		dev_err(dev, "fpga: board setup failed\n");
		return err;
	}

	fpga_max96724_lock_link(priv->dser_dev);
	if ( fpga_max96724_check_link_status(priv->dser_dev, priv->des_link) ){ //link occupied
		fpga_max96724_unlock_link(priv->dser_dev);
		dev_err(dev, "fpga: link_%c is occupied\n",'A'+priv->des_link);
		return -EINVAL;
	}

	err = tegracam_device_register(tc_dev);
	if (err) {
		fpga_max96724_unlock_link(priv->dser_dev);
		dev_err(dev, "fpga: tegra camera driver registration failed\n");
		return err;
	}

	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	fpga_max96724_monopolize_link(priv->dser_dev, priv->des_link);
	iic_write(client,0x40, 0x0000, (MAX9295_ALTER_ADDR_BASE+priv->des_link)<<1);

	// mdelay(100);
	// iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x02bf, 0xa0);
	// iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x02be, 0x18); //power on
	// iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x03f1, 0x89);

	// iic_write(client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x02d6, 0x00);
	// iic_write(client, MAX9295_ALTER_ADDR_BASE + priv->des_link, 0x02d7, 0x60);

	err = iic_read(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x000d, &val);
	if (err || val != 0x91) {
		dev_err(dev, "fpga access fpga_max9295's 9295 failed\n");
		fpga_max96724_restore_link(priv->dser_dev);
		tegracam_device_unregister(tc_dev);
		fpga_max96724_unlock_link(priv->dser_dev);
		return -EINVAL;
	}

	// err = iic_read(client, priv->def_addr, 0x6005, &val);
	// if(err){
	// 	dev_err(dev, "fpga access fpga_max9295's isp failed\n");
	// 	fpga_max96724_restore_link(priv->dser_dev);
	// 	tegracam_device_unregister(tc_dev);
	// 	fpga_max96724_unlock_link(priv->dser_dev);
	// 	return -EINVAL;
	// }

	iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x0044, priv->act_addr<<1); //dst addr
	iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link, 0x0045, priv->def_addr<<1); //src addr, original sensor addr

    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x007b, 0x30+priv->des_link);
    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x0083, 0x30+priv->des_link);
	iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x008b, 0x30+priv->des_link);
    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x0093, 0x30+priv->des_link);
    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x009b, 0x30+priv->des_link);
    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x00a3, 0x30+priv->des_link);
    iic_write(client, MAX9295_ALTER_ADDR_BASE+priv->des_link,0x00ab, 0x30+priv->des_link);

	fpga_max96724_enable_link(priv->dser_dev, priv->des_link);
	fpga_max96724_restore_link(priv->dser_dev);
	fpga_max96724_unlock_link(priv->dser_dev);

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "fpga: tegra camera subdev registration failed\n");
		return err;
	}

	dev_info(&client->dev, "fpga:Detected fpga_max9295 sensor\n");

	return 0;
}

static int fpga_max9295_remove(struct i2c_client *client)
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct fpga_max9295 *priv = (struct fpga_max9295 *)s_data->priv;

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	dev_info(&client->dev,"fpga, fpga_max9295_remove sensor @0x%x.\n",client->addr);
	return 0;
}

static const struct i2c_device_id fpga_max9295_id[] = {
	{ "fpga_max9295", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, fpga_max9295_id);

static struct i2c_driver fpga_max9295_i2c_driver = {
	.driver = {
		.name = "fpga_max9295",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(fpga_max9295_of_match),
	},
	.probe = fpga_max9295_probe,
	.remove = fpga_max9295_remove,
	.id_table = fpga_max9295_id,
};

static int __init fpga_max9295_init(void)
{
	mutex_init(&serdes_lock__);

	return i2c_add_driver(&fpga_max9295_i2c_driver);
}

static void __exit fpga_max9295_exit(void)
{
	mutex_destroy(&serdes_lock__);

	i2c_del_driver(&fpga_max9295_i2c_driver);
}

late_initcall(fpga_max9295_init);
module_exit(fpga_max9295_exit);

MODULE_DESCRIPTION("GMSL Deserializer driver fpga_max9295");
MODULE_AUTHOR("Connor connor.zhou@foxmail.com");
MODULE_LICENSE("GPL v2");