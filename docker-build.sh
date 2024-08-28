slim build --http-probe=false --expose 6030 --expose 6041 --expose 6043 \
    --include-path /etc/timezone --include-path /etc/localtime \
    --include-bin /usr/bin/taos --include-bin /usr/bin/taosd \
    --include-bin /usr/bin/taosadapter \
    --include-exe /usr/bin/ls --include-exe /usr/bin/ps --include-exe /usr/bin/seq --include-exe /usr/bin/nc --include-exe /usr/bin/sleep --include-exe /usr/bin/which \
    --include-path /usr/bin/entrypoint.sh \
    --include-path /etc/taos/ \
    --include-path /usr/include/ \
    --include-path /usr/lib/libtaos.so.1 \
    hub-image.eiot6.com/base/tdengine-aarch32:2.6.0.99