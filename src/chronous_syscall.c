#include "chronous.h"

void execute_critical_syscall(void) {
    unsigned long flags;

    spin_lock_irqsave(&chronous_lock, flags);
    /* Critical section code here */
    pr_info("Chronous: Executing synchronized syscall\n");
    spin_unlock_irqrestore(&chronous_lock, flags);
}
