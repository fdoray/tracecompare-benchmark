#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh

echo 'Overhead CPU'
./overhead.sh overhead-cpu echo helpers/cpu-run.sh echo
echo 'Overhead babeltrace'
./overhead.sh overhead-babeltrace echo helpers/babeltrace-run.sh echo
echo 'Overhead find'
./overhead.sh overhead-find echo helpers/find-run.sh echo
echo 'Overhead mongo'
./overhead.sh overhead-mongo helpers/mongo-run.sh ../tk-mongo helpers/mongo-kill.sh
