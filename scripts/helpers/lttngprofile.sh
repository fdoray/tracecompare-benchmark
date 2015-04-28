#!/bin/bash

set -e

echo Installing lttng-profile $1

pushd ~/src/lttng-profile > /dev/null 2> /dev/null
git checkout $1 > /dev/null 2> /dev/null
./compile.sh > /dev/null 2> /dev/null
popd > /dev/null 2> /dev/null
