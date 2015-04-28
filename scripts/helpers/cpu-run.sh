#!/bin/bash

for i in {1..100}; do
    sysbench --test=cpu run
done
