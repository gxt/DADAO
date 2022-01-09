/*
 * Prototypes for exported functions defined in dadao.c
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* XXX gccint 18.4 Node: Defining data structures for per-function information */
extern void dadao_init_expanders (void);
extern void dd_print_operand_address (FILE *, machine_mode, rtx);
/* XXX gccint 18.9.1 Node: Basic Stack Layout */
extern rtx dadao_dynamic_chain_address (rtx);
extern void dadao_setup_frame_addresses (void);
extern rtx dadao_return_addr_rtx (int, rtx);
/* XXX gccint 18.9.5 Node: Eliminating Frame Pointer and Arg Pointer */
extern int dadao_initial_elimination_offset (int, int);
/* XXX gccint 18.9.12 Node: Generating Code for Profiling */
extern void dadao_function_profiler (FILE *, int);

/* gccint 18.13 Node: Addressing Modes */
extern bool dd_load_legitimate_address_rbzero (rtx);
extern bool dd_load_legitimate_address_rbimm (rtx);
extern bool dd_load_legitimate_address_rbmem (rtx);
extern bool dd_load_legitimate_address_rbrd (rtx);

/* XXX gccint 18.15.2 Node: Representation of condition codes using registers */
#ifdef RTX_CODE
extern machine_mode dadao_select_cc_mode (enum rtx_code, rtx, rtx);
#endif
extern int dadao_reversible_cc_mode (machine_mode);

extern int64_t dadao_intval (const_rtx);
extern int dadao_shiftable_wyde_value (uint64_t);
extern void dadao_output_register_setting (FILE *, int, int64_t, int);
extern int dadao_use_simple_return (void);
extern int dadao_constant_address_p (rtx);
extern void dadao_expand_prologue (void);
extern void dadao_expand_epilogue (void);
extern rtx dadao_get_hard_reg_initial_val (machine_mode, int);

#ifdef RTX_CODE
/* Needs to be ifdef:d for sake of enum rtx_code.  */
extern void dadao_canonicalize_comparison (enum rtx_code *, rtx *, rtx *);
extern rtx dadao_gen_compare_reg (enum rtx_code, rtx, rtx);
#endif

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: t
 * End:
 */
