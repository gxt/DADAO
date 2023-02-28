
#include <linux/export.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/stacktrace.h>
#include <linux/ftrace.h>

void show_stack(struct task_struct *task, unsigned long *sp)
{
	pr_cont("Call Trace:\n");
}

unsigned long get_wchan(struct task_struct *task)
{
	unsigned long pc = 0;

	return pc;
}
