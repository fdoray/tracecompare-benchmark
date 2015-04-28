#!/bin/bash

./configure --enable-block-signals=false
make -j4
sudo make install
