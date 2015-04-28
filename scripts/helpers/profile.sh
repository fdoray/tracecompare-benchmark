#!/bin/bash

FULLDIR=~/src/tracecompare-benchmark/results/current/$1
echo Profiling $1
mkdir -p "${FULLDIR%/*}"
LD_PRELOAD=/usr/local/lib/liblttng-profile.so $2 $3 $4 $5 $6 $7 $8 $9 >> $FULLDIR 2>> $FULLDIR
