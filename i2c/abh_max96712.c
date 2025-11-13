/* abh */

#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <media/camera_common.h>
#include <linux/module.h>

#include "abh_max96712.h"

#define MAX96712_TX11_PIPE_X_EN_ADDR 0x90B
#define MAX96712_TX45_PIPE_X_DST_CTRL_ADDR 0x92D
#define MAX96712_PIPE_X_SRC_0_MAP_ADDR 0x90D


#define MAX96712_PHY_CLK (0x20|25)

struct max96712 {
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	struct mutex lock;
	u8 link_lock_status;
	u8 ismaster;
};

#define CHECKNULL(x) if(x==NULL){ \
		dev_err(NULL, "abh %s: ##x## NULL ptr\n",__func__); \
		return -EINVAL; \
		}

static int vercheck=0;
static int check_version(struct i2c_client *client);
static int check_device_id_rev(struct i2c_client *client);
// static int check_link(struct i2c_client *client);

static int max96712_write_reg(struct device *dev,
	u16 addr, u8 val)
{
	struct max96712 *priv;
	int err;

	priv = dev_get_drvdata(dev);

	err = regmap_write(priv->regmap, addr, val);
	if (err)
		dev_err(dev,
		"abh %s:i2c write failed, 0x%x = %x\n",
		__func__, addr, val);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"abh:96712:w addr(0x%04x)->0x%02x\n",addr,val);

	return err;
}

static int max96712_read_reg(struct device *dev,
	u16 addr, u32* val)
{
	struct max96712 *priv;
	int err;

	priv = dev_get_drvdata(dev);

	err = regmap_read(priv->regmap, addr, val);
	if (err)
		dev_err(dev,
		"abh %s:i2c read failed, 0x%x\n",
		__func__, addr);

	/* delay before next i2c command as required for SERDES link */
	usleep_range(100, 110);
	if(!err)
		dev_info(dev,"abh:96712:r addr(0x%04x)<-0x%02x\n",addr,(u8)(*val));
	return err;
}

int abh_max96712_lock_link(struct device *dev)
{
	struct max96712 *priv = NULL;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	mutex_lock(&priv->lock);
	dev_info(dev,"abh: mutex lock");
	return 0;
}
EXPORT_SYMBOL(abh_max96712_lock_link);

int abh_max96712_unlock_link(struct device *dev)
{
	struct max96712 *priv = NULL;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"abh: mutex unlock");
	mutex_unlock(&priv->lock);
	return 0;
}
EXPORT_SYMBOL(abh_max96712_unlock_link);

int abh_max96712_monopolize_link(struct device *dev, int link)
{
	struct max96712 *priv = NULL;
	u32 value;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "abh: %s: monopolize link_%c\n", __func__, 'A'+link);
	max96712_read_reg(dev,0x0006,&value);
	value = (value & 0xF0) | (1<<link);
	max96712_write_reg(dev,0x0006,value);
	mdelay(150);
	return 0;
}
EXPORT_SYMBOL(abh_max96712_monopolize_link);

int abh_max96712_enable_link(struct device *dev, int link)
{
    struct max96712 *priv = NULL;
 
    CHECKNULL(dev);
    priv = dev_get_drvdata(dev);
    CHECKNULL(priv);
 
    dev_info(dev, "abh: %s: enable link_%c\n", __func__, 'A'+link);
    priv->link_lock_status |= (1<<link);
    return 0;
}
EXPORT_SYMBOL(abh_max96712_enable_link);

int abh_max96712_restore_link(struct device *dev)
{
	struct max96712 *priv = NULL;
	u32 value;

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "abh: restore links\n");
	max96712_read_reg(dev,0x0006,&value);
	max96712_write_reg(dev,0x0006,(value&0xF0)|(priv->link_lock_status&0x0F));
	mdelay(150);
	return 0;
}
EXPORT_SYMBOL(abh_max96712_restore_link);

int abh_max96712_check_link_status(struct device *dev, int link)
{
	struct max96712 *priv = NULL;
	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);
	if(priv->link_lock_status & (0x1<<link)){
		return 1;
	}else{
		return 0;
	}
}
EXPORT_SYMBOL(abh_max96712_check_link_status);

