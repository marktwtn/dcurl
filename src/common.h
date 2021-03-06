/*
 * Copyright (C) 2018-2019 BiiLabs Co., Ltd. and Contributors
 * All Rights Reserved.
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the MIT license. A copy of the license can be found in the file
 * "LICENSE" at the root of this distribution.
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define __DCURL_MAJOR__ 0
#define __DCURL_MINOR__ 4
#define __DCURL_PATCH__ 1

double diff_in_second(struct timespec t1, struct timespec t2);

static inline void ddprintf(const char *format, ...)
{
#if defined(ENABLE_DEBUG)
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    fflush(stdout);
#endif
}

typedef struct _pow_info PoW_Info;

struct _pow_info {
    double time;
    uint64_t hash_count;
};

#endif
