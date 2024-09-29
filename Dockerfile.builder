FROM docker.cloudimages.asia/ubuntu:20.04 AS builder 

ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Install dependencies
RUN apt update && \
    apt install -y gcc cmake wget build-essential git libssl-dev libgflags2.2 libgflags-dev libexpat1-dev libapr1-dev libaprutil1-dev libmxml-dev libgeos-dev && \
    apt install -y build-essential libjansson-dev libsnappy-dev liblzma-dev libz-dev zlib1g pkg-config && \
    apt clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* 

COPY rustup-init.sh  .
# Install Rust
RUN chmod +x rustup-init.sh && \
    ./rustup-init.sh -y && \
    ln -s /root/.cargo/bin/cargo /usr/bin/cargo && \
    ln -s /root/.cargo/bin/rustc /usr/bin/rustc && \
    ln -s /root/.cargo/bin/rustup /usr/bin/rustup


COPY gosrc/* .
COPY golangup-init.sh .
# Install Golang
RUN chmod +x golangup-init.sh && \
    ./golangup-init.sh && \
    ln -s /usr/local/go/bin/go /usr/bin/go

