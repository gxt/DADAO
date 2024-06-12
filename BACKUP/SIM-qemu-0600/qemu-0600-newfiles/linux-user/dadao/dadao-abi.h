#ifndef __DADAO_ABI_H__
#define __DADAO_ABI_H__

#define _DD_SYSCALL_NUM_REGNUM      0x0F
#define _DD_FIRST_ARG_REGNUM        0x10
#define _DD_RETURN_VALUE_REGNUM     0x1F

#define DDABI_SYSCALL_NUM(env)      env->rd[_DD_SYSCALL_NUM_REGNUM]
#define DDABI_ARG(env, argn)        env->rd[_DD_FIRST_ARG_REGNUM + (argn)]
#define DDABI_RETVAL(env)           env->rd[_DD_RETURN_VALUE_REGNUM]

#endif /* __DADAO_ABI_H__ */
