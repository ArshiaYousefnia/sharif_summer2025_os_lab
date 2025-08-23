#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init init_fn(void)
{
    printk(KERN_INFO "Caller started\n");
    asm volatile ("int $0x39");
    return 0;
}

static void __exit exit_fn(void)
{
    printk(KERN_INFO "Caller is Done!\n");
}

module_init(init_fn);
module_exit(exit_fn);

MODULE_LICENSE("GPL");
