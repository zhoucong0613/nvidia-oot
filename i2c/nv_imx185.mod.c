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
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0x635589b9, "camera_common_g_ctrl" },
	{ 0x83421f02, "perf_trace_run_bpf_submit" },
	{ 0xc56a41e6, "vabits_actual" },
	{ 0x2d2c902f, "perf_trace_buf_alloc" },
	{ 0x55d18693, "regmap_util_write_table_8" },
	{ 0x7bd9e57c, "tegracam_get_privdata" },
	{ 0x9bba14c6, "tegracam_v4l2subdev_register" },
	{ 0x22e1008c, "camera_common_mclk_disable" },
	{ 0x8cf65332, "camera_common_mclk_enable" },
	{ 0x31a33147, "tegracam_set_privdata" },
	{ 0xaa061119, "tegracam_device_register" },
	{ 0x9166fada, "strncpy" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x28ea0720, "trace_event_ignore_this_pid" },
	{ 0x5c1f00f4, "event_triggers_call" },
	{ 0x84492ae0, "trace_event_buffer_commit" },
	{ 0xe914e41e, "strcpy" },
	{ 0x402ec5dd, "trace_event_buffer_reserve" },
	{ 0x98cf60b3, "strlen" },
	{ 0x9f0a68f8, "devm_kfree" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0xf87ec0ba, "of_match_device" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x5be113a6, "regmap_read" },
	{ 0x2396c7f0, "clk_set_parent" },
	{ 0x605b25f, "devm_clk_get" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x4474913d, "regmap_write" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x2952e80a, "tegracam_device_unregister" },
	{ 0x8e09dd0b, "tegracam_v4l2subdev_unregister" },
	{ 0x3886add4, "sensor_common_parse_num_modes" },
	{ 0xcfb89098, "bpf_trace_run3" },
	{ 0x7381287f, "trace_handle_return" },
	{ 0x65990cd2, "trace_event_printf" },
	{ 0xc3412757, "trace_raw_output_prep" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "tegra-camera");

MODULE_ALIAS("i2c:imx185");
MODULE_ALIAS("of:N*T*Csony,imx185");
MODULE_ALIAS("of:N*T*Csony,imx185C*");
