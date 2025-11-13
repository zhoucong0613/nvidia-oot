#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <media/camera_common.h>
#include <linux/module.h>

#include "fpga_max96724.h"

#define fpga_max96724_TX11_PIPE_X_EN_ADDR 0x90B
#define fpga_max96724_TX45_PIPE_X_DST_CTRL_ADDR 0x92D
#define fpga_max96724_PIPE_X_SRC_0_MAP_ADDR 0x90D

#define fpga_max96724_PHY_CLK (25)

#define CHECKNULL(x) if(x==NULL){ \
		dev_err(NULL, "fpga %s: ##x## NULL ptr\n",__func__); \
		return -EINVAL; \
		}

struct fpga_max96724 {
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	struct mutex lock;
	u8 link_lock_status;
	u8 ismaster;
};

static int vercheck=0;
static int check_version(struct i2c_client *client);
static int check_device_id_rev(struct i2c_client *client);

static int fpga_max96724_write_reg(struct device *dev,
	u16 addr, u8 val)
{
	struct fpga_max96724 *priv;
	int err;

	priv = dev_get_drvdata(dev);

	err = regmap_write(priv->regmap, addr, val);
	if (err)
		dev_err(dev,
		"fpga %s:i2c write failed, 0x%x = %x\n",
		__func__, addr, val);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"fpga:96724:w addr(0x%04x)->0x%02x\n",addr,val);

	return err;
}

static int fpga_max96724_read_reg(struct device *dev,
	u16 addr, u32* val)
{
	struct fpga_max96724 *priv;
	int err;

	priv = dev_get_drvdata(dev);

	err = regmap_read(priv->regmap, addr, val);
	if (err)
		dev_err(dev,
		"fpga %s:i2c read failed, 0x%x\n",
		__func__, addr);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"fpga:96724:r addr(0x%04x)<-0x%02x\n",addr,(u8)(*val));
	return err;
}

int fpga_max96724_lock_link(struct device *dev)
{
	struct fpga_max96724 *priv = NULL;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	mutex_lock(&priv->lock);
	dev_info(dev,"fpga: mutex lock");
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_lock_link);

int fpga_max96724_unlock_link(struct device *dev)
{
	struct fpga_max96724 *priv = NULL;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"fpga: mutex unlock");
	mutex_unlock(&priv->lock);
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_unlock_link);

int fpga_max96724_monopolize_link(struct device *dev, int link)
{
	struct fpga_max96724 *priv = NULL;
	u32 value;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "fpga: %s: monopolize link_%c\n", __func__, 'A'+link);
	fpga_max96724_read_reg(dev,0x0006,&value);
	value = (value & 0xF0) | (1<<link);
	fpga_max96724_write_reg(dev,0x0006,value);
	mdelay(150);
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_monopolize_link);

int fpga_max96724_enable_link(struct device *dev, int link)
{
    struct fpga_max96724 *priv = NULL;
 
    CHECKNULL(dev);
    priv = dev_get_drvdata(dev);
    CHECKNULL(priv);
 
    dev_info(dev, "fpga: %s: enable link_%c\n", __func__, 'A'+link);
    priv->link_lock_status |= (1<<link);
    return 0;
}
EXPORT_SYMBOL(fpga_max96724_enable_link);

int fpga_max96724_restore_link(struct device *dev)
{
	struct fpga_max96724 *priv = NULL;
	u32 value;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "fpga: restore links\n");
	fpga_max96724_read_reg(dev,0x0006,&value);
	fpga_max96724_write_reg(dev,0x0006,(value&0xF0)|(priv->link_lock_status&0x0F));
	mdelay(150);
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_restore_link);

int fpga_max96724_check_link_status(struct device *dev, int link)
{
	struct fpga_max96724 *priv = NULL;
	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);
	if(priv->link_lock_status & (0x1<<link)){
		return 1;
	}else{
		return 0;
	}
}
EXPORT_SYMBOL(fpga_max96724_check_link_status);

int fpga_max96724_set_link_bandwidth(struct device *dev, int link, int gbps)
{
	struct fpga_max96724 *priv = NULL;
	u32 value;
	u16 reg = 0x0010;
	u8 regval = 0x01;
	if(gbps!=3){ //6Gbps
		regval = 0x02;
	}

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "fpga: set link_%c to %dGbps\n",link+'A',gbps);
	if(link>=2){
		reg++, link-=2;
	}
	fpga_max96724_read_reg(dev,reg,&value);
	value = (value & (~(0x3<<(link*4)))) | (regval<<(link*4));
	fpga_max96724_write_reg(dev,reg,value & 0xFF);
	fpga_max96724_write_reg(dev,0x0018,(1<<link));
	mdelay(300);
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_set_link_bandwidth);

