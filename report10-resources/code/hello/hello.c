#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Hello Driver for OS lab report 10: drivers, Arshia Yousefnia");

static int __init hello_init(void) {
    printk(KERN_INFO "Hello from hello driver\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye from hello driver\n");
}

module_init(hello_init);
module_exit(hello_exit);