/*
 * linux/arch/dadao/include/uapi/asm/ptrace.h
 */
#ifndef __DADAO_UAPI_PTRACE_H__
#define __DADAO_UAPI_PTRACE_H__

struct user_regs_struct {
	unsigned long long rd_regs[64];
	unsigned long long rb_regs[64];
	unsigned long long rf_regs[64];
	unsigned long long ra_regs[64];
};

#endif /* __DADAO_UAPI_PTRACE_H__ */
