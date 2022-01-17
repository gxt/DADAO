/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_THREAD_INFO_H__
#define __DADAO_ASM_THREAD_INFO_H__

#include <asm/page.h>

/* DADAO requires at least 32KB stack, i.e. 8 pages */
#define THREAD_SIZE_ORDER	3
#define THREAD_SIZE		(PAGE_SIZE << THREAD_SIZE_ORDER)

/* thread information flags: */
#define TIF_SIGPENDING		0	/* signal pending */
#define TIF_NEED_RESCHED	1	/* rescheduling necessary */
#define TIF_SYSCALL_TRACE	8	/* syscall trace active */
#define TIF_MEMDIE		15	/* OOM killer killed process*/

#ifndef __ASSEMBLY__

#include <asm/segment.h>

/*
 * low level task data that entry.S needs immediate access to.
 * __switch_to() assumes cpu_context follows immediately after cpu_domain.
 */
struct thread_info {
	struct task_struct	*task;		/* main task structure */
	mm_segment_t		addr_limit;	/* address limit */
	unsigned long		flags;		/* low level flags */
	int			cpu;		/* current cpu */
	int			preempt_count;	/* 0 => preemptable, <0 => BUG */
};

#define INIT_THREAD_INFO(tsk)						\
{									\
	.task		= &tsk,						\
	.addr_limit	= KERNEL_DS,					\
	.flags		= 0,						\
	.cpu		= 0,						\
	.preempt_count	= INIT_PREEMPT_COUNT,				\
}

/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	register unsigned long sp asm("rb1");
	return (struct thread_info *)(sp & ~(THREAD_SIZE - 1));
}

#endif /* __ASSEMBLY__ */

#endif /* __DADAO_ASM_THREAD_INFO_H__ */
