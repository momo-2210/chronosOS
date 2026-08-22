#include <linux/module.h>
#include <linux/kernel.h>
#include "chronous.h"

DEFINE_SPINLOCK(chronous_lock);

static int __init chronous_init(void) {
    pr_info("Chronous: Initialized with Spinlocks.\n");
    return 0;
}

static void __exit chronous_cleanup(void) {
    pr_info("Chronous: Unloaded.\n");
}

module_init(chronous_init);
module_exit(chronous_cleanup);

MODULE_LICENSE("GPL");
