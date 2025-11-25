#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/module.h>

#include <linux/seq_file.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>

#include <media/tegracam_core.h>

#define USE_MAX96724_DESER 1
//#define USE_MAX96712_DESER 1

#if defined(USE_MAX96712_DESER)
#include "abh_max96712.h"  // 包含MAX96712的头文件
#define DESER_START_STREAMING_DEEPTH abh_max96712_start_streaming_deepth
#define DESER_STOP_STREAMING         abh_max96712_stop_streaming
#define DESER_MONOPOLIZE_LINK        abh_max96712_monopolize_link
#define DESER_RESTORE_LINK           abh_max96712_restore_link
#define DESER_ENABLE_LINK            abh_max96712_enable_link
#define DESER_LOCK_LINK              abh_max96712_lock_link
#define DESER_UNLOCK_LINK            abh_max96712_unlock_link
#define DESER_CHECK_LINK_STATUS      abh_max96712_check_link_status

#elif defined(USE_MAX96724_DESER)
#include "abh_max96724.h"  // 包含MAX96724的头文件
#define DESER_START_STREAMING_DEEPTH abh_max96724_start_streaming_deepth
#define DESER_STOP_STREAMING         abh_max96724_stop_streaming
#define DESER_MONOPOLIZE_LINK        abh_max96724_monopolize_link
#define DESER_RESTORE_LINK           abh_max96724_restore_link
#define DESER_ENABLE_LINK            abh_max96724_enable_link
#define DESER_LOCK_LINK              abh_max96724_lock_link
#define DESER_UNLOCK_LINK            abh_max96724_unlock_link
#define DESER_CHECK_LINK_STATUS      abh_max96724_check_link_status
#endif

#include "abh_ov9282_max9295d_mode_tbls.h"

#define MAX9295d_ALTER_ADDR_BASE 0x20

struct abh_max9295d_deepth {
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

const struct of_device_id abh_max9295d_deepth_of_match[] = {
	{ .compatible = "Connor,abh_deepth_max9295d",},
	{ },
};
MODULE_DEVICE_TABLE(of, abh_max9295d_deepth_of_match);

static const u32 ctrl_cid_list[] = {
	TEGRA_CAMERA_CID_SENSOR_MODE_ID,
};

static int abh_max9295d_deepth_read_reg(struct camera_common_data *s_data,
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
		dev_info(dev,"abh:abh_max9295d_deepth:r addr(0x%04x)<-0x%02x\n",addr,*val);
	}
	if (err)
		dev_err(dev, "abh:abh_max9295d_deepth: i2c read failed, addr=0x%x\n", addr);

	return err;
}

static int abh_max9295d_deepth_write_reg(struct camera_common_data *s_data,
				u16 addr, u8 val)
{
	int err;
	struct device *dev = s_data->dev;

	struct i2c_client *client = to_i2c_client(s_data->dev);
	dev_info(dev, "I2C addr: 0x%02x\n", client->addr);


	err = regmap_write(s_data->regmap, addr, val);
	if (err)
		dev_err(dev, "abh:%s:abh_max9295d_deepth:i2c write failed, 0x%x = %x\n",
			__func__, addr, val);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"abh:abh_max9295d_deepth:w addr(0x%04x)->0x%02x\n",addr,val);
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
		dev_info(&client->dev,"abh:abh_max9295d_deepth:w slave=0x%02x addr(0x%04x)->0x%02x\n",slaveaddr,regaddr,data);
		return 0;
	}else{
		dev_err(&client->dev, "abh:%s:abh_max9295d_deepth:i2c write failed, slave=0x%02x addr(0x%04x)->0x%02x\n",
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
		dev_info(&client->dev,"abh:abh_max9295d_deepth:r slave=0x%02x addr(0x%04x)<-0x%02x\n",slaveaddr, regaddr,*data);
		return 0;
	}else{
		dev_err(&client->dev, "abh: %s abh_max9295d_deepth: i2c read failed, slave=0x%02x, addr=0x%04x\n", __func__, slaveaddr, regaddr);
		return -1;
	}
}


// static int abh_max9295d_deepth_write_block(struct camera_common_data *s_data,
//                                     u8 *buf, int len)
// {
//     struct i2c_client *client = to_i2c_client(s_data->dev);
//     struct device *dev = s_data->dev;
//     struct i2c_msg msg;
//     int ret, i;

