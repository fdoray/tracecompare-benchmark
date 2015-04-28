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
#include "utils/scoped_tp.h"

#include "utils/tp.h"

namespace
{

// Maximum stack size.
const size_t kMaxStackSize = 50;

}  // namespace

ScopedTp::ScopedTp()
{
    // Emit a begin event.
    tracepoint(tracekit, begin);
}

ScopedTp::~ScopedTp()
{
    // Emit an end event.
    tracepoint(tracekit, end);
}