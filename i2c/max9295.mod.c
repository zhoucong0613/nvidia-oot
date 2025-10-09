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
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0x337df174, "of_get_property" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x4474913d, "regmap_write" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmaxim,max9295");
MODULE_ALIAS("of:N*T*Cmaxim,max9295C*");
MODULE_ALIAS("i2c:max9295");
