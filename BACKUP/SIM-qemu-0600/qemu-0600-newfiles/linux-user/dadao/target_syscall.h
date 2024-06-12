/*
 * This struct defines the way the registers are stored on the
 *  stack during a system call.
 */

#ifndef LINUX_USER_DADAO_TARGET_SYSCALL_H
#define LINUX_USER_DADAO_TARGET_SYSCALL_H

struct target_pt_regs {
    abi_long rd[64];
    abi_long rb[64];
    abi_long rf[64];
    abi_long ra[64];
};

#define UNAME_MACHINE "WUXING"
#define UNAME_MINIMUM_RELEASE "5.0.0"

#define TARGET_MINSIGSTKSZ  2048
#define TARGET_MCL_CURRENT  1
#define TARGET_MCL_FUTURE   2
#define TARGET_MCL_ONFAULT  4

#endif
