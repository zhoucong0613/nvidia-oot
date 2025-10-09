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
	{ 0x1fdc7df2, "_mcount" },
	{ 0xfdc104d7, "generic_file_llseek" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0xf1da5aaa, "param_ops_int" },
	{ 0x5c4d4505, "param_ops_bool" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe449b979, "simple_attr_read" },
	{ 0x2cda321b, "tegra_get_mc_csi" },
	{ 0xacfadd27, "param_ops_charp" },
	{ 0xddb06315, "tpg_csi_media_controller_cleanup" },
	{ 0x9166fada, "strncpy" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x51b3a93a, "simple_attr_release" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x9f0a68f8, "devm_kfree" },
	{ 0x988a13, "tpg_vi_media_controller_init" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x2cc46c8f, "tpg_csi_media_controller_init" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x2d401d5d, "tpg_vi_media_controller_cleanup" },
	{ 0x4829a47e, "memcpy" },
	{ 0x5deed5ae, "tegra_get_mc_vi" },
	{ 0x77bc13a0, "strim" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0xfca056cf, "simple_attr_open" },
	{ 0x222d07a4, "simple_attr_write" },
};

MODULE_INFO(depends, "tegra-camera");