//     msg.addr  = client->addr;
//     msg.flags = 0;   /* 写 */
//     msg.len   = len;
//     msg.buf   = buf;

//     ret = i2c_transfer(client->adapter, &msg, 1);
//     if (ret == 1) {
//         dev_info(dev, "abh:w block I2C addr=0x%02x len=%d data:",
//                  client->addr, len);
//         for (i = 0; i < len; i++)
//             pr_cont(" 0x%02x", buf[i]);
//         pr_cont("\n");
//         return 0;
//     } else {
//         dev_err(dev, "abh:w block failed I2C addr=0x%02x len=%d ret=%d\n",
//                 client->addr, len, ret);
//         return -EIO;
//     }

// 	usleep_range(100, 110);
// }


// static int abh_max9295d_deepth_read_block(struct camera_common_data *s_data,
//                                    u8 *buf, int len)
// {
//     struct i2c_client *client = to_i2c_client(s_data->dev);
//     struct device *dev = s_data->dev;
//     struct i2c_msg msg;
//     int ret, i;

//     msg.addr  = client->addr;
//     msg.flags = I2C_M_RD;   /* 读 */
//     msg.len   = len;
//     msg.buf   = buf;

//     ret = i2c_transfer(client->adapter, &msg, 1);
//     if (ret == 1) {
//         dev_info(dev, "abh:r block I2C addr=0x%02x len=%d data:",
//                  client->addr, len);
//         for (i = 0; i < len; i++)
//             pr_cont(" 0x%02x", buf[i]);
//         pr_cont("\n");
//         return 0;
//     } else {
//         dev_err(dev, "abh:r block failed I2C addr=0x%02x len=%d ret=%d\n",
//                 client->addr, len, ret);
//         return -EIO;
//     }

// 	usleep_range(100, 110);
// }


static int abh_max9295d_deepth_power_on(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;

	dev_info(dev, "abh: %s\n", __func__);
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

static int abh_max9295d_deepth_power_off(struct camera_common_data *s_data)
{
	int err = 0;
	struct camera_common_power_rail *pw = s_data->power;
	struct camera_common_pdata *pdata = s_data->pdata;
	struct device *dev = s_data->dev;

	dev_info(dev, "abh: %s\n", __func__);

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

static int abh_max9295d_deepth_power_get(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;

	pw->state = SWITCH_OFF;

	return 0;
}

static int abh_max9295d_deepth_power_put(struct tegracam_device *tc_dev)
{
	struct camera_common_data *s_data = tc_dev->s_data;
	struct camera_common_power_rail *pw = s_data->power;

	if (unlikely(!pw))
		return -EFAULT;

	return 0;
}

static int abh_max9295d_deepth_set_group_hold(struct tegracam_device *tc_dev, bool val)
{
	dev_info(tc_dev->dev,"abh: %s\n",__func__);
	return 0;
}

static struct tegracam_ctrl_ops abh_max9295d_deepth_ctrl_ops = {
	.numctrls = ARRAY_SIZE(ctrl_cid_list),
	.ctrl_cid_list = ctrl_cid_list,
	.set_group_hold = abh_max9295d_deepth_set_group_hold,
};

static struct camera_common_pdata *abh_max9295d_deepth_parse_dt(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	struct device_node *node = dev->of_node;
	struct camera_common_pdata *board_priv_pdata;
	const struct of_device_id *match;

	if (!node)
		return NULL;

	match = of_match_device(abh_max9295d_deepth_of_match, dev);
	if (!match) {
		dev_err(dev, "Failed to find matching dt id\n");
		return NULL;
	}

	board_priv_pdata = devm_kzalloc(dev, sizeof(*board_priv_pdata), GFP_KERNEL);

	return board_priv_pdata;
}

static int abh_max9295d_deepth_set_mode(struct tegracam_device *tc_dev)
{
	dev_info(tc_dev->dev,"abh:abh_max9295d_deepth set mode\n");
	return 0;
}

// u8 wbuf1[5] = {0x01, 0x02, 0x05, 0x04, 0x6c};
// u8 wbuf2[4] = {0x01, 0x07, 0x04, 0x6c};
// u8 rbuf[7];

static int abh_max9295d_deepth_start_streaming(struct tegracam_device *tc_dev)
{
	struct abh_max9295d_deepth *priv = (struct abh_max9295d_deepth *)tegracam_get_privdata(tc_dev);
	struct device *dev = tc_dev->dev;
	int err;

	mutex_lock(&serdes_lock__);
	dev_info(dev,"abh: abh_max9295d_deepth start streaming - enter\n");

    iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0308, 0x7c);
	iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0311, 0xc3);
	iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0330, 0x06);
	iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0331, 0x11);
	iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0002, 0xf3);
	iic_write(priv->i2c_client, MAX9295d_ALTER_ADDR_BASE + priv->des_link, 0x0314, 0x6A); 

	err = DESER_START_STREAMING_DEEPTH(priv->dser_dev, &priv->g_ctx);
	if (err)
		goto exit;

	// abh_max9295d_deepth_write_block(priv->s_data, wbuf1, sizeof(wbuf1));
	// mdelay(100);
	// abh_max9295d_deepth_write_block(priv->s_data, wbuf2, sizeof(wbuf2));
	// mdelay(100);
	// abh_max9295d_deepth_read_block(priv->s_data, rbuf, sizeof(rbuf));


	dev_info(dev,"abh: abh_max9295d_deepth start streaming - exit\n");
	mutex_unlock(&serdes_lock__);
	return 0;

