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
	{ 0xf9a482f9, "msleep" },
	{ 0x1fdc7df2, "_mcount" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0xff555827, "single_open" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x428908f, "single_release" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x5be113a6, "regmap_read" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0xcf851688, "seq_read" },
	{ 0xdcb764ad, "memset" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0xe7bec0c9, "seq_lseek" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x4474913d, "regmap_write" },
	{ 0x88db9f48, "__check_object_size" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("i2c:max96712");
