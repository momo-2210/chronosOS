#include <linux/kprobes.h>
#include <linux/module.h>
#include "chronous.h"

static struct kprobe kp = {
    .symbol_name = "__x64_sys_openat",
};

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    stats.syscall_hits++;
    return 0;
}

int chronous_syscall_init(void) {
    kp.pre_handler = handler_pre;
    return register_kprobe(&kp);
}
void chronous_syscall_exit(void) {
    unregister_kprobe(&kp);
}
