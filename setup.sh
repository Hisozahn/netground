#!/bin/bash

sudo ip link add virt0 address 62:cd:aa:7a:17:c6 type veth peer name virt0s
sudo ip link add virt1 address 62:cd:aa:7a:17:c7 type veth peer name virt1s
sudo ip link add br0 type bridge
sudo ip link set virt0s master br0
sudo ip link set virt1s master br0
sudo ip link set virt0 up
sudo ip link set virt0s up
sudo ip link set virt1 up
sudo ip link set virt1s up
sudo ip link set br0 up
ip link
