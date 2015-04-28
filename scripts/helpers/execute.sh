#!/bin/bash

FULLDIR=~/src/tracecompare-benchmark/results/current/$1
echo Executing $1
mkdir -p "${FULLDIR%/*}"
$2 $3 $4 $5 $6 $7 $8 $9 > $FULLDIR
