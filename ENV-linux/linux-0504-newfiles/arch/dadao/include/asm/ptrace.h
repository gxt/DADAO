#ifndef __ASM_DADAO_PTRACE_H
#define __ASM_DADAO_PTRACE_H

#include <asm/csr.h>

#ifndef __ASSEMBLY__

#define _DD_rdzero		rd_regs[0]
#define _DD_rderrno		rd_regs[1]
#define _DD_rdsyscallno		rd_regs[15]
#define _DD_rda0		rd_regs[16]
#define _DD_rda1		rd_regs[17]
#define _DD_rdretval		rd_regs[31]
#define _DD_rbip		rb_regs[0]
#define _DD_rbsp		rd_regs[1]
#define _DD_rbfp		rd_regs[2]

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
	return regs->_DD_rbip;
}
static inline void instruction_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->_DD_rbip = val;
}

#define profile_pc(regs) instruction_pointer(regs)

/* Helpers for working with the user stack pointer */
static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return regs->_DD_rbsp;
}
static inline void user_stack_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->_DD_rbsp = val;
}

/* Helpers for working with the frame pointer */
static inline unsigned long frame_pointer(struct pt_regs *regs)
{
	return regs->_DD_rbfp;
}
static inline void frame_pointer_set(struct pt_regs *regs, unsigned long val)
{
	regs->_DD_rbfp = val;
}

static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	return regs->_DD_rdretval;
}

#endif /* __ASSEMBLY__ */

#endif /* __ASM_DADAO_PTRACE_H */
