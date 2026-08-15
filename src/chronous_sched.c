#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include "chronous.h"

static struct task_struct *sched_thread;

// eta ekta infinite loop, background e running thakbe
static int sched_monitor_fn(void *data) {
    while (!kthread_should_stop()) {
        stats.sched_ticks++;

        msleep(1000); // 1 second wait
    }
    return 0;
}

int chronous_sched_init(void) {
    sched_thread = kthread_run(sched_monitor_fn, NULL, "chronous_sched");
    return IS_ERR(sched_thread) ? PTR_ERR(sched_thread) : 0;
}

void chronous_sched_exit(void) {
    kthread_stop(sched_thread);
}
