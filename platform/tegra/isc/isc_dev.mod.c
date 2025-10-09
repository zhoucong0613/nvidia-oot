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
	{ 0x51b3a93a, "simple_attr_release" },
	{ 0x222d07a4, "simple_attr_write" },
	{ 0xe449b979, "simple_attr_read" },
	{ 0xfdc104d7, "generic_file_llseek" },
	{ 0xcc921354, "i2c_del_driver" },
	{ 0x7b1cfd9e, "i2c_register_driver" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x37a0cba, "kfree" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xeed0b690, "i2c_transfer" },
	{ 0x92997ed8, "_printk" },
	{ 0x1fc87565, "_dev_notice" },
	{ 0x9f0a68f8, "devm_kfree" },
	{ 0x396ecf2f, "device_create" },
	{ 0xd5a0871d, "cdev_add" },
	{ 0x5d7a3971, "cdev_init" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x89c9a638, "_dev_err" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0xdcb764ad, "memset" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0x277c232f, "cdev_del" },
	{ 0xcd9f81af, "device_destroy" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xfca056cf, "simple_attr_open" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");

