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
	{ 0x357f82cc, "camera_common_s_power" },
	{ 0x94eeb924, "camera_common_enum_frameintervals" },
	{ 0x456515a6, "camera_common_enum_framesizes" },
	{ 0x43603ae2, "camera_common_enum_mbus_code" },
	{ 0xf16db404, "v4l2_subdev_link_validate" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x150b8c05, "v4l2_async_register_subdev" },
	{ 0x3549462f, "v4l2_ctrl_handler_init_class" },
	{ 0x9bc34fb, "tegra_media_entity_init" },
	{ 0xafeaf093, "v4l2_i2c_subdev_init" },
	{ 0xc0693e11, "camera_common_initialize" },
	{ 0x451e457e, "camera_common_find_datafmt" },
	{ 0x37a0cba, "kfree" },
	{ 0xd58d4b67, "kmem_cache_alloc_trace" },
	{ 0xc1c7e6c0, "kmalloc_caches" },
	{ 0x8c4ff6e1, "regulator_disable" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0xad130843, "regulator_enable" },
	{ 0x5fe1c050, "camera_common_regulator_get" },
	{ 0x2396c7f0, "clk_set_parent" },
	{ 0x605b25f, "devm_clk_get" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0xf87ec0ba, "of_match_device" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x760a0f4f, "yield" },
	{ 0x497d877c, "camera_common_s_fmt" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xd9f08345, "camera_common_try_fmt" },
	{ 0xf9a482f9, "msleep" },
	{ 0x9bf353b9, "v4l2_ctrl_new_custom" },
	{ 0xdd64e639, "strscpy" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x9166fada, "strncpy" },
	{ 0xa3138585, "v4l2_ctrl_new_std_menu" },
	{ 0x92997ed8, "_printk" },
	{ 0xb21558d2, "v4l2_ctrl_new_std" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xdcb764ad, "memset" },
	{ 0x9f0a68f8, "devm_kfree" },
	{ 0x57143020, "camera_common_remove_debugfs" },
	{ 0xccca63b, "cam_gpio_deregister" },
	{ 0xd3fb483f, "v4l2_ctrl_handler_free" },
	{ 0x5fa58559, "v4l2_async_unregister_subdev" },
	{ 0xfe990052, "gpio_free" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x3886add4, "sensor_common_parse_num_modes" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x3e7ab224, "gpiod_set_raw_value_cansleep" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0x32f151aa, "gpiod_cansleep" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0xcf74e305, "camera_common_g_fmt" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xeed0b690, "i2c_transfer" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "tegra-camera,videodev,v4l2-async");

MODULE_ALIAS("i2c:ar1335");
MODULE_ALIAS("of:N*T*Cnvidia,ar1335");
MODULE_ALIAS("of:N*T*Cnvidia,ar1335C*");
