/* Copyright (c) 2015 Francois Doray <francois.pierre-doray@polymtl.ca>
 *
 * This file is part of tibeecompare.
 *
 * tibeecompare is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tibeecompare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tibeecompare.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UTILS_TIME_H_
#define UTILS_TIME_H_

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static __inline__
uint64_t GetMonotonicTime()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((uint64_t) ts.tv_sec * 1000000000ULL) + ts.tv_nsec;
}

class TicToc
{
public:
    TicToc() : _start(GetMonotonicTime())
    {
    }

    ~TicToc()
    {
        uint64_t duration = GetMonotonicTime() - _start;
        printf("Duration: %lu\n", duration);
    }

private:
    uint64_t _start;
};

#endif  // UTILS_TIME_H_
