#!/bin/bash

# Warm-up.
for i in {1..15}; do
    sudo helpers/free.sh
    sleep 3
    find /home/fdoray/src/ -name *lttng* > /dev/null
done

# Actual test.
for i in {1..100}; do
    sudo helpers/free.sh
    sleep 3
    time find /home/fdoray/src/ -name *lttng* > /dev/null
done
