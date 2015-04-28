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
#include <unistd.h>

#include "utils/attributes.h"
#include "utils/time.h"
#include "utils/use_int.h"

namespace
{

size_t stackDepth = 8;

const size_t kNumActions = 100000000;
const int kNumTests = 20;

int Action()
{
  auto start = GetMonotonicTime();

  int total = 0;
  for (size_t i = 0; i < kNumActions; ++i) {
    total += syscall(39);
  }

  auto end = GetMonotonicTime();
  std::cout << (end - start) << "," << std::endl;

  return total;
}

int NOINLINE Recursive(size_t depth)
{
  if (depth == stackDepth)
    return Action();
  return Recursive(depth + 1);
}

}  // namespace

int main(int argc, const char* argv[])
{
  if (argc == 2)
    stackDepth = atoi(argv[1]);

  std::cout << "duration," << std::endl;

  for (size_t i = 0; i < kNumTests; ++i)
  {
    auto total = Recursive(0);
    USE_INT(total);
  }
}
