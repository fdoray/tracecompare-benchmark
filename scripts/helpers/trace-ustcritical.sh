#!/bin/bash

set -e

lttng create benchmark-ustcritical-$1 $2

lttng enable-channel k -k --num-subbuf 4096 --subbuf-size 16384
lttng enable-event -c k -k syscall_latency
lttng enable-event -c k -k sched_ttwu
lttng enable-event -c k -k sched_switch
lttng enable-event -c k -k sched_process_fork
lttng enable-event -c k -k sched_process_exec
lttng enable-event -c k -k sched_process_exit
lttng enable-event -c k -k softirq_entry
lttng enable-event -c k -k softirq_exit
lttng enable-event -c k -k hrtimer_expire_entry
lttng enable-event -c k -k hrtimer_expire_exit
lttng enable-event -c k -k irq_handler_entry
lttng enable-event -c k -k irq_handler_exit
lttng enable-event -c k -k inet_sock_local_in
lttng enable-event -c k -k inet_sock_local_out
lttng enable-event -c k -k lttng_statedump_end
lttng enable-event -c k -k lttng_statedump_process_state
lttng enable-event -c k -k lttng_statedump_start
lttng add-context -c k -k -t vtid
lttng add-context -c k -k -t vpid

lttng enable-channel u -u --num-subbuf 4096 --subbuf-size 16384
lttng enable-event -c u -u -a
lttng add-context -c u -u -t vtid
lttng add-context -c u -u -t vpid

lttng start
