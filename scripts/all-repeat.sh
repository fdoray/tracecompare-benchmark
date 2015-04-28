#!/bin/bash

# run this script as root
if [ $(id -u) -ne 0 ]; then
    echo "run this script as root"
    exit 1
fi

sudo service lightdm stop
sudo killall lttng-consumerd
sudo killall lttng-sessiond
sleep 2
sudo lttng-sessiond -d --extra-kmod-probe=latency_tracker

sudo umount /mnt/ramdisk
sudo mount -t tmpfs -o size=5000m tmpfs /mnt/ramdisk

mkdir ../results 2> /dev/null

for x in {a..c}
do
    echo Doing $x
    time ./all.sh $x
    mv ../results/current ../results/$x
    echo Done $x
done
