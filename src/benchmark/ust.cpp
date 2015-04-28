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

#include "benchmark/ust-tp.h"
#include "utils/time.h"

namespace
{

const size_t kNumRepetitions = 100000000;
const size_t kNumTests = 20;

const size_t kBufferSize = 20;
const void* kBuffer[kBufferSize];

void DoTestEmpty()
{
  for (size_t i = 0; i < kNumRepetitions; ++i)
  {
    asm("");
  }
}

void DoTestTracepoint(size_t size)
{
  for (size_t i = 0; i < kNumRepetitions; ++i)
  {
    asm("");
    tracepoint(lttng_profile,
               on_cpu_sample,
               size, kBuffer, 0);
  }
}

}  // namespace

int main(int argc, const char* argv[])
{
  if (argc == 2)
  {
    std::cout << "durationempty," << std::endl;
    for (size_t i = 0; i < kNumTests; ++i)
    {
      auto start = GetMonotonicTime();
      DoTestEmpty();
      auto end = GetMonotonicTime();
      std::cout << (end - start) << "," << std::endl;
    }
  }
  else
  {
    std::cout << "durationtp," << std::endl;
    for (size_t i = 0; i < kNumTests; ++i)
    {
      auto start = GetMonotonicTime();
      DoTestTracepoint(kBufferSize);
      auto end = GetMonotonicTime();
      std::cout << (end - start) << "," << std::endl;
    }
  }
}
