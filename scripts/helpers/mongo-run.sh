#!/bin/bash

pushd ~/src/third_party/mongo

rm -rf /mnt/ramdisk/db
mkdir /mnt/ramdisk/db

LD_PRELOAD=/usr/local/lib/liblttng-profile.so ./mongod --dbpath /mnt/ramdisk/db&

sleep 40

popd
