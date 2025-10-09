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
	{ 0x8a373e6d, "max96712_write_reg_Dser" },
	{ 0x56d923d3, "of_node_put" },
	{ 0xf3324ce3, "i2c_mux_add_adapter" },
	{ 0x92997ed8, "_printk" },
	{ 0xd6f1c837, "i2c_mux_alloc" },
	{ 0x4ab03dfc, "of_get_next_child" },
	{ 0x57cf3f6c, "dev_err_probe" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x92794d67, "i2c_put_adapter" },
	{ 0x10d3d0ab, "i2c_mux_del_adapters" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "max96712");

MODULE_ALIAS("i2c:virtual-i2c-mux");
MODULE_ALIAS("of:N*T*Cnvidia,virtual-i2c-mux");
MODULE_ALIAS("of:N*T*Cnvidia,virtual-i2c-muxC*");
