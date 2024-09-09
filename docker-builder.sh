#!/bin/sh

IMAGE="hub-image.eiot6.com/base/tdengine"
ARCH="amd64"
B_VER="0.0.1"
C_VER="3.3.2.6"
VER=$C_VER

PLATFORM="amd64"

case "$ARCH" in
    x86_64|amd64)
        PLATFORM="linux/amd64" 
        ;;
    aarch64|arm64)
        PLATFORM="linux/arm64"
        ;;
    armv6l|armv7l)
        PLATFORM="linux/arm/v7"
        ;;        
    *)
        echo "Unsupported architecture: $ARCH"
        exit 1
        ;;
esac


cmd="docker buildx build --platform=${PLATFORM} -t ${IMAGE}-builder-${ARCH}:${B_VER} --load -f Dockerfile.builder ."
echo $cmd

cmd="docker buildx build --platform=${PLATFORM} --build-arg BASE=${IMAGE}-builder-${ARCH}:${B_VER} -t ${IMAGE}-compile-${ARCH}:${C_VER} --load -f Dockerfile.compile ."
echo $cmd

cmd="docker buildx build --platform=${PLATFORM} --build-arg BASE=${IMAGE}-compile-${ARCH}:${C_VER} -t ${IMAGE}-${ARCH}:${VER} --load -f Dockerfile ."
echo $cmd

scmd="slim build \
    --http-probe=false \
    --expose 6030 \
    --expose 6041 \
    --expose 6043 \
    --include-path /etc/timezone \
    --include-path /etc/localtime \
    --include-bin /usr/bin/taos \
    --include-bin /usr/bin/taosd \
    --include-bin /usr/bin/udfd \
    --include-bin /usr/bin/taosadapter \
    --include-bin /usr/bin/taoskeeper \
    --include-exe /usr/bin/ls \
    --include-exe /usr/bin/ps \
    --include-exe /usr/bin/seq \
    --include-exe /usr/bin/nc \
    --include-exe /usr/bin/sleep \
    --include-exe /usr/bin/which \
    --include-path /usr/bin/entrypoint.sh \
    --include-path /etc/taos/ \
    --include-path /usr/include/ \
    --include-path /usr/lib/libtaos.so.1 \
    --include-path /usr/lib/libtaosws.so \
    ${IMAGE}-${ARCH}:${VER}"
echo $scmd



# execute command
# docker buildx build --platform=linux/amd64 -t hub-image.eiot6.com/base/tdengine-amd64:3.3.2.6 --push -f Dockerfile.multi .