exit:
	mutex_unlock(&serdes_lock__);
	if(err){
		dev_err(dev, "abh: %s: error setting stream\n", __func__);
	}
	dev_info(dev,"abh: abh_max9295d_deepth start streaming - exit\n");
	return err;
}

static int abh_max9295d_deepth_stop_streaming(struct tegracam_device *tc_dev)
{
	struct device *dev = tc_dev->dev;
	// struct abh_max9295d_deepth *priv = (struct abh_max9295d_deepth *)tegracam_get_privdata(tc_dev);

	dev_info(dev,"abh:abh_max9295d_deepth stop streaming\n");
	/* disable serdes streaming */
	// DESER_STOP_STREAMING(priv->dser_dev, &priv->g_ctx);

	return 0;
}

static int abh_max9295d_deepth_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	dev_dbg(&client->dev, "%s:\n", __func__);

	return 0;
}

static const struct v4l2_subdev_internal_ops abh_max9295d_deepth_subdev_internal_ops = {
	.open = abh_max9295d_deepth_open,
};

static struct camera_common_sensor_ops abh_max9295d_deepth_common_ops = {
	.numfrmfmts = ARRAY_SIZE(abh_max9295d_frmfmt_deepth),
	.frmfmt_table = abh_max9295d_frmfmt_deepth,
	.power_on = abh_max9295d_deepth_power_on,
	.power_off = abh_max9295d_deepth_power_off,
	.write_reg = abh_max9295d_deepth_write_reg,
	.read_reg = abh_max9295d_deepth_read_reg,
	.parse_dt = abh_max9295d_deepth_parse_dt,
	.power_get = abh_max9295d_deepth_power_get,
	.power_put = abh_max9295d_deepth_power_put,
	.set_mode = abh_max9295d_deepth_set_mode,
	.start_streaming = abh_max9295d_deepth_start_streaming,
	.stop_streaming = abh_max9295d_deepth_stop_streaming,
};

