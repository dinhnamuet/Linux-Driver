#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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
	{ 0xbd341b00, "i2c_register_driver" },
	{ 0x3c12dfe, "cancel_work_sync" },
	{ 0xdc0e4855, "timer_delete" },
	{ 0x37a0cba, "kfree" },
	{ 0x26ba1c22, "i2c_smbus_read_byte_data" },
	{ 0x8fb2d6c4, "i2c_smbus_read_word_data" },
	{ 0x122c3a7e, "_printk" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0xb262895, "kmalloc_caches" },
	{ 0xdce02f9a, "kmalloc_trace" },
	{ 0xc8058d10, "i2c_smbus_write_byte_data" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0xfff5afc, "time64_to_tm" },
	{ 0x22c776bd, "i2c_smbus_write_word_data" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x24d273d1, "add_timer" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xd59c188f, "i2c_del_driver" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x3a3b9f5d, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Crtc-ds3231");
MODULE_ALIAS("of:N*T*Crtc-ds3231C*");
MODULE_ALIAS("i2c:rtc-ds3231");

MODULE_INFO(srcversion, "5350E76E6617EDCFF85FC43");
