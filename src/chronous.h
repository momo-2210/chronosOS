
#ifndef CHRONOUS_H
#define CHRONOUS_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>

struct chronous_stats {
    unsigned long syscall_hits;
    unsigned long sched_ticks;
    spinlock_t lock;
};

extern struct chronous_stats stats;

#endif
