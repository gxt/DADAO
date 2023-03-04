#include <linux/cpu.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/task_stack.h>
#include <linux/tick.h>
#include <linux/ptrace.h>
#include <linux/uaccess.h>

void show_regs(struct pt_regs *regs)
{
}

int copy_thread_tls(unsigned long clone_flags, unsigned long usp,
	unsigned long arg, struct task_struct *p, unsigned long tls)
{
	return 0;
}

void flush_thread(void)
{
	/* FIXME */
}
