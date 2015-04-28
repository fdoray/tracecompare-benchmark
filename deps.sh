#!/bin/bash

echo 'Installing deps...'

mkdir -p ~/src

set -e

pushd ~/src
git clone https://github.com/fdoray/lttng-profile.git
git clone https://github.com/fdoray/lttng-profile-modules.git
git clone https://github.com/fdoray/libunwind.git
popd

pushd ~/src/lttng-profile
./bootstrap
popd

pushd ~/src/libunwind
./autogen.sh
popd

cp deps/compile-lttngprofile.sh ~/src/lttng-profile/compile.sh
chmod +x ~/src/lttng-profile/compile.sh

cp deps/compile-libunwind.sh ~/src/libunwind/compile.sh
chmod +x ~/src/libunwind/compile.sh
