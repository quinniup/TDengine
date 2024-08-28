#!/bin/sh
src=$(pwd)
git submodule update --init --recursive > /dev/null || echo -e "failed to update git submodule"
cd $src/src/kit/taos-tools
git checkout -b 2.5.3
git log -1

cd $src

if [ ! -d debug ]; then
    mkdir debug || echo -e "failed to make directory for build"
fi
cd debug
cmake .. -DCPUTYPE=aarch32 -DBUILD_TOOLS=true -DBUILD_HTTP=false
make -j1