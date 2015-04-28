// Copyright (c) 2015 Francois Doray <francois.pierre-doray@polymtl.ca>
//
// This file is part of trace-kit.
//
// trace-kit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// trace-kit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with trace-kit.  If not, see <http://www.gnu.org/licenses/>.
#include <iostream>
#include <stdlib.h>

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include "utils/attributes.h"
#include "utils/time.h"
#include "utils/use_int.h"

namespace {

size_t stackDepth = 0;
const size_t kNumBacktraces = 10000000;
const int kNumTests = 20;
const size_t kBufferSize = 60;

int NOINLINE Recursive(size_t depth)
{
  if (depth == stackDepth)
  {
    auto start = GetMonotonicTime();

    size_t total = 0;
    void* buffer[kBufferSize];
    for (size_t i = 0; i < kNumBacktraces; ++i) {
      asm("");
#ifndef BACKTRACE_EMPTY
      total = unw_backtrace (buffer, kBufferSize);
#endif
    }

    auto end = GetMonotonicTime();
    auto total_time = end - start;
    auto step_time = total_time / kNumBacktraces;
    std::cout << step_time << "," << total << std::endl;

    return total;
  }
  return Recursive(depth + 1);
}

}  // namespace

int main(int argc, const char* argv[])
{
  if (argc >= 2 && std::string(argv[1]) == "nocache") {
    unw_set_caching_policy(unw_local_addr_space, UNW_CACHE_NONE);
    std::cout << "nocache" << std::endl;
  }
  else {
    unw_set_caching_policy(unw_local_addr_space, UNW_CACHE_PER_THREAD);
    std::cout << "threadcache" << std::endl;
  }

  std::cout << "duration,size" << std::endl;

#ifndef BACKTRACE_EMPTY
  for (size_t i = 0; i < kNumTests; ++i)
  {
    // int j = 10;
    for (size_t j = 0; j < 25; j += 2)
    {
      stackDepth = j;
      auto total = Recursive(0);
      USE_INT(total);
    }
  }
#else
  for (size_t i = 0; i < kNumTests; ++i)
  {
    stackDepth = 10;
    auto total = Recursive(0);
    USE_INT(total);
  }
#endif
}
