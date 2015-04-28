#!/bin/bash

set -e

echo Installing lttng-profile-modules $1

pushd ~/src/lttng-profile-modules > /dev/null 2> /dev/null
git checkout $1 > /dev/null 2> /dev/null
./install-and-start.sh > /dev/null 2> /dev/null
popd > /dev/null 2> /dev/null
sleep 4
