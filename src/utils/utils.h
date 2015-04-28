/*
 * utils.h
 *
 *  Created on: 2011-12-14
 *    Author: francis
 *
 *  This file was copied from workload-kit.
 *  https://github.com/giraldeau/workload-kit
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <functional>

struct profile {
    std::function<void()> function;
    int repeat;
    float mean;
};

int profile(struct profile *prof);
struct timespec time_sub(struct timespec *x, struct timespec *y);

#endif /* UTILS_UTILS_H_ */
