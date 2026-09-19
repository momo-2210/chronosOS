#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include "chronos.h"

struct io_request {
    struct list_head list;
    pid_t pid;
    char op;   // 'R' = read, 'W' = write
};

static LIST_HEAD(io_queue);
static DEFINE_MUTEX(io_queue_lock);
static struct task_struct *io_worker;
static int queue_len;

int chronos_blockio_submit(char op)
{
    struct io_request *req = kmalloc(sizeof(*req), GFP_KERNEL);
    if (!req)
        return -ENOMEM;

    req->pid = current->pid;
    req->op = op;

    mutex_lock(&io_queue_lock);
    list_add_tail(&req->list, &io_queue);
    queue_len++;
    mutex_unlock(&io_queue_lock);

    return 0;
}

int chronos_blockio_queue_length(void)
{
    int len;
    mutex_lock(&io_queue_lock);
    len = queue_len;
    mutex_unlock(&io_queue_lock);
    return len;
}

static int blockio_worker_fn(void *data)
{
    struct io_request *req;

    while (!kthread_should_stop()) {
        req = NULL;

        mutex_lock(&io_queue_lock);
        if (!list_empty(&io_queue)) {
            req = list_first_entry(&io_queue, struct io_request, list);
            list_del(&req->list);
            queue_len--;
        }
        mutex_unlock(&io_queue_lock);

        if (req) {
            msleep(50);   // real disk access simulate korche

            spin_lock(&chronos_lock);
            stats.block_io_requests++;
            spin_unlock(&chronos_lock);

            pr_info("Chronos: serviced %s request from pid %d\n",
                    req->op == 'W' ? "WRITE" : "READ", req->pid);
            kfree(req);
        } else {
            msleep(100);
        }
    }
    return 0;
}

int chronos_blockio_init(void)
{
    io_worker = kthread_run(blockio_worker_fn, NULL, "chronos_blockio");
    return IS_ERR(io_worker) ? PTR_ERR(io_worker) : 0;
}

void chronos_blockio_exit(void)
{
    struct io_request *req, *tmp;

    kthread_stop(io_worker);

    mutex_lock(&io_queue_lock);
    list_for_each_entry_safe(req, tmp, &io_queue, list) {
        list_del(&req->list);
        kfree(req);
    }
    mutex_unlock(&io_queue_lock);
}
