#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/atomic.h>
#include <linux/smp.h>

static int irq_line = 9;
module_param(irq_line, int, 0644);

static irqreturn_t handler(int irq, void *dev) {
    pr_info("Custom Interrupt called\n");
    return IRQ_NONE;
}

static int __init init_module_fn(void) {
    if (request_irq(irq_line, handler, IRQF_SHARED, "irq_logger_min", (void *)&handler)) {
        pr_err("irq_logger_min: request_irq %d failed.\n", irq_line);
        return -1;
    }
    pr_info("irq_logger_min: attached to IRQ %d\n", irq_line);
    return 0;
}

static void __exit exit_module_fn(void) {
    free_irq(irq_line, (void *)&handler);
}

module_init(init_module_fn);
module_exit(exit_module_fn);

MODULE_LICENSE("GPL");
