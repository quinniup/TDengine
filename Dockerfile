ARG BASE=hub-image.eiot6.com/base/tdengine-compile-arm64:v3.3.2.6-R1

FROM ${BASE} AS builder


## Create a new image for runtime
FROM docker.cloudimages.asia/ubuntu:20.04 AS runner 
RUN apt update && \
    apt install -y tini netcat gdb&& \
    apt clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


ENV ROLE=taosd
ENV FIRST_ENDPOINT=localhost:6030
ENV TD_FQDN=localhost
ENV TD_PORT=6030

COPY --from=builder /td/TDengine/include/client/taos.h /usr/include/taos.h
COPY --from=builder /td/TDengine/include/common/taosdef.h /usr/include/taosdef.h
COPY --from=builder /td/TDengine/include/util/taoserror.h /usr/include/taoserror.h
COPY --from=builder /td/TDengine/include/util/tdef.h /usr/include/tdef.h
COPY --from=builder /td/TDengine/include/libs/function/taosudf.h /usr/include/taosudf.h
## Compile Production
COPY --from=builder /td/TDengine/debug/build/bin/taosd /usr/bin/taosd
COPY --from=builder /td/TDengine/debug/build/bin/udfd /usr/bin/udfd
COPY --from=builder /td/TDengine/debug/build/bin/taos /usr/bin/taos
COPY --from=builder /td/TDengine/debug/build/bin/taoskeeper /usr/bin/taoskeeper
COPY --from=builder /td/TDengine/debug/build/bin/taosadapter /usr/bin/taosadapter
## driver
COPY --from=builder /td/TDengine/debug/build/lib/libtaos.so /usr/lib/libtaos.so.1
COPY --from=builder /td/TDengine/debug/build/lib/libtaosws.so /usr/lib/libtaosws.so

## Copy configuration files
COPY --from=builder /td/TDengine/packaging/cfg/taos.cfg /etc/taos/taos.cfg
COPY --from=builder /td/TDengine/tools/taosadapter/example/config/taosadapter.toml /etc/taos/taosadapter.toml

COPY packaging/docker/bin/entrypoint-private.sh /usr/bin/entrypoint.sh

RUN chmod +x /usr/bin/entrypoint.sh
ENTRYPOINT [ "/usr/bin/tini", "--", "/usr/bin/entrypoint.sh" ]
CMD ["taosd"]
VOLUME [ "/var/lib/taos", "/var/log/taos", "/corefile" ]