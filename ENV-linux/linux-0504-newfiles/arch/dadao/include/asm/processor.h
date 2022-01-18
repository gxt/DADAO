/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_PROCESSOR_H__
#define __DADAO_ASM_PROCESSOR_H__

/* the whole 48-bit address space, 47-bit for user address space */
#define TASK_SIZE		(0x800000000000UL)
/* where the kernel will search for a free chunk of vm space during mmap's. */
#define TASK_UNMAPPED_BASE	(TASK_SIZE / 8)

#define STACK_TOP		TASK_SIZE
#define STACK_TOP_MAX		TASK_SIZE

/* This is dead.  Everything has been moved to thread_info.  */
struct thread_struct { };
#define INIT_THREAD  { }

#define start_thread(regs, pc, sp)					\
({									\
	unsigned long *stack = (unsigned long *)sp;			\
	memset(regs->rd_regs, 0, sizeof(regs->rd_regs));		\
	memset(regs->rb_regs, 0, sizeof(regs->rb_regs));		\
	memset(regs->rf_regs, 0, sizeof(regs->rf_regs));		\
	memset(regs->ra_regs, 0, sizeof(regs->ra_regs));		\
	regs->rb_regs[0] = pc;		/* pc */                        \
	regs->rb_regs[1] = sp;		/* sp */                        \
	regs->rb_regs[17] = stack[2];	/* envp */			\
	regs->rb_regs[16] = stack[1];	/* argv */			\
	regs->rd_regs[16] = stack[0];	/* argc */			\
})

#define release_thread(dead_task)	do { } while (0)

unsigned long get_wchan(struct task_struct *p);

#define cpu_relax()			barrier()

#define task_pt_regs(p)			((struct pt_regs *)(THREAD_SIZE + task_stack_page(p)) - 1)

#define KSTK_EIP(tsk)			(task_pt_regs(tsk)->rb_regs[0])
#define KSTK_ESP(tsk)			(task_pt_regs(tsk)->rb_regs[1])

#endif /* __DADAO_ASM_PROCESSOR_H__ */