int abh_max96712_set_link_bandwidth(struct device *dev, int link, int gbps)
{
	struct max96712 *priv = NULL;
	u32 value;
	u16 reg = 0x0010;
	u8 regval = 0x01;
	if(gbps!=3){ //6Gbps
		regval = 0x02;
	}

	CHECKNULL(dev);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev, "abh: set link_%c to %dGbps\n",link+'A',gbps);
	if(link>=2){
		reg++, link-=2;
	}
	max96712_read_reg(dev,reg,&value);
	value = (value & (~(0x3<<(link*4)))) | (regval<<(link*4));
	max96712_write_reg(dev,reg,value & 0xFF);
	max96712_write_reg(dev,0x0018,(1<<link));
	mdelay(300);
	return 0;
}
EXPORT_SYMBOL(abh_max96712_set_link_bandwidth);

static int max96712_setup_pipeline_deepth(struct device *dev,struct gmsl_link_ctx *g_ctx)
{
	struct max96712 *priv = NULL;
	int pipe_id = 0;
	u32 i = 0;
	u8 dst_vc = 0,src_vc = 0;


	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"abh:96712: num_streams=%d\n",g_ctx->num_streams);

	for (i = 0; i < g_ctx->num_streams; i++) {

 		if(g_ctx->des_link == 0)
		{
			pipe_id = 0;
		}
		else if(g_ctx->des_link == 1)
		{
			pipe_id = 2;
		}
        else if(g_ctx->des_link == 2)
		{
			pipe_id = 4;
		}
        else if(g_ctx->des_link == 3)
		{
			pipe_id = 6;
		}
	
		dev_info(dev, "abh: pipe_%d receive stream_id=%d, datatype=0x%x\n", pipe_id,i,g_ctx->streams[i]);

		if(g_ctx->des_link == 0)
		{
			dst_vc = 0;
		}
		else if(g_ctx->des_link == 1)
		{
			dst_vc = 2;
		}
        else if(g_ctx->des_link == 2)
		{
			dst_vc = 0;
		}
        else if(g_ctx->des_link == 3)
		{
			dst_vc = 2;
		}

		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX11_PIPE_X_EN_ADDR+0, 0x1f);
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX45_PIPE_X_DST_CTRL_ADDR+0, (g_ctx->des_link<2)?0x55:0xAA);  // 0x55=controller1  0xAA=controller2
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX45_PIPE_X_DST_CTRL_ADDR+1, (g_ctx->des_link<2)?0x01:0x02); 
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+0, g_ctx->streams[i] | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+1, g_ctx->streams[i] | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+2, 0x00 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+3, 0x00 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+4, 0x01 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+5, 0x01 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+6, 0x02 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+7, 0x02 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+8, 0x03 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+9, 0x03 | (dst_vc << 6));			
	}

	return 0;
}

int abh_max96712_start_streaming_deepth(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct max96712 *priv = NULL;
	int err;
	//u32 val;
	
	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"abh: 96712 start streaming, link=%c - enter\n",'A'+g_ctx->des_link);

	mutex_lock(&priv->lock);
	err = max96712_setup_pipeline_deepth(dev, g_ctx);
	if (err)
		return err;

	max96712_write_reg(dev,0xf4, 0xff);
	// max96712_read_reg(dev,0xf4,&val);
	// max96712_write_reg(dev,0xf4, (val | (1<<g_ctx->des_link))&0xff );

	// //reset mipi phy
	// if(s_priv->des_link==2 || s_priv->des_link==3){
	// 	max96712_write_reg(dev,0x8a2, 0x30);
	// }else{
	// 	max96712_write_reg(dev,0x8a2, 0xc0);
	// }
	// max96712_write_reg(dev,0x8a2, 0xf0);

	// reset link
	max96712_write_reg(dev,0x018, (1<<g_ctx->des_link));
	usleep_range(50*1000, 50*1000+1000);
	mdelay(100);
	
	//skew
	if(g_ctx->des_link==0 || g_ctx->des_link==1){
        max96712_write_reg(dev,0x0903,0x10);
		max96712_write_reg(dev,0x0903,0x33);
		max96712_write_reg(dev,0x0943,0x10);
		max96712_write_reg(dev,0x0943,0x33);
	}else{
		max96712_write_reg(dev,0x0983,0x10);
		max96712_write_reg(dev,0x0983,0x33);
        max96712_write_reg(dev,0x09c3,0x10);
		max96712_write_reg(dev,0x09c3,0x33);
	}

	mutex_unlock(&priv->lock);
	dev_info(dev,"abh: 96712 start streaming - exit\n");
	return 0;
}
EXPORT_SYMBOL(abh_max96712_start_streaming_deepth);

