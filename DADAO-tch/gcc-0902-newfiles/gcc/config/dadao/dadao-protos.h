/* Prototypes for dadao.c functions used in the md file & header files.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Shi Yining <1700012728@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
extern void  dadao_expand_prologue (void);
extern void  dadao_expand_epilogue (void);
extern bool dadao_offset_address_p (rtx x);
extern int dadao_initial_elimination_offset (int, int);
extern bool dadao_function_arg_regno_p(int);
#ifdef RTX_CODE
extern void dadao_expand_conditional_branch(rtx_code,rtx,rtx,rtx);
#endif
extern bool dadao_debug_func(int);
extern void dadao_init_cumulative_args(CUMULATIVE_ARGS*);
