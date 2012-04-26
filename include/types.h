#pragma once
#include "arch.h"

#define NULL ((void *)0)

typedef unsigned long size_t;
typedef unsigned long ptrdiff_t;


#if WORD_SIZE == 4
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif

#ifdef ARCH_ARMV7A
typedef uint32_t uint_fast32_t;
typedef uint32_t register_t;
#endif
