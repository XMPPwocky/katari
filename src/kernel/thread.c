#include "kernel/kernel.h"
#include "kernel/thread.h"

struct Thread kernel_thread;
struct Thread *curr_thread = &kernel_thread;