static int abh_max9295d_deepth_board_setup(struct abh_max9295d_deepth *priv)
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
		dev_err(dev, "abh: reg not found\n");
		goto error;
	}

	err = of_property_read_u32(node, "def-addr",
					&priv->def_addr);
	if (err < 0) {
		dev_err(dev, "abh: def-addr not found\n");
		goto error;
	}

	priv->g_ctx.ser_dev = dev;

	dser_node = of_parse_phandle(node, "nvidia,gmsl-dser-device", 0);
	if (dser_node == NULL) {
		dev_err(dev,
			"abh: missing %s handle\n",	"nvidia,gmsl-dser-device");
		err = -EINVAL;
		goto error;
	}

	dser_i2c = of_find_i2c_device_by_node(dser_node);
	of_node_put(dser_node);

	if (dser_i2c == NULL) {
		dev_err(dev, "abh: missing deserializer dev handle\n");
		err = -EINVAL;
		goto error;
	}
	if (dser_i2c->dev.driver == NULL) {
		dev_err(dev, "abh: missing deserializer driver\n");
		err = -EINVAL;
		goto error;
	}

	priv->dser_dev = &dser_i2c->dev;
	priv->g_ctx.des_dev =  &dser_i2c->dev;

	err = of_property_read_string(node, "des-link", &str_value);
    if (err < 0) {
        dev_err(dev, "abh: des-link property is not found\n");
        return -EINVAL;
    }
    dev_info(dev, "abh: link is %s\n",str_value);
 
    priv->des_link = str_value[0]-'A';
	priv->g_ctx.des_link = priv->des_link;

	/* populate g_ctx from DT */
	gmsl = of_get_child_by_name(node, "gmsl-link");
	if (gmsl == NULL) {
		dev_err(dev, "abh: missing gmsl-link device node\n");
		err = -EINVAL;
		goto error;
	}

	err = of_property_read_u32(gmsl, "num-ser-lanes", &value);
	if (err < 0) {
		dev_err(dev, "abh: No num-lanes info\n");
		goto error;
	}
	priv->g_ctx.num_ser_csi_lanes = value;

	priv->g_ctx.num_streams =
			of_property_count_strings(gmsl, "streams");
	if (priv->g_ctx.num_streams <= 0) {
		dev_err(dev, "abh: No streams found\n");
		err = -EINVAL;
		goto error;
	}

	for (i = 0; i < priv->g_ctx.num_streams; i++) {
		of_property_read_string_index(gmsl, "streams", i,
						&str_value1[i]);
		if (!str_value1[i]) {
			dev_err(dev, "abh: invalid stream info\n");
			goto error;
		}
		if (!strcmp(str_value1[i], "raw8")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_RAW_8;
		} else if (!strcmp(str_value1[i], "embed")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_EMBED;
		} else if (!strcmp(str_value1[i], "yuv10")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_RAW_10;
		} else if (!strcmp(str_value1[i], "yuv16")) {
			priv->g_ctx.streams[i] =
							GMSL_CSI_DT_YUV16;
		}  else {
			dev_err(dev, "abh: invalid stream data type\n");
			goto error;
		}
	}

	priv->g_ctx.sensor_dev = dev;

	return 0;

error:
	dev_err(dev, "abh: board setup failed\n");
	if(err==0) err=-EINVAL;
	return err;
}

