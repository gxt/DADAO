/*
 * Helper definitions for DADAO
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2021:
 *  Hao Chenqi <hchenqi@pku.edu.cn>
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

DEF_HELPER_2(ftcls, i64, env, i64)
DEF_HELPER_2(focls, i64, env, i64)
DEF_HELPER_2(ft2fo, i64, env, i64)
DEF_HELPER_2(fo2ft, i64, env, i64)

DEF_HELPER_3(ftadd, i64, env, i64, i64)
DEF_HELPER_3(ftsub, i64, env, i64, i64)
DEF_HELPER_3(ftmul, i64, env, i64, i64)
DEF_HELPER_3(ftdiv, i64, env, i64, i64)
DEF_HELPER_3(ftsgnj, i64, env, i64, i64)
DEF_HELPER_3(ftsgnjn, i64, env, i64, i64)
DEF_HELPER_2(ftsqrt, i64, env, i64)

DEF_HELPER_3(foadd, i64, env, i64, i64)
DEF_HELPER_3(fosub, i64, env, i64, i64)
DEF_HELPER_3(fomul, i64, env, i64, i64)
DEF_HELPER_3(fodiv, i64, env, i64, i64)
DEF_HELPER_3(fosgnj, i64, env, i64, i64)
DEF_HELPER_3(fosgnjn, i64, env, i64, i64)
DEF_HELPER_2(fosqrt, i64, env, i64)

DEF_HELPER_3(ftqcmp, i64, env, i64, i64)
DEF_HELPER_3(ftscmp, i64, env, i64, i64)

DEF_HELPER_3(foqcmp, i64, env, i64, i64)
DEF_HELPER_3(foscmp, i64, env, i64, i64)

DEF_HELPER_2(ft2it, i64, env, i64)
DEF_HELPER_2(ft2io, i64, env, i64)
DEF_HELPER_2(ft2ut, i64, env, i64)
DEF_HELPER_2(ft2uo, i64, env, i64)
DEF_HELPER_2(it2ft, i64, env, i64)
DEF_HELPER_2(io2ft, i64, env, i64)
DEF_HELPER_2(ut2ft, i64, env, i64)
DEF_HELPER_2(uo2ft, i64, env, i64)

DEF_HELPER_2(fo2it, i64, env, i64)
DEF_HELPER_2(fo2io, i64, env, i64)
DEF_HELPER_2(fo2ut, i64, env, i64)
DEF_HELPER_2(fo2uo, i64, env, i64)
DEF_HELPER_2(it2fo, i64, env, i64)
DEF_HELPER_2(io2fo, i64, env, i64)
DEF_HELPER_2(ut2fo, i64, env, i64)
DEF_HELPER_2(uo2fo, i64, env, i64)

DEF_HELPER_4(ftmadd, i64, env, i64, i64, i64)
DEF_HELPER_4(fomadd, i64, env, i64, i64, i64)
