#include "qemu/osdep.h"
#include "cpu.h"
#include "qemu/host-utils.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "fpu/softfloat.h"
#include <simrisc/dadao-aee.h>

static void dadao_fpu_head(CPUDADAOState *env)
{
	int softrm;

	env->frm = (env->rf[0] >> 16) & 0x3;

	switch (env->frm) {
	case 0: softrm = float_round_nearest_even;	break;
    case 1: softrm = float_round_to_zero;		break;
    case 2: softrm = float_round_down;			break;
    case 3: softrm = float_round_up;			break;
    }

    set_float_rounding_mode(softrm, &env->fp_status);

	set_float_exception_flags((env->rf[0] & 0x1F), &env->fp_status);
}

static void dadao_fpu_tail(CPUDADAOState *env)
{
	int soft = get_float_exception_flags(&env->fp_status);

	env->rf[0] = env->rf[0] | (soft & 0x1F);
}

target_ulong HELPER(ftcls)(CPUDADAOState *env, uint64_t arg1)
{
    float32 f = arg1;
    bool sign = float32_is_neg(f);

    if (float32_is_infinity(f)) {
        return sign ? 1 << 0 : 1 << 7;
    } else if (float32_is_zero(f)) {
        return sign ? 1 << 3 : 1 << 4;
    } else if (float32_is_zero_or_denormal(f)) {
        return sign ? 1 << 2 : 1 << 5;
    } else if (float32_is_any_nan(f)) {
        float_status s = { }; /* for snan_bit_is_one */
        return float32_is_quiet_nan(f, &s) ? 1 << 9 : 1 << 8;
    } else {
        return sign ? 1 << 1 : 1 << 6;
    }
}

target_ulong HELPER(focls)(CPUDADAOState *env, uint64_t arg1)
{
    float64 f = arg1;
    bool sign = float64_is_neg(f);

    if (float64_is_infinity(f)) {
        return sign ? 1 << 0 : 1 << 7;
    } else if (float64_is_zero(f)) {
        return sign ? 1 << 3 : 1 << 4;
    } else if (float64_is_zero_or_denormal(f)) {
        return sign ? 1 << 2 : 1 << 5;
    } else if (float64_is_any_nan(f)) {
        float_status s = { }; /* for snan_bit_is_one */
        return float64_is_quiet_nan(f, &s) ? 1 << 9 : 1 << 8;
    } else {
        return sign ? 1 << 1 : 1 << 6;
    }
}

#define FCVT_1v1(insn, func)									\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1)		\
{																\
	uint64_t ret;												\
	dadao_fpu_head(env);										\
    ret = func(arg1, &env->fp_status);							\
	dadao_fpu_tail(env);										\
	return ret;													\
}

FCVT_1v1(ft2fo, float32_to_float64)
FCVT_1v1(fo2ft, float64_to_float32)

FCVT_1v1(ft2it, float32_to_int32)
FCVT_1v1(ft2io, float32_to_int64)
FCVT_1v1(ft2ut, float32_to_uint32)
FCVT_1v1(ft2uo, float32_to_uint64)
FCVT_1v1(it2ft, int32_to_float32)
FCVT_1v1(io2ft, int64_to_float32)
FCVT_1v1(ut2ft, uint32_to_float32)
FCVT_1v1(uo2ft, uint64_to_float32)

FCVT_1v1(fo2it, float64_to_int32)
FCVT_1v1(fo2io, float64_to_int64)
FCVT_1v1(fo2ut, float64_to_uint32)
FCVT_1v1(fo2uo, float64_to_uint64)
FCVT_1v1(it2fo, int32_to_float64)
FCVT_1v1(io2fo, int64_to_float64)
FCVT_1v1(ut2fo, uint32_to_float64)
FCVT_1v1(uo2fo, uint64_to_float64)

FCVT_1v1(ftsqrt, float32_sqrt)
FCVT_1v1(fosqrt, float64_sqrt)

#undef FCVT_1v1

#define FCAL_s2d1(insn, func)												\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)		\
{																			\
	uint64_t ret;															\
	dadao_fpu_head(env);													\
    ret = func(arg1, arg2, &env->fp_status);								\
	dadao_fpu_tail(env);													\
	return ret;																\
}

FCAL_s2d1(ftadd, float32_add)
FCAL_s2d1(ftsub, float32_sub)
FCAL_s2d1(ftmul, float32_mul)
FCAL_s2d1(ftdiv, float32_div)

FCAL_s2d1(foadd, float64_add)
FCAL_s2d1(fosub, float64_sub)
FCAL_s2d1(fomul, float64_mul)
FCAL_s2d1(fodiv, float64_div)

#undef FCAL_s2d1

#define FCAL_s3d1(insn, func)															\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2, uint64_t arg3)	\
{																						\
	uint64_t ret;																		\
	dadao_fpu_head(env);																\
    ret = func(arg1, arg2, arg3, 0, &env->fp_status);									\
	dadao_fpu_tail(env);																\
	return ret;																			\
}

FCAL_s3d1(ftmadd, float32_muladd)
FCAL_s3d1(fomadd, float64_muladd)

#undef FCAL_s3d1

#define FSGN_s2d1(insn, func, signbit)										\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)		\
{																			\
    return func(arg1, signbit);												\
}

FSGN_s2d1(ftsgnj, float32_set_sign, (arg2 >> 31))
FSGN_s2d1(ftsgnn, float32_set_sign, (arg2 >> 31) ^ 1)

FSGN_s2d1(fosgnj, float64_set_sign, (arg2 >> 63))
FSGN_s2d1(fosgnn, float64_set_sign, (arg2 >> 63) ^ 1)

#undef FSGN_s2d1

#define FCMP_real(insn, cmpfunc, nanval)										\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)			\
{																				\
	dadao_fpu_head(env);														\
	FloatRelation result = cmpfunc(arg1, arg2, &env->fp_status);				\
	dadao_fpu_tail(env);														\
	switch (result) {															\
		case float_relation_greater:		return 1;							\
		case float_relation_equal:			return 0;							\
		case float_relation_less:			return -1;							\
		case float_relation_unordered:		return nanval;						\
		default:							g_assert_not_reached();				\
	}																			\
}

FCMP_real(ftqcmp, float32_compare_quiet, __DDAEE_FLOAT_TETRA_QNAN__)
FCMP_real(ftscmp, float32_compare,       __DDAEE_FLOAT_TETRA_SNAN__)

FCMP_real(foqcmp, float64_compare_quiet, __DDAEE_FLOAT_OCTA_QNAN__)
FCMP_real(foscmp, float64_compare,       __DDAEE_FLOAT_OCTA_SNAN__)

#undef FCMP_real
