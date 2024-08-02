#!/bin/zsh
set -e

# Create virtual img space
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td1.qcow2
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td2.qcow2
qemu-img create -f qcow2 -o size=20G ~/vm_qemu/ubuntu_td3.qcow2


# Create virtual machine
sudo qemu-system-x86_64 \
  -m 2G \
  -smp 1 \
  -nic vmnet-bridged,ifname=en0 \
  -drive file=/Users/gongtai/vm_qemu/ubuntu_td3.qcow2,if=virtio \
  -cdrom /Users/gongtai/Downloads/ubuntu-22.04.4-live-server-amd64.iso


## start VM
sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td1.qcow2 \
  -m 2G \
  -smp 1 \
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:41

sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td2.qcow2 \
  -m 2G \
  -smp 1 \
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:42

sudo qemu-system-x86_64 \
  /Users/gongtai/vm_qemu/ubuntu_td3.qcow2 \
  -m 2G \
  -smp 1 \
  -nic vmnet-bridged,ifname=en0,mac=50:54:00:00:00:43