static int max96712_setup_pipeline(struct device *dev,struct gmsl_link_ctx *g_ctx)
{
	struct max96712 *priv = NULL;
	int pipe_id = 0;
	u32 i = 0;
	u8 dst_vc = 0,src_vc = 0;


	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"abh:96712: num_streams=%d\n",g_ctx->num_streams);

	for (i = 0; i < g_ctx->num_streams; i++) {

 		if(g_ctx->des_link == 0)
		{
			pipe_id = 1;
		}
		else if(g_ctx->des_link == 1)
		{
			pipe_id = 3;
		}
        else if(g_ctx->des_link == 2)
		{
			pipe_id = 5;
		}
        else if(g_ctx->des_link == 3)
		{
			pipe_id = 7;
		}
	
		dev_info(dev, "abh: pipe_%d receive stream_id=%d, datatype=0x%x\n", pipe_id,i,g_ctx->streams[i]);

		if(g_ctx->des_link == 0)
		{
			dst_vc = 1;
		}
		else if(g_ctx->des_link == 1)
		{
			dst_vc = 3;
		}
        else if(g_ctx->des_link == 2)
		{
			dst_vc = 1;
		}
        else if(g_ctx->des_link == 3)
		{
			dst_vc = 3;
		}

		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX11_PIPE_X_EN_ADDR+0, 0x1f);
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX45_PIPE_X_DST_CTRL_ADDR+0, (g_ctx->des_link<2)?0x55:0xAA);  // 0x55=controller1  0xAA=controller2
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_TX45_PIPE_X_DST_CTRL_ADDR+1, (g_ctx->des_link<2)?0x01:0x02); 
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+0, g_ctx->streams[i] | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+1, g_ctx->streams[i] | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+2, 0x00 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+3, 0x00 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+4, 0x01 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+5, 0x01 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+6, 0x02 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+7, 0x02 | (dst_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+8, 0x03 | (src_vc << 6));
		max96712_write_reg(dev, (0x40*pipe_id)+MAX96712_PIPE_X_SRC_0_MAP_ADDR+9, 0x03 | (dst_vc << 6));			
	}

	return 0;
}

int abh_max96712_start_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	struct max96712 *priv = NULL;
	int err;
	//u32 val;
	
	CHECKNULL(dev);
	CHECKNULL(g_ctx);
	priv = dev_get_drvdata(dev);
	CHECKNULL(priv);

	dev_info(dev,"abh: 96712 start streaming, link=%c - enter\n",'A'+g_ctx->des_link);

	mutex_lock(&priv->lock);
	err = max96712_setup_pipeline(dev, g_ctx);
	if (err)
		return err;

	max96712_write_reg(dev,0xf4, 0xff);
	// max96712_read_reg(dev,0xf4,&val);
	// max96712_write_reg(dev,0xf4, (val | (1<<g_ctx->des_link))&0xff );

	// //reset mipi phy
	// if(s_priv->des_link==2 || s_priv->des_link==3){
	// 	max96712_write_reg(dev,0x8a2, 0x30);
	// }else{
	// 	max96712_write_reg(dev,0x8a2, 0xc0);
	// }
	// max96712_write_reg(dev,0x8a2, 0xf0);

	// reset link
	max96712_write_reg(dev,0x018, (1<<g_ctx->des_link));
	usleep_range(50*1000, 50*1000+1000);
	mdelay(100);
	
	//skew
	if(g_ctx->des_link==0 || g_ctx->des_link==1){
        max96712_write_reg(dev,0x0903,0x10);
		max96712_write_reg(dev,0x0903,0x33);
		max96712_write_reg(dev,0x0943,0x10);
		max96712_write_reg(dev,0x0943,0x33);
	}else{
		max96712_write_reg(dev,0x0983,0x10);
		max96712_write_reg(dev,0x0983,0x33);
        max96712_write_reg(dev,0x09c3,0x10);
		max96712_write_reg(dev,0x09c3,0x33);
	}

	mutex_unlock(&priv->lock);
	dev_info(dev,"abh: 96712 start streaming - exit\n");
	return 0;
}
EXPORT_SYMBOL(abh_max96712_start_streaming);

