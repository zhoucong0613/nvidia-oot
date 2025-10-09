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
	{ 0x2d3385d3, "system_wq" },
	{ 0xc1c7e6c0, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x1fdc7df2, "_mcount" },
	{ 0xfe4a5955, "genl_register_family" },
	{ 0x98cf60b3, "strlen" },
	{ 0x9d68ca9b, "genl_unregister_family" },
	{ 0x84823cf3, "nla_strscpy" },
	{ 0x356988dd, "format_tvcf_version" },
	{ 0x3c12dfe, "cancel_work_sync" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe45e9ca3, "camtest_log" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0x6c6dca97, "nla_put" },
	{ 0xf83251ef, "kfree_skb_reason" },
	{ 0x8c8edba4, "netlink_unicast" },
	{ 0x131fa714, "camtest_try_acquire_global_log" },
	{ 0x3edd21e7, "init_net" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x1e1fc5ad, "__alloc_skb" },
	{ 0x5009c71d, "glob_match" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x4ab03dfc, "of_get_next_child" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x92997ed8, "_printk" },
	{ 0xddcd86c6, "of_find_compatible_node" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0xccd8152b, "tegracam_query_version" },
	{ 0xd58d4b67, "kmem_cache_alloc_trace" },
	{ 0x148653, "vsnprintf" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfd7ca875, "of_find_node_by_name" },
	{ 0x37a0cba, "kfree" },
	{ 0x71e078b2, "camtest_release_global_log" },
	{ 0x1b1fa641, "genlmsg_put" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x5e281df5, "tegracam_version" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x56d923d3, "of_node_put" },
	{ 0x14b89635, "arm64_const_caps_ready" },
};

MODULE_INFO(depends, "tegra-camera,tegracam_log");

