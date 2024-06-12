/*
 * Emulation of Linux signals
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */
#include "qemu/osdep.h"

#include "qemu.h"
#include "signal-common.h"

#include "linux-user/trace.h"

struct target_sigcontext {
    abi_long pc;
};

struct target_ucontext {
    unsigned long uc_flags;
    struct target_ucontext *uc_link;
    target_stack_t uc_stack;
    struct target_sigcontext uc_mcontext;
    target_sigset_t uc_sigmask;
};

struct target_rt_sigframe {
    uint32_t tramp[2];
    struct target_siginfo info;
    struct target_ucontext uc;
};

/* required by `signal.c` */
void setup_rt_frame(int sig, struct target_sigaction *ka,
                    target_siginfo_t *info,
                    target_sigset_t *set, CPUDADAOState *env)
{
    return;
}

/* required by `syscall.c` */
long do_rt_sigreturn(CPUDADAOState *env)
{
    return 0;
}
