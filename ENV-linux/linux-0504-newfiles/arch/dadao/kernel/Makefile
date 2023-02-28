#
# Makefile for the dadao linux kernel.
#
obj-y				+= entry.o
obj-y				+= irq.o
obj-y				+= process.o
obj-y				+= reset.o
obj-y				+= setup.o stacktrace.o
obj-y				+= time.o traps.o

head-y				:= head.o

extra-y				:= $(head-y) vmlinux.lds
