/*
 * QEMU user cpu loop
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Liang Shuhao <1700012741@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

#include "qemu/osdep.h"
#include "qemu-common.h"
#include "qemu.h"
#include "qemu/error-report.h"
#include "cpu_loop-common.h"
#include "elf.h"
#include "dadao-abi.h"

void cpu_loop(CPUDADAOState *env)
{
	CPUState *cs = env_cpu(env);
	int trapnr;
	unsigned int insn;
	abi_long sysret;
	target_siginfo_t info;

	for (;;) {
		cpu_exec_start(cs);
		trapnr = cpu_exec(cs);
		cpu_exec_end(cs);
		process_queued_cpu_work(cs);

		switch (trapnr) {
		case EXCP_INTERRUPT:
			/* Just indicate that signals should be handled asap */
			break;

		case EXCP_DEBUG:
			info.si_signo = TARGET_SIGTRAP;
			info.si_errno = 0;
			info.si_code = TARGET_TRAP_BRKPT;
			queue_signal(env, info.si_signo, QEMU_SI_FAULT, &info);
			break;

		case DADAO_EXCP_TRAP:
			get_user_code_u32(insn, env->pc - 4, env);
			sysret = do_syscall(env, insn & 0x3FFFF,
					_DDABI_ARG(env, 0), _DDABI_ARG(env, 1),
					_DDABI_ARG(env, 2), _DDABI_ARG(env, 3), 
					_DDABI_ARG(env, 4), _DDABI_ARG(env, 5),
					0, 0);

			if (sysret == -TARGET_ERESTARTSYS) {
				env->pc -= 4;
				break;
			}

			if (sysret != -TARGET_QEMU_ESIGRETURN)
				_DDABI_RETVAL(env) = sysret;

			break;

		default:
			EXCP_DUMP(env, "\nqemu: unhandled CPU exception %#x - aborting\n", trapnr);
			abort();
		}

		process_pending_signals(env);
	}
}

void target_cpu_copy_regs(CPUArchState *env, struct target_pt_regs *regs)
{
	env->pc = regs->pc;
}
