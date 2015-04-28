// Copyright (c) 2014 Francois Doray <francois.pierre-doray@polymtl.ca>
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
 
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER tracekit

#undef TRACEPOINT_INCLUDE_FILE
#define TRACEPOINT_INCLUDE_FILE utils/tp.h

#if !defined(_UTILS_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _UTILS_TP_H

#include <lttng/tracepoint.h>
#include <pthread.h>

TRACEPOINT_EVENT(
    tracekit,
    begin,
    TP_ARGS(),
    TP_FIELDS()
)

TRACEPOINT_EVENT(
    tracekit,
    end,
    TP_ARGS(),
    TP_FIELDS()
)

TRACEPOINT_EVENT(
    tracekit,
    value,
    TP_ARGS(int, value),
    TP_FIELDS(
        ctf_integer(int, value, value))
)

#endif /* _UTILS_TP_H */
#include <lttng/tracepoint-event.h>
