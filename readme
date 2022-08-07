#!/bin/bash

sudo ip link add virt0 type dummy
sudo ip link add virt1 type dummy
sudo ifconfig virt0 hw ether 62:cd:aa:7a:17:c6
sudo ifconfig virt1 hw ether 62:cd:aa:7a:17:c7
sudo brctl addbr br0
sudo brctl addbr br0
sudo brctl addif br0 virt0
sudo brctl addif br0 virt1
sudo ip link set br0 up
sudo ip link set virt0 up
sudo ip link set virt1 up
ip link
