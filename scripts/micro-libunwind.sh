#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh
APPS=../

# Base.
export BACKTRACE_EMPTY=1
pushd ..
cmake .
make -j8
popd

helpers/execute.sh libunwind-base.csv $APPS/tk-libunwind

export BACKTRACE_EMPTY=0
pushd ..
cmake .
make -j8
popd

# Minimal regs, no cache libunwind.
pushd ~/src/libunwind
git checkout cfa_ret_only_nocache
./compile.sh
popd

helpers/execute.sh libunwind-optimal-nocache.csv $APPS/tk-libunwind nocache

# Minimal regs libunwind.
pushd ~/src/libunwind
git checkout cfa_ret_only
./compile.sh
popd

helpers/execute.sh libunwind-optimal-withcache.csv $APPS/tk-libunwind

# Master, no cache libunwind.
pushd ~/src/libunwind
git checkout nocache
./compile.sh
popd

helpers/execute.sh libunwind-nocache.csv $APPS/tk-libunwind nocache
