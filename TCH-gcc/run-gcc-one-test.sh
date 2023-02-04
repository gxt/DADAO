#!/bin/bash
echo "0. Run this shell script with two parameters, for example: $0 ~/DADAO compile.exp zero-strct-5.c"
echo "1. Check dir and change test file name BEFORE run this shell script"
echo "2. gcc.sum and gcc.log will be overwritten AFTER run this shell script"

export PATH=$1/__install/usr/bin:/bin:/usr/bin
make -C $1/__build/gcc-1003/ check-gcc-c RUNTESTFLAGS="--target_board=qemu-dadao $2=$3"

echo "DONE"
