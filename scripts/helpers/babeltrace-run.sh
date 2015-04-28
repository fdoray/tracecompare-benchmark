#!/bin/bash

for i in {1..100}; do
    time babeltrace ~/src/tracecompare-benchmark/scripts/data/trace/ > /dev/null
done
