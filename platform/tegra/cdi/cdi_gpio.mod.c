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
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x40b346f6, "gpiochip_add_data_with_key" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xbc50c615, "gpiochip_find" },
	{ 0x4829a47e, "memcpy" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x98cf60b3, "strlen" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xd3555e57, "gpiochip_get_data" },
	{ 0x1ca356a, "gpiochip_remove" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cnvidia,cdi-gpio");
MODULE_ALIAS("of:N*T*Cnvidia,cdi-gpioC*");
