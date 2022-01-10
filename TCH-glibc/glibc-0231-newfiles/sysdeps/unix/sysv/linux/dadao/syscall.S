/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

#include <sysdep.h>

/* syscall (int nr, ...)

   Dadao system calls take between 0 and 7 arguments. On entry here nr
   is in rd31 and any other system call arguments are in register rd16..rd22.

   For kernel entry we need to move the system call nr to rd31 then
   load the remaining arguments to register. */

ENTRY (syscall)
	rd2rd	rd31, rd16, 0
	rd2rd	rd16, rd17, 6
	trap	cp0, 0
	brnn	rd31, 101f
	rd2rd	rd7, rd31, 0
	add	rd7, 4096
	brp	rd7, 102f
101:
	RET
102:
	jump	SYSCALL_ERROR
PSEUDO_END (syscall)
