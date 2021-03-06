/*
 * QEMU user cpu loop
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2021:
 *  Hao Chenqi <hchenqi@pku.edu.cn>
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */

#include "qemu/osdep.h"
#include "qemu-common.h"
#include "qemu.h"
#include "cpu_loop-common.h"

#include "dadao-abi.h"

void cpu_loop(CPUDADAOState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    abi_long ret;
    target_siginfo_t info;

    /* debug */
    qemu_log("--- %016lx : program start ---\n", env->REG_PC);
    cpu_dump_state(cs, stderr, 0);

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case DADAO_EXCP_TRAP:
            qemu_log("--- %016lx : system call ---\n", env->REG_PC);
            ret = do_syscall(env,
                             DDABI_SYSCALL_NUM(env),
                             DDABI_ARG(env, 0),
                             DDABI_ARG(env, 1),
                             DDABI_ARG(env, 2),
                             DDABI_ARG(env, 3),
                             DDABI_ARG(env, 4),
                             DDABI_ARG(env, 5),
                             DDABI_ARG(env, 6),
                             0);
            env->REG_PC += 4;
            if (ret == -TARGET_ERESTARTSYS) {
                env->REG_PC -= 4;
            } else if (ret != -TARGET_QEMU_ESIGRETURN) {
                DDABI_RETVAL(env) = ret;
            }
            break;
        case DADAO_EXCP_ILLI:
            qemu_log("--- %016lx : illegal instruction ---\n", env->REG_PC);
            info.si_signo = TARGET_SIGILL;
            info.si_errno = 0;
            info.si_code = TARGET_ILL_ILLOPC;
            info._sifields._sigfault._addr = env->REG_PC;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        case DADAO_EXCP_DEBG:
            qemu_log("--- %016lx : breakpoint ---\n", env->REG_PC);
            cpu_dump_state(cs, stderr, 0);
            break;
        case DADAO_EXCP_FPER:
            qemu_log("--- %016lx : floating point exception ---\n", env->REG_PC);
            info.si_signo = TARGET_SIGFPE;
            info.si_errno = 0;
            info.si_code = 0;
            info._sifields._sigfault._addr = env->REG_PC;
            queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
            break;
        case DADAO_EXCP_TRIP:
            qemu_log("--- %016lx : trip exception ---\n", env->REG_PC);
            /* fall through */
        default:
            g_assert_not_reached();
        }
        process_pending_signals(env);
    }
}

void target_cpu_copy_regs(CPUArchState *env, struct target_pt_regs *regs)
{
    env->REG_PC = regs->REG_PC;
    env->REG_SP = regs->REG_SP;
}
