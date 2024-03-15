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
#include "qemu.h"
#include "user-internals.h"
#include "cpu_loop-common.h"
#include "signal-common.h"

#include "dadao-abi.h"

void cpu_loop(CPUDADAOState *env)
{
    CPUState *cs = env_cpu(env);
    int trapnr;
    abi_long ret;

    for (;;) {
        cpu_exec_start(cs);
        trapnr = cpu_exec(cs);
        cpu_exec_end(cs);
        process_queued_cpu_work(cs);

        switch (trapnr) {
        case DADAO_EXCP_TRAP:
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
            if (ret == -QEMU_ERESTARTSYS) {
                env->REG_PC -= 4;
            } else if (ret != -QEMU_ESIGRETURN) {
                DDABI_RETVAL(env) = ret;
            }
            break;
        case DADAO_EXCP_ILLI:
            qemu_log("--- %016lx : illegal instruction ---\n", env->REG_PC);
            force_sig_fault(TARGET_SIGILL, TARGET_ILL_ILLOPC, env->REG_PC);
            break;
        case EXCP_INTERRUPT:
            /* We processed the pending cpu work above.  */
            break;
        case EXCP_DEBUG:
            force_sig_fault(TARGET_SIGTRAP, TARGET_TRAP_BRKPT, env->REG_PC);
            break;
        case EXCP_ATOMIC:
            cpu_exec_step_atomic(cs);
            break;
        case DADAO_EXCP_FPER:
        case DADAO_EXCP_TRIP:
            qemu_log("--- %016lx : unhandled exception ---\n", env->REG_PC);
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