static int fpga_max96724_setup_pipeline(struct device *dev,struct gmsl_link_ctx *g_ctx)
{
	struct fpga_max96724 *priv = NULL;
	int pipe_id = 0;
	u32 i = 0;
	u8 dst_vc = 0,src_vc = 0;


	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"fpga:96724: num_streams=%d\n",g_ctx->num_streams);

	for (i = 0; i < g_ctx->num_streams; i++) {
		pipe_id = g_ctx->des_link;
		dev_info(dev, "fpga: pipe_%d receive stream_id=%d, datatype=0x%x\n", pipe_id,i,g_ctx->streams[i]);

		dst_vc = (g_ctx->des_link==0 || g_ctx->des_link==2) ? 0:1; //for link B and linkD set vc=2

		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_TX11_PIPE_X_EN_ADDR+0, 0x1f);
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_TX45_PIPE_X_DST_CTRL_ADDR+0, (g_ctx->des_link<2)?0x55:0xAA);  // 0x55=controller1  0xAA=controller2
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_TX45_PIPE_X_DST_CTRL_ADDR+1, (g_ctx->des_link<2)?0x01:0x02); 
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+0, g_ctx->streams[i] | (src_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+1, g_ctx->streams[i] | (dst_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+2, 0x00 | (src_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+3, 0x00 | (dst_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+4, 0x01 | (src_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+5, 0x01 | (dst_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+6, 0x02 | (src_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+7, 0x02 | (dst_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+8, 0x03 | (src_vc << 6));
		fpga_max96724_write_reg(dev, (0x40*pipe_id)+fpga_max96724_PIPE_X_SRC_0_MAP_ADDR+9, 0x03 | (dst_vc << 6));			
	}

	return 0;
}

int fpga_max96724_start_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct fpga_max96724 *priv = NULL;
	int err;
	u32 val;
	
	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"fpga: 96724 start streaming, link=%c - enter\n",'A'+g_ctx->des_link);

	mutex_lock(&priv->lock);
	err = fpga_max96724_setup_pipeline(dev, g_ctx);
	if (err)
		return err;

	fpga_max96724_read_reg(dev,0xf4,&val);
	fpga_max96724_write_reg(dev,0xf4, (val | (1<<g_ctx->des_link))&0xff );

	// //reset mipi phy
	// if(s_priv->des_link==2 || s_priv->des_link==3){
	// 	fpga_max96724_write_reg(dev,0x8a2, 0x30);
	// }else{
	// 	fpga_max96724_write_reg(dev,0x8a2, 0xc0);
	// }
	// fpga_max96724_write_reg(dev,0x8a2, 0xf0);

	// reset link
	fpga_max96724_write_reg(dev,0x018, (1<<g_ctx->des_link));
	mdelay(150);
	
	//skew
	if(g_ctx->des_link==0 || g_ctx->des_link==1){
		fpga_max96724_write_reg(dev,0x0943,0x10);
		fpga_max96724_write_reg(dev,0x0943,0x33);
	}else{
		fpga_max96724_write_reg(dev,0x0983,0x10);
		fpga_max96724_write_reg(dev,0x0983,0x33);
	}

	mutex_unlock(&priv->lock);
	dev_info(dev,"fpga: 96724 start streaming - exit\n");
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_start_streaming);

int fpga_max96724_stop_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct fpga_max96724 *priv = NULL;
	u32 val;
	u8 tmp;

	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"fpga: 96724 stop streaming, link_%c - enter\n",'A'+g_ctx->des_link);
	mutex_lock(&priv->lock);

	fpga_max96724_read_reg(dev,0xf4,&val);
	tmp = (val & (~(1<<g_ctx->des_link))) & 0xff;
	fpga_max96724_write_reg(dev,0xf4, tmp );
	if(tmp==0){
		fpga_max96724_write_reg(dev,0x018, 0x0f);
		dev_info(dev,"fpga: all stopped, reset links\n");
		mdelay(30);
	}

	mutex_unlock(&priv->lock);
	dev_info(dev,"fpga: 96724 stop streaming - exit\n");
	return 0;
}
EXPORT_SYMBOL(fpga_max96724_stop_streaming);

const struct of_device_id fpga_max96724_of_match[] = {
	{ .compatible = "Connor,fpga_max96724", },
	{ },
};
MODULE_DEVICE_TABLE(of, fpga_max96724_of_match);

static int fpga_max96724_parse_dt(struct fpga_max96724 *priv,
				struct i2c_client *client)
{
	struct device_node *node = client->dev.of_node;
	const struct of_device_id *match;

	if (!node)
		return -EINVAL;

	match = of_match_device(fpga_max96724_of_match, &client->dev);
	if (!match) {
		dev_err(&client->dev, "Failed to find matching dt id\n");
		return -EFAULT;
	}

	return 0;
}

static struct regmap_config fpga_max96724_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
};

