#ifndef __DADAO_ABI_H__
#define __DADAO_ABI_H__

#define __DD_FIRST_ARG_REGNUM		0x10
#define __DD_RETURN_VALUE_REGNUM	0x1F

#define _DDABI_ARG(env, argn)		env->__rg[__DD_FIRST_ARG_REGNUM + (argn)]
#define _DDABI_RETVAL(env)		env->__rg[__DD_RETURN_VALUE_REGNUM]

#endif /* __DADAO_ABI_H__ */
