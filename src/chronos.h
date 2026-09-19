#ifndef CHRONOS_H
#define CHRONOS_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>

struct chronos_stats {
    unsigned long syscall_hits;
    unsigned long sched_ticks;
    unsigned long block_io_requests;   // NEW
    spinlock_t lock;
};

extern struct chronos_stats stats;
extern spinlock_t chronos_lock;

// Block I/O queue manager (new)
int chronos_blockio_submit(char op);
int chronos_blockio_queue_length(void);
int chronos_blockio_init(void);
void chronos_blockio_exit(void);

  int chronos_syscall_init(void);
  void chronos_syscall_exit(void);
  int chronos_sched_init(void);
  void chronos_sched_exit(void);

#endif
