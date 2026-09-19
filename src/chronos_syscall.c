#include <linux/kprobes.h>
#include <linux/module.h>
#include "chronos.h"

static struct kprobe kp = {
    .symbol_name = "__x64_sys_openat",
};

// target function call hobar AGE ei handler automatically run hobe
static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    spin_lock(&chronos_lock);
    stats.syscall_hits++;
    spin_unlock(&chronos_lock);
    return 0;
}

int chronos_syscall_init(void) {
    kp.pre_handler = handler_pre;
    return register_kprobe(&kp);
}

void chronos_syscall_exit(void) {
    unregister_kprobe(&kp);
}
