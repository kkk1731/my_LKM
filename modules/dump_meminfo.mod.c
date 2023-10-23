#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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
	{ 0x8688285f, "module_layout" },
	{ 0x9ba4ef56, "kernel_write" },
	{ 0xc2c2c461, "kmalloc_caches" },
	{ 0x4670df79, "param_ops_int" },
	{ 0x754d539c, "strlen" },
	{ 0x249d0ace, "filp_close" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x976b4ed7, "pv_ops" },
	{ 0xc5850110, "printk" },
	{ 0x1d19f77b, "physical_mask" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x85efc7e0, "zero_pfn" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x25f81b7e, "kmem_cache_alloc_trace" },
	{ 0xdd3ea3a7, "find_get_pid" },
	{ 0x37a0cba, "kfree" },
	{ 0xf71b53be, "get_pid_task" },
	{ 0x4b526509, "param_ops_ulong" },
	{ 0xe914e41e, "strcpy" },
	{ 0xf92a1d91, "filp_open" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C7F8EFADB1344CBFB54E638");
