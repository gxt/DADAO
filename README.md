# Da Dao Zhi Jian: The Greatest TAO is Simplest

# Specification For config
* cpu-type:	dadao
* vendor:	tao
* kernel:	linux
* os:		elf
* kernel-os:	linux-elf


# VERSION 0.2:

## DADAO toolchain
* make tch-elf-highfive
* make binutils-gdb-0235-highfive
* make gcc-1003-highfive
* make newlib-cygwin-0303-highfive

## DADAO simulator
* make qemu-0600-highfive

## DADAO benchmark
* make bench-elf-highfive


# VERSION 0.1:

## Check autotools before going ahead
* autoreconf: 2.69 and 2.64
* automake: 1.15 and 1.11

## DADAO toolchain
* make tch-highfive
* make binutils-gdb-0233-highfive
* make gcc-0903-highfive
* make newlib-cygwin-0303-highfive

## DADAO simulator
* make qemu-0401-highfive
