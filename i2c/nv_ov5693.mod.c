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
	{ 0xeb07893f, "trace_event_raw_init" },
	{ 0xc77cd0d1, "trace_event_reg" },
	{ 0x51b3a93a, "simple_attr_release" },
	{ 0x222d07a4, "simple_attr_write" },
	{ 0xe449b979, "simple_attr_read" },
	{ 0xfdc104d7, "generic_file_llseek" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x635589b9, "camera_common_g_ctrl" },
	{ 0x6aad71a8, "camera_common_i2c_aggregate" },
	{ 0x7bd9e57c, "tegracam_get_privdata" },
	{ 0x83421f02, "perf_trace_run_bpf_submit" },
	{ 0xc56a41e6, "vabits_actual" },
	{ 0x2d2c902f, "perf_trace_buf_alloc" },
	{ 0x8e09dd0b, "tegracam_v4l2subdev_unregister" },
	{ 0x3886add4, "sensor_common_parse_num_modes" },
	{ 0xb45a4375, "cam_gpio_register" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x5fe1c050, "camera_common_regulator_get" },
	{ 0x2396c7f0, "clk_set_parent" },
	{ 0x605b25f, "devm_clk_get" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x9bba14c6, "tegracam_v4l2subdev_register" },
	{ 0x2952e80a, "tegracam_device_unregister" },
	{ 0x22e1008c, "camera_common_mclk_disable" },
	{ 0x8cf65332, "camera_common_mclk_enable" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0xbc226d62, "i2c_new_client_device" },
	{ 0xdd64e639, "strscpy" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa916b694, "strnlen" },
	{ 0xdcb764ad, "memset" },
	{ 0x6ac93af8, "i2c_get_adapter" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x31a33147, "tegracam_set_privdata" },
	{ 0xaa061119, "tegracam_device_register" },
	{ 0x9166fada, "strncpy" },
	{ 0x8c4ff6e1, "regulator_disable" },
	{ 0xad130843, "regulator_enable" },
	{ 0x171cd15f, "cam_gpio_ctrl" },
	{ 0x3e7ab224, "gpiod_set_raw_value_cansleep" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x32f151aa, "gpiod_cansleep" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x28ea0720, "trace_event_ignore_this_pid" },
	{ 0x5c1f00f4, "event_triggers_call" },
	{ 0x84492ae0, "trace_event_buffer_commit" },
	{ 0xe914e41e, "strcpy" },
	{ 0x402ec5dd, "trace_event_buffer_reserve" },
	{ 0x98cf60b3, "strlen" },
	{ 0x598423aa, "regmap_bulk_read" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0xfca056cf, "simple_attr_open" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0xab5822d0, "camera_common_parse_clocks" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0xf87ec0ba, "of_match_device" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x5be113a6, "regmap_read" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x55d18693, "regmap_util_write_table_8" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x4474913d, "regmap_write" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xccca63b, "cam_gpio_deregister" },
	{ 0xfe990052, "gpio_free" },
	{ 0xcfb89098, "bpf_trace_run3" },
	{ 0x7381287f, "trace_handle_return" },
	{ 0x65990cd2, "trace_event_printf" },
	{ 0xc3412757, "trace_raw_output_prep" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "tegra-camera");

MODULE_ALIAS("i2c:ov5693");
MODULE_ALIAS("of:N*T*Covti,ov5693");
MODULE_ALIAS("of:N*T*Covti,ov5693C*");
