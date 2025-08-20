#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define LOG_FILE "/var/log/net_traffic.log"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Network Traffic Logger using Netfilter for OS lab report 10: drivers, Arshia Yousefnia");

static struct nf_hook_ops nfho;

static unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *iph;
    struct file *filp;
    loff_t pos = 0;
    char buf[128];

    if (!skb || !skb->network_header) return NF_ACCEPT;

    iph = ip_hdr(skb);
    if (iph) {
        snprintf(buf, sizeof(buf), "Packet: SRC=%pI4 DST=%pI4 PROTO=%u\n", &iph->saddr, &iph->daddr, iph->protocol);

        filp = filp_open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (!IS_ERR(filp)) {
            kernel_write(filp, buf, strlen(buf), &pos);
            filp_close(filp, NULL);
        }
    }
    return NF_ACCEPT;
}

static int __init net_init(void) {
    nfho.hook = hook_func;
    nfho.hooknum = NF_INET_PRE_ROUTING;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Arshia loaded NetLogger module\n");
    return 0;
}

static void __exit net_exit(void) {
    nf_unregister_net_hook(&init_net, &nfho);
    printk(KERN_INFO "Arshia unloaded NetLogger module\n");
}

module_init(net_init);
module_exit(net_exit);
