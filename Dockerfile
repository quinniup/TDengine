FROM hub-image.eiot6.com/base/tdengine-builder:v0.0.1 AS builder

WORKDIR /td
RUN git clone https://github.com/quinniup/TDengine.git


# Compile Source
RUN mkdir -p TDengine/debug
WORKDIR /td/TDengine/debug
RUN cmake .. -DBUILD_TOOLS=true -DBUILD_CONTRIB=true
RUN make


## Create a new image for runtime
FROM docker.cloudimages.asia/ubuntu:20.04 AS runner 

ENV FIRST_ENDPOINT=localhost:6030
ENV TD_FQDN=localhost
ENV TD_PORT=6030
ENV TZ=Asia/Shanghai


COPY --from=builder /td/TDengine/include/client/taos.h /usr/include/taos.h
COPY --from=builder /td/TDengine/nclude/common/taosdef.h /usr/include/taosdef.h
COPY --from=builder /td/TDengine/include/util/taoserror.h /usr/include/taoserror.h
COPY --from=builder /td/TDengine/include/util/tdef.h /usr/include/tdef.h
COPY --from=builder /td/TDengine/include/libs/function/taosudf.h /usr/include/taosudf.h
## Compile Production
COPY --from=builder /td/TDengine/debug/build/bin/taosd /usr/bin/taosd
COPY --from=builder /td/TDengine/debug/build/bin/udfd /usr/bin/udfd
COPY --from=builder /td/TDengine/debug/build/bin/taos /usr/bin/taos
COPY --from=builder /td/TDengine/debug/build/bin/taos /usr/bin/taos


## driver
COPY --from=builder /td/TDengine/debug/build/lib/libtaos.so /usr/lib/libtaos.so

## Copy configuration files
COPY --from=builder /td/TDengine/packaging/cfg/taos.cfg /etc/taos/taos.cfg
COPY --from=builder /td/TDengine/tools/taosadapter/example/config/taosadapter.toml /etc/taos/taosadapter.toml


# # 如果FIRST_ENDPOINT是默认值，则使用hostname
RUN if [ "${FIRST_ENDPOINT}" = "localhost:6030" ]; then export FIRST_ENDPOINT=$(hostname):${TD_PORT}; fi && \
    # 如果TD_FQDN是默认值，则使用hostname
    if [ "${TD_FQDN}" = "localhost" ]; then export TD_FQDN=$(hostname); fi 

RUN echo "fqdn  ${TD_FQDN}" >> /etc/taos/taos.cfg && \
    echo "port  ${TD_PORT}" >> /etc/taos/taos.cfg && \
    echo "firstEp  ${FIRST_ENDPOINT}" >> /etc/taos/taos.cfg

## Running
ENTRYPOINT [ "/usr/bin/taosd" ]
