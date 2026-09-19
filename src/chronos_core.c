#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include "chronos.h"

struct chronos_stats stats;
DEFINE_SPINLOCK(chronos_lock);

extern int chronos_syscall_init(void);
extern void chronos_syscall_exit(void);
extern int chronos_sched_init(void);
extern void chronos_sched_exit(void);

static int chronos_show(struct seq_file *m, void *v) {
    seq_printf(m, "syscall_hits: %lu\n", stats.syscall_hits);
    seq_printf(m, "sched_ticks: %lu\n", stats.sched_ticks);
    seq_printf(m, "block_io_requests: %lu\n", stats.block_io_requests);
    seq_printf(m, "block_io_queue_length: %d\n", chronos_blockio_queue_length());
    return 0;
}

static int chronos_open(struct inode *inode, struct file *file) {
    return single_open(file, chronos_show, NULL);
}

static ssize_t chronos_write(struct file *file, const char __user *buf,
                              size_t count, loff_t *ppos) {
    char kbuf[8];
    size_t len = min(count, sizeof(kbuf) - 1);

    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    if (kbuf[0] == 'W' || kbuf[0] == 'w')
        chronos_blockio_submit('W');
    else
        chronos_blockio_submit('R');

    return count;
}

static const struct proc_ops chronos_fops = {
    .proc_open    = chronos_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
    .proc_write   = chronos_write,
};

static int __init chronos_init(void) {
    proc_create("chronos", 0666, NULL, &chronos_fops);
    chronos_syscall_init();
    chronos_sched_init();
    chronos_blockio_init();
    pr_info("Chronos: Initialized with Spinlocks.\n");
    return 0;
}

static void __exit chronos_cleanup(void) {
    chronos_syscall_exit();
    chronos_sched_exit();
    chronos_blockio_exit();
    remove_proc_entry("chronos", NULL);
    pr_info("Chronos: Unloaded.\n");
}

module_init(chronos_init);
module_exit(chronos_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chronos Team");
MODULE_DESCRIPTION("Kernel monitoring module with block I/O queue");
