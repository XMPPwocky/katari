#pragma once

#include "kernel/kernel.h"

typedef uint32_t semaphore_t;

/* All these are non-blocking and return true on success. */
bool semaphore_V(semaphore_t sem);
bool semaphore_P(semaphore_t sem);
