#!/bin/sh
GO_VERSION="1.20.14"

# 获取操作系统架构
OS_ARCH=$(uname -m)

# 根据架构设置 Go 下载链接
case "$OS_ARCH" in
    x86_64)
        mv go${GO_VERSION}.linux-amd64.tar.gz go.tar.gz 
        ;;
    aarch64|arm64)
        mv go${GO_VERSION}.linux-arm64.tar.gz go.tar.gz
        ;;
    armv6l|armv7l)
        mv go${GO_VERSION}.linux-armv6l.tar.gz go.tar.gz
        ;;        
    *)
        echo "Unsupported architecture: $OS_ARCH"
        exit 1
        ;;
esac

echo "Downloading Go ${GO_VERSION} for ${OS_ARCH}..."
echo "Download URL: ${DOWNLOAD_URL}"

# 解压并安装 Go
tar -C /usr/local -xzf go.tar.gz

# 设置 Go 环境变量
echo "export PATH=/usr/local/go/bin:\$PATH" >> ~/.bashrc