int abh_max96712_stop_streaming(struct device *dev, struct gmsl_link_ctx *g_ctx)
{
	// struct max96712 *priv = NULL;
	// u32 val;
	// u8 tmp;

	// CHECKNULL(dev);
	// CHECKNULL(g_ctx);
	// priv = dev_get_drvdata(dev);
	// CHECKNULL(priv);

	// dev_info(dev,"abh: 96712 stop streaming, link_%c - enter\n",'A'+g_ctx->des_link);
	// mutex_lock(&priv->lock);

	// max96712_read_reg(dev,0xf4,&val);
	// tmp = (val & (~(1<<g_ctx->des_link))) & 0xff;
	// max96712_write_reg(dev,0xf4, tmp );
	// if(tmp==0){
	// 	max96712_write_reg(dev,0x018, 0x0f);
	// 	dev_info(dev,"abh: all stopped, reset links\n");
	// 	mdelay(30);
	// }

	// mutex_unlock(&priv->lock);
	dev_info(dev,"abh: 96712 stop streaming - exit\n");
	return 0;
}
EXPORT_SYMBOL(abh_max96712_stop_streaming);

const struct of_device_id max96712_of_match[] = {
	{ .compatible ="Connor,abh_max96712", },
	{ },
};
MODULE_DEVICE_TABLE(of, max96712_of_match);

static int max96712_parse_dt(struct max96712 *priv,
				struct i2c_client *client)
{
	struct device_node *node = client->dev.of_node;
	const struct of_device_id *match;

	if (!node)
		return -EINVAL;

	match = of_match_device(max96712_of_match, &client->dev);
	if (!match) {
		dev_err(&client->dev, "Failed to find matching dt id\n");
		return -EFAULT;
	}

	return 0;
}

static struct regmap_config max96712_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
};

static int max96712_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct max96712 *priv;
	int err = 0;
	u32 val = 0;
	struct device_node *node = client->dev.of_node;

	dev_info(&client->dev, "abh: [MAX96712]: probing GMSL Deserializer @0x%x\n",client->addr);

	priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
	priv->i2c_client = client;
	priv->regmap = devm_regmap_init_i2c(priv->i2c_client,
				&max96712_regmap_config);
	if (IS_ERR(priv->regmap)) {
		dev_err(&client->dev,
			"regmap init failed: %ld\n", PTR_ERR(priv->regmap));
		return -ENODEV;
	}

	err = max96712_parse_dt(priv, client);
	if (err) {
		dev_err(&client->dev, "abh: unable to parse dt\n");
		return -EFAULT;
	}

	mutex_init(&priv->lock);

	dev_set_drvdata(&client->dev, priv);

	err=check_device_id_rev(client);
	if(err){
		dev_err(&client->dev, "abh: failed on check id & rev\n");
		return -EFAULT;
	}


	if (of_get_property(node, "is-master", NULL)) {
		priv->ismaster = 1;
		dev_info(&client->dev, "abh: this 96712 is master\n");
	}else{
		priv->ismaster = 0;
		dev_info(&client->dev, "abh: this 96712 is slave\n");
	}

	if( check_version(client)<0 ){
		return -EFAULT;
	}

	err=max96712_write_reg(&client->dev,0x0013,0x40); //reset all
	mdelay(100);
	err=max96712_write_reg(&client->dev,0x0013,0x00);

	//enable VDD LDO Regulator
    err=max96712_write_reg(&client->dev,0x0017,0x04);
	err=max96712_write_reg(&client->dev,0x0019,0x10);

	//turn on LED
	err=max96712_write_reg(&client->dev,0x0300,0x90);

	//disable Links which are not Locked
	err=max96712_read_reg(&client->dev,0x0006,&val);
	err=max96712_write_reg(&client->dev,0x0006,(val&0xF0)|(priv->link_lock_status&0x0F));


	max96712_write_reg(&client->dev,0x90a,0xc0); //4 lanes, D-Phy, 2bits VC
	max96712_write_reg(&client->dev,0x94a,0xc0); //4 lanes, D-Phy, 2bits VC

	max96712_write_reg(&client->dev,0x8a3,0xe4); //phy lane mapping
	max96712_write_reg(&client->dev,0x8a4,0xe4); //phy lane mapping

    max96712_write_reg(&client->dev,0x8a0,0x04); // 2x4 mode

	max96712_write_reg(&client->dev,0x415,MAX96712_PHY_CLK);
	max96712_write_reg(&client->dev,0x418,MAX96712_PHY_CLK);
	max96712_write_reg(&client->dev,0x41b,MAX96712_PHY_CLK);
	max96712_write_reg(&client->dev,0x41e,MAX96712_PHY_CLK);

	max96712_write_reg(&client->dev,0xf0,0x20);
	max96712_write_reg(&client->dev,0xf1,0x64); 
	max96712_write_reg(&client->dev,0xf2,0xa8);
	max96712_write_reg(&client->dev,0xf3,0xec); 
	max96712_write_reg(&client->dev,0xf4,0x00);

	max96712_write_reg(&client->dev,0x0023,0x0f); //auto reset dec error

	//mfp10, enable TX
    max96712_write_reg(&client->dev,0x0320,0x83); //TX on A
	max96712_write_reg(&client->dev,0x0357,0x2a); //TX on B
	max96712_write_reg(&client->dev,0x038d,0x2a); //TX on C
	max96712_write_reg(&client->dev,0x03c4,0x2a); //TX on D

	max96712_write_reg(&client->dev,0x0027,0x00); //disable unconcerned error report
	max96712_write_reg(&client->dev,0x0029,0xf0); //disable unconcerned error report
	max96712_write_reg(&client->dev,0x1250,0xff); //set 128 ECC threshold, reset error cnt

	/* dev communication gets validated when GMSL link setup is done */
	dev_info(&client->dev, "abh: %s:  success\n", __func__);

	return err;
}

