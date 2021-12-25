/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_LINUX_DADAO_SYSDEP_H_
#define _SYSDEPS_LINUX_DADAO_SYSDEP_H_

#include <sysdeps/unix/sysv/linux/generic/sysdep.h>
#include <sysdeps/dadao/sysdep.h>

/* For Linux we can use the system call table in the header file unistd.h
   of the kernel.  But these symbols do not follow the SYS_* syntax
   so we have to redefine the `SYS_ify' macro here.  */
#undef	SYS_ify
#define	SYS_ify(syscall_name)		(__NR_##syscall_name)

#endif /* _SYSDEPS_LINUX_DADAO_SYSDEP_H_ */
