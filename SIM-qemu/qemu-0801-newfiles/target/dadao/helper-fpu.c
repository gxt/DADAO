#include "qemu/osdep.h"
#include "cpu.h"
#include "qemu/host-utils.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "fpu/softfloat.h"

void HELPER(set_rounding_mode)(CPUDADAOState *env)
{
    int softrm;

    env->frm = env->rf[0] >> 16;

    switch (env->frm) {
    case 0:
	softrm = float_round_nearest_even;
	break;
    case 1:
	softrm = float_round_to_zero;
	break;
    case 2:
	softrm = float_round_down;
	break;
    case 3:
	softrm = float_round_up;
	break;
    }

    set_float_rounding_mode(softrm, &env->fp_status);
}

target_ulong HELPER(ftclass)(CPUDADAOState *env, uint64_t arg1)
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

target_ulong HELPER(foclass)(CPUDADAOState *env, uint64_t arg1)
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

uint64_t HELPER(ft2fo)(CPUDADAOState* env, uint64_t arg1)
{
    return float32_to_float64(arg1, &env->fp_status);
}

uint64_t HELPER(fo2ft)(CPUDADAOState* env, uint64_t arg1)
{
    return float64_to_float32(arg1, &env->fp_status);
}

#define FCVT_1v1(insn, func)									\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1)		\
{																\
    return func(arg1, &env->fp_status);							\
}

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
    return func(arg1, arg2, &env->fp_status);								\
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

uint64_t HELPER(ftabs)(CPUDADAOState* env, uint64_t arg1)
{
    return float32_abs(arg1);
}

uint64_t HELPER(ftneg)(CPUDADAOState* env, uint64_t arg1)
{
    return float32_sub(0, arg1, &env->fp_status);
}

uint64_t HELPER(foabs)(CPUDADAOState* env, uint64_t arg1)
{
    return float64_abs(arg1);
}

uint64_t HELPER(foneg)(CPUDADAOState* env, uint64_t arg1)
{
    return float64_sub(0, arg1, &env->fp_status);
}

uint64_t HELPER(ftcun)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)
{
    FloatRelation result = float32_compare(arg1, arg2, &env->fp_status);
    return result == float_relation_unordered;
}

uint64_t HELPER(ftcor)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)
{
    FloatRelation result = float32_compare(arg1, arg2, &env->fp_status);
    return result != float_relation_unordered;
}

uint64_t HELPER(focun)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)
{
    FloatRelation result = float64_compare(arg1, arg2, &env->fp_status);
    return result == float_relation_unordered;
}

uint64_t HELPER(focor)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)
{
    FloatRelation result = float64_compare(arg1, arg2, &env->fp_status);
    return result != float_relation_unordered;
}

#define FCMP_real(insn, cmpfunc, true_or_false)									\
uint64_t HELPER(insn)(CPUDADAOState* env, uint64_t arg1, uint64_t arg2)		\
{																				\
	FloatRelation result = cmpfunc(arg1, arg2, &env->fp_status);				\
	if (result == float_relation_unordered)		return -1;						\
	return (true_or_false);														\
}

FCMP_real(ftcne, float32_compare, result != float_relation_equal)
FCMP_real(ftceq, float32_compare, result == float_relation_equal)
FCMP_real(ftclt, float32_compare, result == float_relation_less)
FCMP_real(ftcge, float32_compare, result != float_relation_less)
FCMP_real(ftcgt, float32_compare, result == float_relation_greater)
FCMP_real(ftcle, float32_compare, result != float_relation_greater)

FCMP_real(focne, float64_compare, result != float_relation_equal)
FCMP_real(foceq, float64_compare, result == float_relation_equal)
FCMP_real(foclt, float64_compare, result == float_relation_less)
FCMP_real(focge, float64_compare, result != float_relation_less)
FCMP_real(focgt, float64_compare, result == float_relation_greater)
FCMP_real(focle, float64_compare, result != float_relation_greater)

#undef FCMP_real
