/*
 * utils.c
 *
 *  Created on: 2011-12-14
 *      Author: francis
 *
 *  This file was copied from workload-kit.
 *  https://github.com/giraldeau/workload-kit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "utils/utils.h"

/*
 * computes time difference (x - y)
 */
struct timespec time_sub(struct timespec *x, struct timespec *y)
{
    struct timespec res;
    res.tv_sec  = x->tv_sec  - y->tv_sec;
    res.tv_nsec = x->tv_nsec - y->tv_nsec;
    if(x->tv_nsec < y->tv_nsec) {
        res.tv_sec--;
        res.tv_nsec += 1000000000;
    }
    return res;
}

void time_add(struct timespec *x, struct timespec *y)
{
    x->tv_sec  = x->tv_sec + y->tv_sec;
    x->tv_nsec = x->tv_nsec + y->tv_nsec;
    if(x->tv_nsec >= 1000000000) {
        x->tv_sec++;
        x->tv_nsec -= 1000000000;
    }
}

void profile_func(struct profile *prof) {
    int i;
    struct timespec *data;
    /* allocate memory ahead and writes to it to avoid page fault */
    data = reinterpret_cast<struct timespec*>(malloc(prof->repeat * sizeof(struct timespec)));
    memset(data, 0, prof->repeat * sizeof(struct timespec));

    for (i = 0; i < prof->repeat; i++) {
        clock_gettime(CLOCK_MONOTONIC, &data[i]);
        prof->function();
    }

    struct timespec total = { .tv_sec = 0, .tv_nsec = 0 };
    for (i = 0; i < prof->repeat - 1; i++) {
        struct timespec delta = time_sub(&data[i + 1], &data[i]);
        time_add(&total, &delta);
    }
    prof->mean = (double)(total.tv_sec * 1000000000 + total.tv_nsec) / prof->repeat;
    printf("mean=%f\n", prof->mean);
}
