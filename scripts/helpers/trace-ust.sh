#!/bin/bash

set -e

lttng create benchmark-ust-$1 $2

lttng enable-channel k -k --num-subbuf 4096 --subbuf-size 16384
lttng enable-event -c k -k syscall_latency
lttng add-context -c k -k -t vtid
lttng add-context -c k -k -t vpid

lttng enable-channel u -u --num-subbuf 4096 --subbuf-size 16384
lttng enable-event -c u -u -a
lttng add-context -c u -u -t vtid
lttng add-context -c u -u -t vpid
lttng start