static int abh_max9295d_deepth_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct device *dev = &client->dev;
	struct device_node *node = dev->of_node;
	struct tegracam_device *tc_dev;
	struct abh_max9295d_deepth *priv;
	int err;
	u8 val;

    dev_info(dev, "abh: probing v4l2 sensor @0x%x.\n",client->addr);

    if (!IS_ENABLED(CONFIG_OF) || !node)
		return -EINVAL;

    priv = devm_kzalloc(dev, sizeof(struct abh_max9295d_deepth), GFP_KERNEL);
	if (!priv) {
		dev_err(dev, "abh: unable to allocate memory!\n");
		return -ENOMEM;
	}

	tc_dev = devm_kzalloc(dev, sizeof(struct tegracam_device), GFP_KERNEL);
	if (!tc_dev)
		return -ENOMEM;

	priv->i2c_client = tc_dev->client = client;
	tc_dev->dev = dev;
	strncpy(tc_dev->name, "abh_max9295d_deepth", sizeof(tc_dev->name));
	tc_dev->dev_regmap_config = &sensor_regmap_config;
	tc_dev->sensor_ops = &abh_max9295d_deepth_common_ops;
	tc_dev->v4l2sd_internal_ops = &abh_max9295d_deepth_subdev_internal_ops;
	tc_dev->tcctrl_ops = &abh_max9295d_deepth_ctrl_ops;

	err = abh_max9295d_deepth_board_setup(priv);
	if (err) {
		dev_err(dev, "abh: board setup failed\n");
		return err;
	}

	mutex_lock(&serdes_lock__);
	// DESER_LOCK_LINK(priv->dser_dev);
	// if ( DESER_CHECK_LINK_STATUS(priv->dser_dev, priv->des_link) ){
	// 	DESER_UNLOCK_LINK(priv->dser_dev);
	// 	dev_err(dev, "abh: link_%c is occupied\n",'A'+priv->des_link);
	// 	return -EINVAL;
	// }

	err = tegracam_device_register(tc_dev);
	if (err) {
		mutex_unlock(&serdes_lock__);
		// DESER_UNLOCK_LINK(priv->dser_dev);
		dev_err(dev, "abh: tegra camera driver registration failed\n");
		return err;
	}

	priv->tc_dev = tc_dev;
	priv->s_data = tc_dev->s_data;
	priv->subdev = &tc_dev->s_data->subdev;
	tegracam_set_privdata(tc_dev, (void *)priv);

	DESER_MONOPOLIZE_LINK(priv->dser_dev, priv->des_link);
	mdelay(50);
	iic_write(client,0x40, 0x0000, (MAX9295d_ALTER_ADDR_BASE+priv->des_link)<<1);

	mdelay(10);

	iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link, 0x02bf, 0xa0);
	iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link, 0x02be, 0x18);

	err = iic_read(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link, 0x000d, &val);
	if (err || val != 0x95) {
		dev_err(dev, "abh access abh_max9295d_deepth's 9295d failed\n");
		DESER_RESTORE_LINK(priv->dser_dev);
		tegracam_device_unregister(tc_dev);
		mutex_unlock(&serdes_lock__);
		// DESER_UNLOCK_LINK(priv->dser_dev);
		return -EINVAL;
	}

	iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link, 0x0042, priv->act_addr<<1); //dst addr
	iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link, 0x0043, priv->def_addr<<1); //src addr, original sensor addr

    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x007b, 0x30+priv->des_link);
    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x0083, 0x30+priv->des_link);
	iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x008b, 0x30+priv->des_link);
    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x0093, 0x30+priv->des_link);
    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x009b, 0x30+priv->des_link);
    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x00a3, 0x30+priv->des_link);
    iic_write(client, MAX9295d_ALTER_ADDR_BASE+priv->des_link,0x00ab, 0x30+priv->des_link);

	DESER_ENABLE_LINK(priv->dser_dev, priv->des_link);
	DESER_RESTORE_LINK(priv->dser_dev);
	mutex_unlock(&serdes_lock__);
	// DESER_UNLOCK_LINK(priv->dser_dev);

	err = tegracam_v4l2subdev_register(tc_dev, true);
	if (err) {
		dev_err(dev, "abh: tegra camera subdev registration failed\n");
		return err;
	}

	dev_info(&client->dev, "abh:Detected abh_max9295d_deepth sensor\n");

	return 0;
}

static int abh_max9295d_deepth_remove(struct i2c_client *client)
{
	struct camera_common_data *s_data = to_camera_common_data(&client->dev);
	struct abh_max9295d_deepth *priv = (struct abh_max9295d_deepth *)s_data->priv;

	tegracam_v4l2subdev_unregister(priv->tc_dev);
	tegracam_device_unregister(priv->tc_dev);

	dev_info(&client->dev,"abh, abh_max9295d_deepth_remove sensor @0x%x.\n",client->addr);
	return 0;
}

static const struct i2c_device_id abh_max9295d_deepth_id[] = {
	{ "abh_max9295d_deepth", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, abh_max9295d_deepth_id);

static struct i2c_driver abh_max9295d_deepth_i2c_driver = {
	.driver = {
		.name = "abh_max9295d_deepth",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(abh_max9295d_deepth_of_match),
	},
	.probe = abh_max9295d_deepth_probe,
	.remove = abh_max9295d_deepth_remove,
	.id_table = abh_max9295d_deepth_id,
};

static int __init abh_max9295d_deepth_init(void)
{
	mutex_init(&serdes_lock__);

	return i2c_add_driver(&abh_max9295d_deepth_i2c_driver);
}

static void __exit abh_max9295d_deepth_exit(void)
{
	mutex_destroy(&serdes_lock__);

	i2c_del_driver(&abh_max9295d_deepth_i2c_driver);
}

late_initcall(abh_max9295d_deepth_init);
module_exit(abh_max9295d_deepth_exit);

MODULE_SOFTDEP("pre: abh_gc2145_max9295d");
MODULE_DESCRIPTION("GMSL Deserializer driver abh_max9295d_deepth");
MODULE_AUTHOR("Connor connor.zhou@foxmail.com");
MODULE_LICENSE("GPL v2");