static int get_mipifreq(void)
{
	struct file *file;
	loff_t pos = 0;
	char buf[10];
	int freq = 0;
	ssize_t ret;

	file = filp_open("/etc/mipifreq", O_RDONLY, 0);
	if (IS_ERR(file)) {
		printk("fpga: no mipifreq define file\n");
		goto defaultval;
	}

	ret = kernel_read(file, buf, 4, &pos);
	filp_close(file, NULL);
	if (ret > 0) {
		buf[ret] = 0;
		printk("fpga: read from mipifreq: %s\n", buf);
		freq = simple_strtoul(buf,NULL,10);
		freq = freq / 100;
		if(freq<2 || freq>25){
			printk("fpga: mipifreq out of range: %d, use default\n", freq);
			goto defaultval;
		}
		return freq;
	} else {
		printk("fpga: failed to read from mipifreq\n");
		goto defaultval;
	}

defaultval:
	return (fpga_max96724_PHY_CLK&0x1f);
}

static int fpga_max96724_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
    struct fpga_max96724 *priv;
    int err = 0;
	u32 val = 0;
	struct device_node *node = client->dev.of_node;
	int mipifreq = 0;

    dev_info(&client->dev, "fpga: [fpga_max96724]: probing GMSL Deserializer @0x%x\n",client->addr);

    priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
    priv->i2c_client = client;
	priv->regmap = devm_regmap_init_i2c(priv->i2c_client, &fpga_max96724_regmap_config);
	if (IS_ERR(priv->regmap)) {
		dev_err(&client->dev,
			"regmap init failed: %ld\n", PTR_ERR(priv->regmap));
		return -ENODEV;
	}

    err = fpga_max96724_parse_dt(priv, client);
	if (err) {
		dev_err(&client->dev, "fpga: unable to parse dt\n");
		return -EFAULT;
	}

	mutex_init(&priv->lock);

    dev_set_drvdata(&client->dev, priv);
	
	err=check_device_id_rev(client);
	if(err){
		dev_err(&client->dev, "fpga: failed on check id & rev\n");
		return -EFAULT;
	}

	if (of_get_property(node, "is-master", NULL)) {
		priv->ismaster = 1;
		dev_info(&client->dev, "fpga: this fpga_max96724 is master\n");
	}else{
		priv->ismaster = 0;
		dev_info(&client->dev, "fpga: this fpga_max96724 is slave\n");
	}

	if( check_version(client)<0 ){
		return -EFAULT;
	}

	err=fpga_max96724_write_reg(&client->dev,0x0013,0x40); //reset all
	mdelay(100);
	err=fpga_max96724_write_reg(&client->dev,0x0013,0x00);

#if 1
	err=fpga_max96724_write_reg(&client->dev,0x0005,0x88);
	err=fpga_max96724_write_reg(&client->dev,0x0310,0x90); 
#else
	err=fpga_max96724_write_reg(&client->dev,0x0005,0xc8);
