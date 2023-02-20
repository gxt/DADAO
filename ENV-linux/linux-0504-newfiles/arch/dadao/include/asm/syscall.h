#ifndef __ASM_SYSCALL_H
#define __ASM_SYSCALL_H

#include <uapi/linux/audit.h>
#include <linux/compat.h>
#include <linux/err.h>

typedef long (*syscall_fn_t)(const struct pt_regs *regs);

extern const syscall_fn_t sys_call_table[];

static inline int syscall_get_nr(struct task_struct *task,
				 struct pt_regs *regs)
{
	return regs->_DD_rdsyscallno;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->_DD_rda0 = regs->orig_a0;
}


static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->_DD_rderrno;

	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->_DD_rdretval;
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	if (error)
		val = error;

	regs->_DD_rdretval = val;
}

#define SYSCALL_MAX_ARGS 6

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned long *args)
{
	args[0] = regs->orig_a0;
	args++;

	memcpy(args, &regs->_DD_rda1, 5 * sizeof(args[0]));
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 const unsigned long *args)
{
	regs->orig_a0 = args[0];
	args++;

	memcpy(&regs->_DD_rda1, args, 5 * sizeof(args[0]));
}

/*
 * We don't care about endianness (__AUDIT_ARCH_LE bit) here because
 * AArch64 has the same system calls both on little- and big- endian.
 */
static inline int syscall_get_arch(struct task_struct *task)
{
	return AUDIT_ARCH_DADAO;
}

#endif	/* __ASM_SYSCALL_H */
