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
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x277c232f, "cdev_del" },
	{ 0xc1c7e6c0, "kmalloc_caches" },
	{ 0x5d7a3971, "cdev_init" },
	{ 0x1fdc7df2, "_mcount" },
	{ 0xfdc104d7, "generic_file_llseek" },
	{ 0x6d41f8b3, "devm_pwm_get" },
	{ 0x94c27ac9, "debugfs_create_dir" },
	{ 0xff555827, "single_open" },
	{ 0xac8a73e3, "of_root" },
	{ 0xe30fbb73, "of_property_read_u32_index" },
	{ 0xef34bf3e, "hrtimer_active" },
	{ 0x428908f, "single_release" },
	{ 0xdda9e539, "pwm_apply_state" },
	{ 0xeed0b690, "i2c_transfer" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x2af6dc75, "gpio_to_desc" },
	{ 0x4b0cd0e6, "seq_printf" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xcd9f81af, "device_destroy" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x92794d67, "i2c_put_adapter" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xe1bf0af5, "debugfs_create_file" },
	{ 0xcf851688, "seq_read" },
	{ 0xba86023c, "__platform_driver_register" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe449b979, "simple_attr_read" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x3c5d543a, "hrtimer_start_range_ns" },
	{ 0xd85c5e47, "_dev_warn" },
	{ 0xdcb764ad, "memset" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0xbc226d62, "i2c_new_client_device" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xf35f1bbd, "devm_fwnode_gpiod_get_index" },
	{ 0xa12a4ea3, "of_find_property" },
	{ 0x5fd15d16, "of_property_read_string" },
	{ 0xbae8826c, "of_property_count_elems_of_size" },
	{ 0x9166fada, "strncpy" },
	{ 0x43cfdd90, "debugfs_remove" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xb4e9097d, "gpiod_direction_input" },
	{ 0x396ecf2f, "device_create" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0x58e475d1, "pid_task" },
	{ 0x65f296bc, "gpiod_direction_output_raw" },
	{ 0x51b3a93a, "simple_attr_release" },
	{ 0xa2476b84, "i2c_unregister_device" },
	{ 0x89c9a638, "_dev_err" },
	{ 0x9f0a68f8, "devm_kfree" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x6323d63f, "of_get_child_by_name" },
	{ 0xd5a0871d, "cdev_add" },
	{ 0x85fd2dbe, "_dev_info" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x4ab03dfc, "of_get_next_child" },
	{ 0x1000e51, "schedule" },
	{ 0x92997ed8, "_printk" },
	{ 0x908e5601, "cpu_hwcaps" },
	{ 0xdf32c959, "of_get_named_gpio_flags" },
	{ 0x69f38847, "cpu_hwcap_keys" },
	{ 0x7f24de73, "jiffies_to_usecs" },
	{ 0x337df174, "of_get_property" },
	{ 0x6a805dbd, "devm_gpiod_put" },
	{ 0xd58d4b67, "kmem_cache_alloc_trace" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x444af313, "init_pid_ns" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe990052, "gpio_free" },
	{ 0xe7bec0c9, "seq_lseek" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x54818afc, "of_get_parent" },
	{ 0x37a0cba, "kfree" },
	{ 0xae5c7863, "gpiod_to_irq" },
	{ 0x2ab815b1, "send_sig_info" },
	{ 0x8b7f7aab, "gpiod_set_raw_value" },
	{ 0x2d0684a9, "hrtimer_init" },
	{ 0xcf2a6966, "up" },
	{ 0x8cb3f82a, "class_destroy" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xe1194d5, "hrtimer_try_to_cancel" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xa51893d0, "gpiod_count" },
	{ 0xed811a3c, "of_get_compatible_child" },
	{ 0x270beffc, "gpiod_get_raw_value" },
	{ 0xc3390a68, "platform_driver_unregister" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0x79c597d7, "of_property_read_variable_u32_array" },
	{ 0x6ac93af8, "i2c_get_adapter" },
	{ 0x4e5b36a5, "devm_kmalloc" },
	{ 0x8abf92ad, "devm_request_threaded_irq" },
	{ 0x14b89635, "arm64_const_caps_ready" },
	{ 0xeab3563e, "__class_create" },
	{ 0x3cd60f09, "find_pid_ns" },
	{ 0xfca056cf, "simple_attr_open" },
	{ 0x92e683f5, "down_timeout" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x222d07a4, "simple_attr_write" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cnvidia,cdi-mgr");
MODULE_ALIAS("of:N*T*Cnvidia,cdi-mgrC*");
