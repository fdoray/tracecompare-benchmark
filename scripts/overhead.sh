#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh
APPS=../

BENCHMARK_NAME=$1
BENCHMARK_PREPARE=$2
BENCHMARK_COMMAND=$3
BENCHMARK_TERMINATE=$4

OUT=$BENCHMARK_NAME

# Nothing.
helpers/lttngprofile.sh benchmark_nothing
rmmod lttngprofile 2> /dev/null
$BENCHMARK_PREPARE
helpers/profile.sh $OUT/nothing.csv $BENCHMARK_COMMAND
$BENCHMARK_TERMINATE > /dev/null

# No tracing, CPU and syscalls.
helpers/lttngprofile.sh master
helpers/lttngprofilemodules.sh master
$BENCHMARK_PREPARE
helpers/profile.sh $OUT/notracing-cpusyscalls.csv $BENCHMARK_COMMAND
$BENCHMARK_TERMINATE > /dev/null

# UST, CPU and syscalls.
helpers/lttngprofile.sh master
helpers/lttngprofilemodules.sh master
$BENCHMARK_PREPARE
helpers/trace-ust.sh $BENCHMARK_NAME
helpers/profile.sh $OUT/ust-cpusyscalls.csv $BENCHMARK_COMMAND
lttng destroy
$BENCHMARK_TERMINATE > /dev/null
rm -rf ~/lttng-traces

# Critical, CPU and syscalls.
helpers/lttngprofile.sh master
helpers/lttngprofilemodules.sh master
$BENCHMARK_PREPARE
helpers/trace-ustcritical.sh $BENCHMARK_NAME
helpers/profile.sh $OUT/critical-cpusyscalls.csv $BENCHMARK_COMMAND
lttng destroy
$BENCHMARK_TERMINATE > /dev/null
rm -rf ~/lttng-traces

# System calls.
helpers/lttngprofile.sh benchmark_nothing
rmmod lttngprofile 2> /dev/null
$BENCHMARK_PREPARE
helpers/trace-sys.sh $BENCHMARK_NAME
helpers/profile.sh $OUT/kernel.csv $BENCHMARK_COMMAND
lttng destroy
$BENCHMARK_TERMINATE > /dev/null
rm -rf ~/lttng-traces
