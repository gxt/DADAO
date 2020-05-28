/*
 * Prototypes for exported functions defined in dadao.c
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

extern void dadao_init_expanders (void);
extern int dadao_eh_return_data_regno (int);
extern int dadao_initial_elimination_offset (int, int);
extern int dadao_function_arg_regno_p (int, int);
extern void dadao_function_profiler (FILE *, int);
extern int dadao_reversible_cc_mode (machine_mode);
extern const char *dadao_text_section_asm_op (void);
extern const char *dadao_data_section_asm_op (void);
extern void dadao_output_quoted_string (FILE *, const char *, int);
extern void dadao_asm_output_label (FILE *, const char *);
extern void dadao_asm_output_internal_label (FILE *, const char *);
extern void dadao_asm_weaken_label (FILE *, const char *);
extern void dadao_asm_output_labelref (FILE *, const char *);
extern void dadao_asm_output_def (FILE *, const char *, const char *);
extern void dadao_asm_output_reg_push (FILE *, int);
extern void dadao_asm_output_reg_pop (FILE *, int);
extern void dadao_asm_output_skip (FILE *, int);
extern void dadao_asm_output_align (FILE *, int);
extern int64_t dadao_intval (const_rtx);
extern int dadao_shiftable_wyde_value (uint64_t);
extern void dadao_output_register_setting (FILE *, int, int64_t, int);
extern int dadao_opposite_regno (int, int);
extern unsigned dadao_dbx_register_number (unsigned);
extern int dadao_use_simple_return (void);
extern void dadao_make_decl_one_only (tree);
extern void dadao_asm_output_pool_prologue (FILE *, const char *, tree, int);
extern void dadao_asm_output_aligned_common (FILE *, const char *, int, int);
extern void dadao_asm_output_aligned_local (FILE *, const char *, int, int);
extern void dadao_asm_declare_register_global
  (FILE *, tree, int, const char *);
extern void dadao_asm_output_addr_diff_elt (FILE *, rtx, int, int);
extern void dadao_asm_output_addr_vec_elt (FILE *, int);
extern enum reg_class dadao_secondary_reload_class
  (enum reg_class, machine_mode, rtx, int);
extern rtx dadao_dynamic_chain_address (rtx);
extern rtx dadao_return_addr_rtx (int, rtx);
extern rtx dadao_eh_return_stackadj_rtx (void);
extern rtx dadao_eh_return_handler_rtx (void);
extern int dadao_constant_address_p (rtx);
extern void dadao_expand_prologue (void);
extern void dadao_expand_epilogue (void);
extern rtx dadao_get_hard_reg_initial_val (machine_mode, int);
extern int dadao_asm_preferred_eh_data_format (int, int);
extern void dadao_setup_frame_addresses (void);

#ifdef RTX_CODE
/* Needs to be ifdef:d for sake of enum rtx_code.  */
extern machine_mode dadao_select_cc_mode (enum rtx_code, rtx, rtx);
extern void dadao_canonicalize_comparison (enum rtx_code *, rtx *, rtx *);
extern rtx dadao_gen_compare_reg (enum rtx_code, rtx, rtx);
#endif

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: t
 * End:
 */
