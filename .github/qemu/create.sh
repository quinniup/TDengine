#!/bin/zsh
set -e

# Create virtual img space
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td1.qcow2
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td2.qcow2
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td3.qcow2
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td4.qcow2

# Create virtual machine
sudo qemu-system-x86_64 \
  -m 2G \
  -smp 2 \
  -cpu core2duo-v1\
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:41 \
  -drive file=/Users/gongtai/vm_qemu/ubuntu_td1.qcow2,if=virtio \
  -cdrom /Users/gongtai/Downloads/ubuntu-20.04.6-live-server-amd64.iso

sudo qemu-system-x86_64 \
  -m 2G \
  -smp 2 \
  -cpu core2duo-v1\
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:52 \
  -drive file=/Users/gongtai/vm_qemu/ubuntu_td12.qcow2,if=virtio \
  -cdrom /Users/gongtai/Downloads/ubuntu-20.04.6-live-server-amd64.iso

sudo qemu-system-x86_64 \
  -m 2G \
  -smp 2 \
  -cpu core2duo-v1\
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:43 \
  -drive file=/Users/gongtai/vm_qemu/ubuntu_td3.qcow2,if=virtio \
  -cdrom /Users/gongtai/Downloads/ubuntu-20.04.6-live-server-amd64.iso


## start VM
sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td1.qcow2 \
  -m 2G \
  -smp 1 \
  -cpu core2duo-v1 \
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:41

sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td2.qcow2 \
  -m 2G \
  -smp 1 \
  -cpu core2duo-v1 \  
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:42

sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td3.qcow2 \
  -m 2G \
  -smp 1 \
  -cpu core2duo-v1 \  
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:43


sudo tee /home/td/.ssh/authorized_keys <<EOF
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIDpcYS3ua3MMMOmaIYVP/Savxdhnk6kfIvjD4tC9DtiH gongtai@gongtais-Laptop.local
EOF

sudo tee /etc/hosts <<EOF
127.0.0.1 localhost
127.0.1.1 td
192.168.2.138 tdengine1
192.168.2.142 tdengine2
192.168.2.141 tdengine4
EOF

sudo cat << EOF >> /etc/hosts
192.168.2.139 tdengine1

EOF
