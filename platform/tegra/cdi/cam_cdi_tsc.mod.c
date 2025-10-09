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
	{ 0xc3390a68, "platform_driver_unregister" },
	{ 0xba86023c, "__platform_driver_register" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x74b5ea68, "lcm_not_zero" },
	{ 0xfe990052, "gpio_free" },
	{ 0xf50c4bd5, "class_unregister" },
	{ 0xcd9f81af, "device_destroy" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8cb3f82a, "class_destroy" },
	{ 0x396ecf2f, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xeab3563e, "__class_create" },
	{ 0x36b3a7dd, "debugfs_create_regset32" },
	{ 0x8be7bc6b, "__register_chrdev" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x142d1479, "of_address_to_resource" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x4ab03dfc, "of_get_next_child" },
	{ 0x181f8bb6, "devm_ioremap_resource" },
	{ 0x8154d466, "platform_get_resource" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x6b4a1a7b, "of_device_get_match_data" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xd1303328, "Hawk_Owl_Fsync_program" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "nv_hawk_owl");

MODULE_ALIAS("of:N*T*Cnvidia,tegra234-cam-cdi-tsc");
MODULE_ALIAS("of:N*T*Cnvidia,tegra234-cam-cdi-tscC*");
