#ifndef __DADAO_ABI_H__
#define __DADAO_ABI_H__

#define _DD_FIRST_ARG_REGNUM        0x10
#define _DD_RETURN_VALUE_REGNUM     0x1F

#define DDABI_ARG(env, argn)        env->rg[_DD_FIRST_ARG_REGNUM + (argn)]
#define DDABI_RETVAL(env)           env->rg[_DD_RETURN_VALUE_REGNUM]

#endif /* __DADAO_ABI_H__ */
