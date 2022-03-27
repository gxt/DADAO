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

DEF_HELPER_2(ft2fo, i64, env, i64)
DEF_HELPER_2(fo2ft, i64, env, i64)
DEF_HELPER_2(ft2rd, i64, env, i64)
DEF_HELPER_2(fo2rd, i64, env, i64)
DEF_HELPER_2(rd2ft, i64, env, i64)
DEF_HELPER_2(rd2fo, i64, env, i64)

DEF_HELPER_3(ftadd, i64, env, i64, i64)
DEF_HELPER_3(ftsub, i64, env, i64, i64)
DEF_HELPER_3(ftmul, i64, env, i64, i64)
DEF_HELPER_3(ftdiv, i64, env, i64, i64)
DEF_HELPER_2(ftabs, i64, env, i64)
DEF_HELPER_2(ftneg, i64, env, i64)
DEF_HELPER_2(ftsqrt, i64, env, i64)

DEF_HELPER_3(foadd, i64, env, i64, i64)
DEF_HELPER_3(fosub, i64, env, i64, i64)
DEF_HELPER_3(fomul, i64, env, i64, i64)
DEF_HELPER_3(fodiv, i64, env, i64, i64)
DEF_HELPER_2(foabs, i64, env, i64)
DEF_HELPER_2(foneg, i64, env, i64)
DEF_HELPER_2(fosqrt, i64, env, i64)

DEF_HELPER_3(ftcun, i64, env, i64, i64)
DEF_HELPER_3(ftcor, i64, env, i64, i64)
DEF_HELPER_3(ftcne, i64, env, i64, i64)
DEF_HELPER_3(ftceq, i64, env, i64, i64)
DEF_HELPER_3(ftclt, i64, env, i64, i64)
DEF_HELPER_3(ftcge, i64, env, i64, i64)
DEF_HELPER_3(ftcgt, i64, env, i64, i64)
DEF_HELPER_3(ftcle, i64, env, i64, i64)

DEF_HELPER_3(focun, i64, env, i64, i64)
DEF_HELPER_3(focor, i64, env, i64, i64)
DEF_HELPER_3(focne, i64, env, i64, i64)
DEF_HELPER_3(foceq, i64, env, i64, i64)
DEF_HELPER_3(foclt, i64, env, i64, i64)
DEF_HELPER_3(focge, i64, env, i64, i64)
DEF_HELPER_3(focgt, i64, env, i64, i64)
DEF_HELPER_3(focle, i64, env, i64, i64)
