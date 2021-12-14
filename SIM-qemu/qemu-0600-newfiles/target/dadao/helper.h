/*
 * Helper definitions for DADAO
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */

#ifndef CONFIG_USER_ONLY
DEF_HELPER_4(cp0_set, void, env, i32, i32, i32)
DEF_HELPER_3(cp0_get, i32, env, i32, i32)
DEF_HELPER_1(cp1_putc, void, i64)
#endif

DEF_HELPER_2(exception, void, env, i32)

DEF_HELPER_2(seq, i64, i64, i64)
DEF_HELPER_2(sne, i64, i64, i64)
DEF_HELPER_2(slt, i64, i64, i64)
DEF_HELPER_2(sgt, i64, i64, i64)
DEF_HELPER_2(sle, i64, i64, i64)
DEF_HELPER_2(sge, i64, i64, i64)
DEF_HELPER_2(sab, i64, i64, i64)
DEF_HELPER_2(sae, i64, i64, i64)
DEF_HELPER_2(sbl, i64, i64, i64)
DEF_HELPER_2(sbe, i64, i64, i64)
