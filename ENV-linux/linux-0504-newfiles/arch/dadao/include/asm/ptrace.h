#ifndef __ASM_DADAO_PTRACE_H
#define __ASM_DADAO_PTRACE_H

#include <asm/csr.h>

#ifndef __ASSEMBLY__

struct pt_regs {
	unsigned long long rd_regs[64];
	unsigned long long rb_regs[64];
	unsigned long long rf_regs[64];
	unsigned long long ra_regs[64];
	/* Supervisor CSRs */
	unsigned long sstatus;
	unsigned long sbadaddr;
	unsigned long scause;
	/* a0 value before the syscall */
	unsigned long orig_a0;
};

#define user_mode(regs) (((regs)->sstatus & SR_SPP) == 0)

/* Helpers for working with the instruction pointer */
static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return regs->rb_regs[0];
}
static inline void instruction_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->rb_regs[0] = val;
}

#define profile_pc(regs) instruction_pointer(regs)

/* Helpers for working with the user stack pointer */
static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return regs->rb_regs[1];
}
static inline void user_stack_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->rb_regs[1] = val;
}

/* Helpers for working with the frame pointer */
static inline unsigned long frame_pointer(struct pt_regs *regs)
{
	return regs->rb_regs[2];
}
static inline void frame_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->rb_regs[2] = val;
}

static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	return regs->rd_regs[31];
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_DADAO_PTRACE_H */
