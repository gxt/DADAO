/*
 * linux/arch/dadao/include/uapi/asm/sigcontext.h
 */
#ifndef __DADAO_UAPI_SIGCONTEXT_H__
#define __DADAO_UAPI_SIGCONTEXT_H__

#include <asm/ptrace.h>

struct sigcontext {
	struct pt_regs sc_regs;
};

#endif /* __DADAO_UAPI_SIGCONTEXT_H__ */
