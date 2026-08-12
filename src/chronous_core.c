#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include "chronous.h"

struct chronous_stats stats;

static int chronous_show(struct seq_file *m, void *v) {
    seq_printf(m, "syscall_hits: %lu\n", stats.syscall_hits);
    seq_printf(m, "sched_ticks: %lu\n", stats.sched_ticks);
    return 0;
}
static int chronous_open(struct inode *inode, struct file *file) {
    return single_open(file, chronous_show, NULL);
}
static const struct proc_ops chronous_fops = {
    .proc_open = chronous_open,
    .proc_read = seq_read,
    .proc_release = single_release,
};

static int __init chronous_init(void) {
    spin_lock_init(&stats.lock);
    proc_create("chronous", 0444, NULL, &chronous_fops);
    printk(KERN_INFO "Chronous OS: module loaded\n");
    return 0;
}
static void __exit chronous_exit(void) {
    remove_proc_entry("chronous", NULL);
    printk(KERN_INFO "Chronous OS: module unloaded\n");
}
module_init(chronous_init);
module_exit(chronous_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Chronous OS - kernel monitoring module");
