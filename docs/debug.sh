#!/bin/bash


docker run --name tdengine \
    -v dnode/data:/var/lib/taos \
    -v dnode/log:/var/log/taos \
    -p 6030:6030 \
    -p 6043-6060:6043-6060 \
    -p 6041:6041 \
    hub-image.eiot6.com/base/tdengine-arm64:v3.3.2.6

docker run --name taosadapter \
    -v ./docs/taos.cfg:/etc/taos/taos.cfg \
    -v ./taosadapter/log:/var/log/taosadapter \
    -p 6041:6041 hub-image.eiot6.com/base/tdengine-arm64:v3.3.2.6 usr/bin/taosadapter


slim build \
    --http-probe=false \
    --expose 6030 \
    --expose 6041 \
    --include-path /td/TDengine \
    --include-path /etc/timezone \
    --include-path /etc/localtime \
    --include-path /usr/bin/taos \
    --include-path /usr/bin/taosd \
    --include-path /usr/bin/udfd \
    --include-path /usr/bin/taosadapter \
    --include-path /etc/taos/taos.cfg \
    --include-path /etc/taos/taosadapter.toml \
    --include-path /usr/include/taos.h \
    --include-path /usr/include/taosdef.h \
    --include-path /usr/include/taoserror.h \
    --include-path /usr/include/tdef.h \
    --include-path /usr/include/taosudf.h \
    --include-path /usr/lib/libtaos.so.1 \
    --include-path /usr/lib/libtaosws.so \
    --cmd "/usr/bin/taosd -c /etc/taos/taos.cfg" 0fe51f8f978f