#endif

	//turn on LED
	err=fpga_max96724_write_reg(&client->dev,0x0309,0x90);

	//disable Links which are not Locked
	err=fpga_max96724_read_reg(&client->dev,0x0006,&val);
	err=fpga_max96724_write_reg(&client->dev,0x0006,(val&0xF0)|(priv->link_lock_status&0x0F));


	fpga_max96724_write_reg(&client->dev,0x90a,0xc0); //4 lanes, D-Phy, 2bits VC
	fpga_max96724_write_reg(&client->dev,0x94a,0xc0); //4 lanes, D-Phy, 2bits VC
	fpga_max96724_write_reg(&client->dev,0x98a,0x40); //2 lanes, D-Phy, 2bits VC
	fpga_max96724_write_reg(&client->dev,0x9ca,0x40); //2 lanes, D-Phy, 2bits VC

	fpga_max96724_write_reg(&client->dev,0x8a3,0xe4); //phy lane mapping
	fpga_max96724_write_reg(&client->dev,0x8a4,0xe4); //phy lane mapping

    fpga_max96724_write_reg(&client->dev,0x8a0,0x24); // 2x4 mode

	mipifreq = get_mipifreq();
	fpga_max96724_write_reg(&client->dev,0x415,0x20|mipifreq);
	fpga_max96724_write_reg(&client->dev,0x418,0x20|mipifreq);
	fpga_max96724_write_reg(&client->dev,0x41b,0x20|mipifreq);
	fpga_max96724_write_reg(&client->dev,0x41e,0x20|mipifreq);

	fpga_max96724_write_reg(&client->dev,0xf0,0x62); //pipe1 GMSLB, Z-pipe; pipe0 GMSLA, Z-pipe
	fpga_max96724_write_reg(&client->dev,0xf1,0xea); //pipe3 GMSLD, Z-pipe; pipe2 GMSLC, Z-pipe
	fpga_max96724_write_reg(&client->dev,0xf4,0x00); //disable pipes 0-4 (default val)

	//mfp0, enable TX with ID=A
    fpga_max96724_write_reg(&client->dev,0x0300,0x83); //TX on A
	fpga_max96724_write_reg(&client->dev,0x0301,0xaa); //TX on A
	fpga_max96724_write_reg(&client->dev,0x0337,0x2a); //TX on B
	fpga_max96724_write_reg(&client->dev,0x036d,0x2a); //TX on C
	fpga_max96724_write_reg(&client->dev,0x03a4,0x2a); //TX on D

	fpga_max96724_write_reg(&client->dev,0x0027,0x00); //disable unconcerned error report
	fpga_max96724_write_reg(&client->dev,0x0029,0xf0); //disable unconcerned error report

	dev_info(&client->dev, "fpga: %s:  success\n", __func__);
	return err;
}

static int check_device_id_rev(struct i2c_client *client)
{
	int err=0;
	unsigned int value=0;

	err=fpga_max96724_read_reg(&client->dev,0x004C,&value);
	dev_info(&client->dev,"fpga:device revision=0x%x\n",(value&0x0f));
	return err;
}

static int check_version(struct i2c_client *client)
{
	struct fpga_max96724 *priv = NULL;
	const uint16_t ver0addr = 0x0316; //ver0 : mfp7
	const uint16_t ver1addr = 0x0319; //ver1 : mfp8

	uint16_t veraddrs[2] = {ver0addr,ver1addr};
	uint32_t ver=0,tmp;
	int i=0,err=0;

	priv = dev_get_drvdata(&client->dev);
	CHECKNULL(priv);

	if(priv->ismaster){
		for(i=0;i<2;i++){
			err=fpga_max96724_read_reg(&client->dev,veraddrs[i],&tmp);
			tmp = (tmp & 0x8)>>3;
			ver = ver | (tmp<<i);
		}
		dev_info(&client->dev,"fpga: version=%d\n",ver);
		if(ver==0x01){
			vercheck=1;
		}else{
			vercheck=-1;
			err=-1;
		}
	}else{
		if(vercheck<0){
			err=-1;
		}
	}

	return err;
}

static int fpga_max96724_remove(struct i2c_client *client)
{

	struct fpga_max96724 *priv;
	
	dev_info(&client->dev, "fpga: %s:  success\n", __func__);

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		mutex_destroy(&priv->lock);
		client = NULL;
	}

	return 0;
}

static const struct i2c_device_id fpga_max96724_id[] = {
	{ "fpga_max96724", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, fpga_max96724_id);

static struct i2c_driver fpga_max96724_i2c_driver = {
	.driver = {
		.name = "fpga_max96724",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(fpga_max96724_of_match),
	},
	.probe = fpga_max96724_probe,
	.remove = fpga_max96724_remove,
	.id_table = fpga_max96724_id,
};

static int __init fpga_max96724_init(void)
{
	return i2c_add_driver(&fpga_max96724_i2c_driver);
}

static void __exit fpga_max96724_exit(void)
{
	i2c_del_driver(&fpga_max96724_i2c_driver);
}

module_init(fpga_max96724_init);
module_exit(fpga_max96724_exit);

MODULE_DESCRIPTION("GMSL Deserializer driver fpga_max96724");
MODULE_AUTHOR("Connor connor.zhou@foxmail.com");
MODULE_LICENSE("GPL v2");