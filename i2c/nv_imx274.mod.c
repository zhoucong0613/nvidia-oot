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
	{ 0xf1da5aaa, "param_ops_int" },
	{ 0x51b3a93a, "simple_attr_release" },
	{ 0x222d07a4, "simple_attr_write" },
	{ 0xe449b979, "simple_attr_read" },
	{ 0xfdc104d7, "generic_file_llseek" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x635589b9, "camera_common_g_ctrl" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x9bba14c6, "tegracam_v4l2subdev_register" },
	{ 0x22e1008c, "camera_common_mclk_disable" },
	{ 0x598423aa, "regmap_bulk_read" },
	{ 0x8cf65332, "camera_common_mclk_enable" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0xbc226d62, "i2c_new_client_device" },
	{ 0xdd64e639, "strscpy" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0xdcb764ad, "memset" },
	{ 0x6ac93af8, "i2c_get_adapter" },
	{ 0x31a33147, "tegracam_set_privdata" },
	{ 0xaa061119, "tegracam_device_register" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x9166fada, "strncpy" },
	{ 0xad130843, "regulator_enable" },
	{ 0x7bd9e57c, "tegracam_get_privdata" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x8c4ff6e1, "regulator_disable" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x5fe1c050, "camera_common_regulator_get" },
	{ 0x2396c7f0, "clk_set_parent" },
	{ 0x605b25f, "devm_clk_get" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x2952e80a, "tegracam_device_unregister" },
	{ 0x8e09dd0b, "tegracam_v4l2subdev_unregister" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x3886add4, "sensor_common_parse_num_modes" },
	{ 0xfca056cf, "simple_attr_open" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0xf87ec0ba, "of_match_device" },
	{ 0xfe990052, "gpio_free" },
	{ 0xad8c9ca, "devm_regulator_put" },
	{ 0x55d18693, "regmap_util_write_table_8" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x5be113a6, "regmap_read" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x4474913d, "regmap_write" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "tegra-camera");

MODULE_ALIAS("i2c:imx274");
MODULE_ALIAS("of:N*T*Csony,imx274");
MODULE_ALIAS("of:N*T*Csony,imx274C*");
