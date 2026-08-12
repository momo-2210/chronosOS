#ifndef CHRONOUS_H
#define CHRONOUS_H
#include <linux/spinlock.h>

struct chronous_stats {
    unsigned long syscall_hits;   // Member 2 update korbe
    unsigned long sched_ticks;    // Member 3 update korbe
    spinlock_t lock;              // Member 4 add korbe
};

extern struct chronous_stats stats;

#endif
