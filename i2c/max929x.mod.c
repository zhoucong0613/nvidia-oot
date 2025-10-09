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
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xf9a482f9, "msleep" },
	{ 0x4474913d, "regmap_write" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x8d0bb3b5, "__devm_regmap_init_i2c" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x4829a47e, "memcpy" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*CMaxim,max929x");
MODULE_ALIAS("of:N*T*CMaxim,max929xC*");
MODULE_ALIAS("i2c:max929x");
