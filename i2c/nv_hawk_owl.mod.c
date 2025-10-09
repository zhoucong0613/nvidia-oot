#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x25f8bfc1, "module_layout" },
	{ 0xaa061119, "tegracam_device_register" },
	{ 0x31a33147, "tegracam_set_privdata" },
	{ 0xc1c7e6c0, "kmalloc_caches" },
	{ 0x22e1008c, "camera_common_mclk_disable" },
	{ 0xf9a482f9, "msleep" },
	{ 0x1fdc7df2, "_mcount" },
	{ 0x8cf65332, "camera_common_mclk_enable" },
	{ 0x97d868c2, "_dev_printk" },
	{ 0xf1da5aaa, "param_ops_int" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x598423aa, "regmap_bulk_read" },
	{ 0x7bd9e57c, "tegracam_get_privdata" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x3886add4, "sensor_common_parse_num_modes" },
	{ 0x2952e80a, "tegracam_device_unregister" },
	{ 0xd85c5e47, "_dev_warn" },
	{ 0xdcb764ad, "memset" },
	{ 0x9bba14c6, "tegracam_v4l2subdev_register" },
	{ 0xbc226d62, "i2c_new_client_device" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x9166fada, "strncpy" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x89c9a638, "_dev_err" },
	{ 0xf87ec0ba, "of_match_device" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0x2396c7f0, "clk_set_parent" },
	{ 0x92997ed8, "_printk" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x8a373e6d, "max96712_write_reg_Dser" },
	{ 0x605b25f, "devm_clk_get" },
	{ 0xd58d4b67, "kmem_cache_alloc_trace" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0xf500be5d, "max96712_read_reg_Dser" },
	{ 0x37a0cba, "kfree" },
	{ 0x4829a47e, "memcpy" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x8e09dd0b, "tegracam_v4l2subdev_unregister" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x6ac93af8, "i2c_get_adapter" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x4474913d, "regmap_write" },
};

MODULE_INFO(depends, "tegra-camera,max96712");

MODULE_ALIAS("of:N*T*Cnvidia,ar0234_hawk_owl");
MODULE_ALIAS("of:N*T*Cnvidia,ar0234_hawk_owlC*");
MODULE_ALIAS("i2c:ar0234");
