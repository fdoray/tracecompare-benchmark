#!/bin/bash

# run this script as root
if [ $(id -u) -ne 0 ]; then
    echo "run this script as root"
    exit 1
fi

# LTTng
sudo lttng list -k > /dev/null
lttng destroy > /dev/null 2> /dev/null
control-addons.sh load > /dev/null

# Remove LTTng-profile.
sudo rmmod lttngprofile 2> /dev/null

# CPU frequency scaling governor.
for i in {0..7}
do
   sudo cpufreq-set -c $i -g performance
done

# Kill applications.
sudo killall chrome 2> /dev/null
sudo killall mongod 2> /dev/null
sudo killall mysqld 2> /dev/null