static int check_device_id_rev(struct i2c_client *client)
{
	int err=0;
	unsigned int value=0;

	err=max96712_read_reg(&client->dev,0x004C,&value);
	dev_info(&client->dev,"abh:device revision=0x%x\n",(value&0x0f));
	return err;
}

static int check_version(struct i2c_client *client)
{
	struct max96712 *priv = NULL;
	const uint16_t ver0addr = 0x02B0; //ver0 : mfp0
	const uint16_t ver1addr = 0x02BF; //ver1 : mfp5
	const uint16_t ver2addr = 0x02C5; //ver2 : mfp7
	const uint16_t ver3addr = 0x02C8; //ver3 : mfp8

	uint16_t veraddrs[4] = {ver0addr,ver1addr,ver2addr,ver3addr};
	uint32_t ver=0,tmp;
	int i=0,err=0;

	priv = dev_get_drvdata(&client->dev);
	CHECKNULL(priv);

	if(priv->ismaster){
		//disable output
		for(i=0;i<4;i++){
			err=max96712_write_reg(&client->dev,veraddrs[i],0x85);
		}
		usleep_range(500, 1000);
		for(i=0;i<4;i++){
			err=max96712_read_reg(&client->dev,veraddrs[i],&tmp);
			tmp = (tmp & 0x8)>>3;
			ver = ver | (tmp<<i);
		}
		dev_info(&client->dev,"abh: version=%d\n",ver);
		if(ver==0){
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

static int max96712_remove(struct i2c_client *client)
{
	struct max96712 *priv;

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		mutex_destroy(&priv->lock);
		i2c_unregister_device(client);
		client = NULL;
	}

	return 0;
}

static const struct i2c_device_id max96712_id[] = {
	{ "max96712", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, max96712_id);

static struct i2c_driver max96712_i2c_driver = {
	.driver = {
		.name = "abh_max96712",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(max96712_of_match),
	},
	.probe = max96712_probe,
	.remove = max96712_remove,
	.id_table = max96712_id,
};

static int __init max96712_init(void)
{
	return i2c_add_driver(&max96712_i2c_driver);
}

static void __exit max96712_exit(void)
{
	i2c_del_driver(&max96712_i2c_driver);
}

module_init(max96712_init);
module_exit(max96712_exit);

MODULE_DESCRIPTION("GMSL Deserializer driver abh_max96712");
MODULE_AUTHOR("abh abh@163.com");
MODULE_LICENSE("GPL v2");