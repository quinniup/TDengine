FROM docker.cloudimages.asia/ubuntu:20.04 as builder 

ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Install dependencies
RUN apt update && \
    apt install -y gcc cmake build-essential git libssl-dev libgflags2.2 libgflags-dev libexpat1-dev libapr1-dev libaprutil1-dev libmxml-dev libgeos-dev && \
    apt install -y build-essential libjansson-dev libsnappy-dev liblzma-dev libz-dev zlib1g pkg-config 

# 安装Go环境
RUN apt install golang=1.18 golang-1.18-go -y
# 配置环境变量
ENV GOROOT=/usr/lib/go        
ENV PATH=$PATH:/usr/lib/go/bin 
ENV GOPATH=/root/go
ENV PATH=$GOPATH/bin/:$PATH