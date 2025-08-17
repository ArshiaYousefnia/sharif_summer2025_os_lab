#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/types.h>
#include <linux/errno.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aref Zarezadeh");
MODULE_DESCRIPTION("Print addresses of syscalls");
MODULE_VERSION("1.0");

#define MAX_SYSCALLS 512

typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
typedef const char *(*kallsyms_lookup_t)(
    unsigned long addr,
    unsigned long *symbolsize,
    unsigned long *offset,
    char **modname,
    char *namebuf
);

static kallsyms_lookup_name_t my_kallsyms_lookup_name = NULL;
static kallsyms_lookup_t my_kallsyms_lookup = NULL;

static void *get_symbol_addr_by_kprobe(const char *name)
{
    struct kprobe kp = { .symbol_name = name };
    void *addr = NULL;
    int ret = register_kprobe(&kp);
    
    if (ret == 0) {
        addr = kp.addr;
        unregister_kprobe(&kp);
    } else {
        pr_warn("kprobe register failed for %s (err %d)\n", name, ret);
        addr = NULL;
    }
    return addr;
}

static int __init sysaddr_init(void)
{
    void *tbl = NULL;
    unsigned long *syscall_table = NULL;
    int i;

    pr_info("sysaddr: init\n");

    my_kallsyms_lookup_name = (kallsyms_lookup_name_t)get_symbol_addr_by_kprobe("kallsyms_lookup_name");
    pr_info("sysaddr: kallsyms_lookup_name at %p\n", my_kallsyms_lookup_name);

    my_kallsyms_lookup = (kallsyms_lookup_t)get_symbol_addr_by_kprobe("kallsyms_lookup");
    pr_info("sysaddr: kallsyms_lookup at %p\n", my_kallsyms_lookup);
    
    tbl = (void *)my_kallsyms_lookup_name("sys_call_table");
    pr_info("sysaddr: found symbol 'sys_call_table' at %p\n", tbl);



    syscall_table = (unsigned long *)tbl;
    pr_info("sysaddr: walking syscall table at %p (printing up to %d entries)\n", syscall_table, MAX_SYSCALLS);

    for (i = 0; i < MAX_SYSCALLS; ++i) {
        unsigned long entry = syscall_table[i];
        unsigned long symsize = 0, offset = 0;
        char *modname = NULL;
        char namebuf[KSYM_NAME_LEN] = {0};
    
        if (!entry)
            continue;
    
        const char *symname = my_kallsyms_lookup(entry, &symsize, &offset, &modname, namebuf);
        if (symname && symname[0]) {
            pr_info("sysaddr: nr=%3d entry=%p  name=%s +0x%lx  module=%s\n",
                    i, (void *)entry, symname, offset, modname ? modname : "kernel");
        }
    }
    pr_info("sysaddr: done\n");
    return 0;
}

static void __exit sysaddr_exit(void)
{
    pr_info("sysaddr: exit\n");
}

module_init(sysaddr_init);
module_exit(sysaddr_exit);
