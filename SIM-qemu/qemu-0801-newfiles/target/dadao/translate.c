/*
 * DADAO Translation
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2021:
 *  Hao Chenqi <hchenqi@pku.edu.cn>
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 *
 * Function hierachy:
 *   QEMU Runtime
 *          |
 *   gen_intermediate_code
 *          |
 *   disas_dadao_insn
 *          |
 *   decode tree
 *          |
 *   tcg_gen_...
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "disas/disas.h"
#include "tcg/tcg-op.h"
#include "qemu/log.h"
#include "qemu/bitops.h"
#include "qemu/qemu-print.h"
#include "exec/cpu_ldst.h"
#include "exec/translator.h"

#include "exec/helper-proto.h"
#include "exec/helper-gen.h"

#include "exec/log.h"

#define HELPER_H "helper.h"
#include "exec/helper-info.c.inc"
#undef  HELPER_H

typedef struct DisasContext {
    DisasContextBase base;
    uint32_t mem_idx;

    /* If not -1, jmp_pc contains this value and so is a direct jump.  */
    uint64_t jmp_pc_imm;
} DisasContext;

/* is_jmp field values */
#define DISAS_JUMP DISAS_TARGET_0 

static TCGv_i64 cpu_rd[64];
static TCGv_i64 cpu_rb[64];
static TCGv_i64 cpu_rf[64];
static TCGv_i64 cpu_ra[64];
static TCGv_i64 jmp_pc;
static TCGv_i64 load_res;
static TCGv_i64 load_val;

#define cpu_pc   cpu_rb[0]
#define cpu_rasp cpu_ra[0]

static const char *regnames[] = {
    "rd00", "rd01", "rd02", "rd03", "rd04", "rd05", "rd06", "rd07",
    "rd08", "rd09", "rd10", "rd11", "rd12", "rd13", "rd14", "rd15",
    "rd16", "rd17", "rd18", "rd19", "rd20", "rd21", "rd22", "rd23",
    "rd24", "rd25", "rd26", "rd27", "rd28", "rd29", "rd30", "rd31",
    "rd32", "rd33", "rd34", "rd35", "rd36", "rd37", "rd38", "rd39",
    "rd40", "rd41", "rd42", "rd43", "rd44", "rd45", "rd46", "rd47",
    "rd48", "rd49", "rd50", "rd51", "rd52", "rd53", "rd54", "rd55",
    "rd56", "rd57", "rd58", "rd59", "rd60", "rd61", "rd62", "rd63",

    "rb00", "rb01", "rb02", "rb03", "rb04", "rb05", "rb06", "rb07",
    "rb08", "rb09", "rb10", "rb11", "rb12", "rb13", "rb14", "rb15",
    "rb16", "rb17", "rb18", "rb19", "rb20", "rb21", "rb22", "rb23",
    "rb24", "rb25", "rb26", "rb27", "rb28", "rb29", "rb30", "rb31",
    "rb32", "rb33", "rb34", "rb35", "rb36", "rb37", "rb38", "rb39",
    "rb40", "rb41", "rb42", "rb43", "rb44", "rb45", "rb46", "rb47",
    "rb48", "rb49", "rb50", "rb51", "rb52", "rb53", "rb54", "rb55",
    "rb56", "rb57", "rb58", "rb59", "rb60", "rb61", "rb62", "rb63",

    "rf00", "rf01", "rf02", "rf03", "rf04", "rf05", "rf06", "rf07",
    "rf08", "rf09", "rf10", "rf11", "rf12", "rf13", "rf14", "rf15",
    "rf16", "rf17", "rf18", "rf19", "rf20", "rf21", "rf22", "rf23",
    "rf24", "rf25", "rf26", "rf27", "rf28", "rf29", "rf30", "rf31",
    "rf32", "rf33", "rf34", "rf35", "rf36", "rf37", "rf38", "rf39",
    "rf40", "rf41", "rf42", "rf43", "rf44", "rf45", "rf46", "rf47",
    "rf48", "rf49", "rf50", "rf51", "rf52", "rf53", "rf54", "rf55",
    "rf56", "rf57", "rf58", "rf59", "rf60", "rf61", "rf62", "rf63",

    "ra00", "ra01", "ra02", "ra03", "ra04", "ra05", "ra06", "ra07",
    "ra08", "ra09", "ra10", "ra11", "ra12", "ra13", "ra14", "ra15",
    "ra16", "ra17", "ra18", "ra19", "ra20", "ra21", "ra22", "ra23",
    "ra24", "ra25", "ra26", "ra27", "ra28", "ra29", "ra30", "ra31",
    "ra32", "ra33", "ra34", "ra35", "ra36", "ra37", "ra38", "ra39",
    "ra40", "ra41", "ra42", "ra43", "ra44", "ra45", "ra46", "ra47",
    "ra48", "ra49", "ra50", "ra51", "ra52", "ra53", "ra54", "ra55",
    "ra56", "ra57", "ra58", "ra59", "ra60", "ra61", "ra62", "ra63",
};

/* Map TCG globals to CPU context.  */
void dadao_translate_init(void)
{
    for (int i = 0; i < 64; i++) {
        cpu_rd[i] = tcg_global_mem_new_i64(cpu_env, 
                                           offsetof(CPUDADAOState, rd[i]), 
                                           regnames[i + 64 * 0]);
    }
    for (int i = 0; i < 64; i++) {
        cpu_rb[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rb[i]), 
                                           regnames[i + 64 * 1]);
    }
    for (int i = 0; i < 64; i++) {
        cpu_rf[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rf[i]), 
                                           regnames[i + 64 * 2]);
    }
    for (int i = 0; i < 64; i++) {
        cpu_ra[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, ra[i]), 
                                           regnames[i + 64 * 3]);
    }

    jmp_pc = tcg_global_mem_new_i64(cpu_env,
                                     offsetof(CPUDADAOState, jmp_pc),
                                     "jmp_pc");
    load_res = tcg_global_mem_new_i64(cpu_env,
                                     offsetof(CPUDADAOState, load_res),
                                     "load_res");
    load_val = tcg_global_mem_new_i64(cpu_env,
                                     offsetof(CPUDADAOState, load_val),
                                     "load_val");
}

bool disas_dadao(DisasContext *ctx, uint32_t insn);

#include "decode-insns.c.inc"

static void gen_exception(int excp)
{
    TCGv_i32 tmp = tcg_constant_i32(excp);
    gen_helper_exception(cpu_env, tmp);
}

/* load store instructions */

static bool trans_ld_all(DisasContext *ctx, arg_disas_dadao0 *a, 
                         TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha == 0 && cpu_ha != cpu_rf) {
        return true;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rb[a->hb], a->imms12);
    tcg_gen_qemu_ld_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    return true;
}

static bool trans_st_all(DisasContext *ctx, arg_disas_dadao0 *a, 
                         TCGv_i64* cpu_ha, MemOp mop)
{
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rb[a->hb], a->imms12);
    tcg_gen_qemu_st_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    return true;
}

static bool trans_ldm_all(DisasContext *ctx, arg_disas_dadao1 *a,
                          TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha + a->immu6 > 64 || a->immu6 == 0) {
        return false;
    }
    if (a->ha == 0 && a->immu6 == 1 && cpu_ha != cpu_rf) {
        return true;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rb[a->hb], cpu_rd[a->hc]);
    if (a->ha == 0 && cpu_ha != cpu_rf) {
        a->ha++;
	a->immu6--;
	tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
    }
    tcg_gen_qemu_ld_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    a->immu6--;
    while (a->immu6--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_ld_i64(cpu_ha[++a->ha], addr, ctx->mem_idx, mop);
    }
    return true;
}

static bool trans_stm_all(DisasContext *ctx, arg_disas_dadao1 *a,
                          TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha + a->immu6 > 64 || a->immu6 == 0) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rb[a->hb], cpu_rd[a->hc]);
    tcg_gen_qemu_st_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    a->immu6--;
    while (a->immu6--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_st_i64(cpu_ha[++a->ha], addr, ctx->mem_idx, mop);
    }
    return true;
}

static bool trans_LDBS(DisasContext *ctx, arg_LDBS *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_SB);
}

static bool trans_LDWS(DisasContext *ctx, arg_LDWS *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TESW);
}

static bool trans_LDTS(DisasContext *ctx, arg_LDTS *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TESL);
}

static bool trans_LDO(DisasContext *ctx, arg_LDO *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_LDBU(DisasContext *ctx, arg_LDBU *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_LDWU(DisasContext *ctx, arg_LDWU *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_LDTU(DisasContext *ctx, arg_LDTU *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_LDRB(DisasContext *ctx, arg_LDRB *a)
{
    return trans_ld_all(ctx, a, cpu_rb, MO_TEUQ);
}

static bool trans_LDFT(DisasContext *ctx, arg_LDFT *a)
{
    return trans_ld_all(ctx, a, cpu_rf, MO_TEUL);
}

static bool trans_LDFO(DisasContext *ctx, arg_LDFO *a)
{
    return trans_ld_all(ctx, a, cpu_rf, MO_TEUQ);
}

static bool trans_STB(DisasContext *ctx, arg_STB *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_STW(DisasContext *ctx, arg_STW *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_STT(DisasContext *ctx, arg_STT *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_STO(DisasContext *ctx, arg_STO *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_STRB(DisasContext *ctx, arg_STRB *a)
{
    return trans_st_all(ctx, a, cpu_rb, MO_TEUQ);
}

static bool trans_STFT(DisasContext *ctx, arg_STFT *a)
{
    return trans_st_all(ctx, a, cpu_rf, MO_TEUL);
}

static bool trans_STFO(DisasContext *ctx, arg_STFO *a)
{
    return trans_st_all(ctx, a, cpu_rf, MO_TEUQ);
}

static bool trans_LDMBS(DisasContext *ctx, arg_LDMBS *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_SB);
}

static bool trans_LDMWS(DisasContext *ctx, arg_LDMWS *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TESW);
}

static bool trans_LDMTS(DisasContext *ctx, arg_LDMTS *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TESL);
}

static bool trans_LDMO(DisasContext *ctx, arg_LDMO *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_LDMBU(DisasContext *ctx, arg_LDMBU *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_LDMWU(DisasContext *ctx, arg_LDMWU *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_LDMTU(DisasContext *ctx, arg_LDMTU *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_LDMRB(DisasContext *ctx, arg_LDMRB *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_LDMFT(DisasContext *ctx, arg_LDMFT *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_LDMFO(DisasContext *ctx, arg_LDMFO *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_LDMRA(DisasContext *ctx, arg_LDMRA *a)
{
    return trans_ldm_all(ctx, a, cpu_ra, MO_TEUQ);
}

static bool trans_STMB(DisasContext *ctx, arg_STMB *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_STMW(DisasContext *ctx, arg_STMW *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_STMT(DisasContext *ctx, arg_STMT *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_STMO(DisasContext *ctx, arg_STMO *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_STMRB(DisasContext *ctx, arg_STMRB *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_STMFT(DisasContext *ctx, arg_STMFT *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_STMFO(DisasContext *ctx, arg_STMFO *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUQ);
}

static bool trans_STMRA(DisasContext *ctx, arg_STMRA *a)
{
    return trans_stm_all(ctx, a, cpu_ra, MO_TEUQ);
}

/* register assignment instructions */

static bool trans_hb2ha_all(DisasContext* ctx, arg_disas_dadao2* a,
                            TCGv_i64* cpu_hb, TCGv_i64* cpu_ha) 
{
    if (a->hb + a->immu6 > 64 || a->hc + a->immu6 > 64 || a->immu6 == 0) {
        return false;
    }
    if (a->hb == 0 && a->immu6 == 1 && cpu_ha != cpu_rf) {
        return true;
    }
    if (a->hb == 0 && cpu_ha != cpu_rf) {
        a->hb++;
	a->hc++;
	a->immu6--;
    }
    if (cpu_hb != cpu_ha) {
        while (a->immu6--) {
            tcg_gen_mov_i64(cpu_ha[a->hb++], cpu_hb[a->hc++]);
        }
    } else {
        if (a->hb < a->hc) {
            while (a->immu6--) {
                tcg_gen_mov_i64(cpu_ha[a->hb++], cpu_hb[a->hc++]);
            }
        } else if (a->hb > a->hc) {
            a->hb += a->immu6; a->hc += a->immu6;
	    a->hb--; a->hc--;
            while (a->immu6--) {
                tcg_gen_mov_i64(cpu_ha[a->hb--], cpu_hb[a->hc--]);
            }
        } else {
            /* a->hb == a->hc, do nothing */
        }
    }
    return true;
}

static bool trans_cs_all(DisasContext* ctx, arg_disas_dadao3* a, TCGCond cond)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 zero = tcg_constant_i64(0);
    tcg_gen_movcond_i64(cond, cpu_rd[a->hb], cpu_rd[a->ha], zero,
                        cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_cs_eq_ne(DisasContext* ctx, arg_disas_dadao3* a, TCGCond cond)
{
    if (a->hc == 0) {
        return true;
    }
    tcg_gen_movcond_i64(cond, cpu_rd[a->hc], cpu_rd[a->ha], cpu_rd[a->hb],
                        cpu_rd[a->hd], cpu_rd[a->hc]);
    return true;
}

static bool trans_RD2RD(DisasContext* ctx, arg_RD2RD* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rd);
}

static bool trans_RD2RB(DisasContext* ctx, arg_RD2RB* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rb);
}

static bool trans_RB2RD(DisasContext* ctx, arg_RB2RD* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rb, cpu_rd);
}

static bool trans_RB2RB(DisasContext* ctx, arg_RB2RB* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rb, cpu_rb);
}

static bool trans_RD2RF(DisasContext* ctx, arg_RD2RF* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rf);
}

static bool trans_RF2RD(DisasContext* ctx, arg_RF2RD* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rf, cpu_rd);
}

static bool trans_RF2RF(DisasContext* ctx, arg_RF2RF* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rf, cpu_rf);
}

static bool trans_RD2RA(DisasContext* ctx, arg_RD2RA* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_ra);
}

static bool trans_RA2RD(DisasContext* ctx, arg_RA2RD* a)
{
    return trans_hb2ha_all(ctx, a, cpu_ra, cpu_rd);
}

static bool trans_CSN(DisasContext *ctx, arg_CSN *a)
{
    return trans_cs_all(ctx, a, TCG_COND_LT);
}

static bool trans_CSZ(DisasContext *ctx, arg_CSZ *a)
{
    return trans_cs_all(ctx, a, TCG_COND_EQ);
}

static bool trans_CSP(DisasContext *ctx, arg_CSP *a)
{
    return trans_cs_all(ctx, a, TCG_COND_GT);
}

static bool trans_CSEQ(DisasContext *ctx, arg_CSEQ *a)
{
    return trans_cs_eq_ne(ctx, a, TCG_COND_EQ);
}

static bool trans_CSNE(DisasContext *ctx, arg_CSNE *a)
{
    return trans_cs_eq_ne(ctx, a, TCG_COND_NE);
}

static bool trans_ORWrd(DisasContext *ctx, arg_ORWrd *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = (int64_t)a->immu16 << (a->ww * 16);
    tcg_gen_ori_i64(cpu_rd[a->ha], cpu_rd[a->ha], arg);
    return true;
}

static bool trans_ANDNWrd(DisasContext *ctx, arg_ANDNWrd *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = ~((int64_t)a->immu16 << (a->ww * 16));
    tcg_gen_andi_i64(cpu_rd[a->ha], cpu_rd[a->ha], arg);
    return true;
}

static bool trans_SETZWrd(DisasContext *ctx, arg_SETZWrd *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = (int64_t)a->immu16 << (a->ww * 16);
    tcg_gen_movi_i64(cpu_rd[a->ha], arg);
    return true;
}

static bool trans_SETOW(DisasContext *ctx, arg_SETOW *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = ((int64_t)a->immu16 << (a->ww * 16)) |
                  ~((int64_t)0xFFFF << (a->ww * 16));
    tcg_gen_movi_i64(cpu_rd[a->ha], arg);
    return true;
}

static bool trans_ORWrb(DisasContext *ctx, arg_ORWrb *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = (int64_t)a->immu16 << (a->ww * 16);
    tcg_gen_ori_i64(cpu_rb[a->ha], cpu_rb[a->ha], arg);
    return true;
}

static bool trans_ANDNWrb(DisasContext *ctx, arg_ANDNWrb *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = ~((int64_t)a->immu16 << (a->ww * 16));
    tcg_gen_andi_i64(cpu_rb[a->ha], cpu_rb[a->ha], arg);
    return true;
}

static bool trans_SETZWrb(DisasContext *ctx, arg_SETZWrb *a)
{
    if (a->ha == 0) {
        return true;
    }
    int64_t arg = (int64_t)a->immu16 << (a->ww * 16);
    tcg_gen_movi_i64(cpu_rb[a->ha], arg);
    return true;
}

static bool trans_SETW(DisasContext *ctx, arg_SETW *a)
{
    int64_t mask = ~((int64_t)0xFFFF << (a->ww * 16));
    int64_t arg = (int64_t)a->immu16 << (a->ww * 16);
    tcg_gen_andi_i64(cpu_rf[a->ha], cpu_rf[a->ha], mask);
    tcg_gen_ori_i64(cpu_rf[a->ha], cpu_rf[a->ha], arg);
    return true;
}

/* logic instructions */

static bool trans_ANDI(DisasContext *ctx, arg_ANDI *a)
{
    if (a->ha == 0) {
        return true;
    }
    tcg_gen_andi_i64(cpu_rd[a->ha], cpu_rd[a->hb], a->immu12);
    return true;
}

/* arithmetic instructions */

static bool trans_ADDIrd(DisasContext *ctx, arg_ADDIrd *a)
{
    if (a->ha == 0) {
        return true;
    }
    tcg_gen_addi_i64(cpu_rd[a->ha], cpu_rd[a->hb], a->imms12);
    return true;
}

static bool trans_ADDIrb(DisasContext *ctx, arg_ADDIrb *a)
{
    if (a->ha == 0) {
        return true;
    }
    tcg_gen_addi_i64(cpu_rb[a->ha], cpu_rb[a->hb], a->imms12);
    return true;
}

static bool trans_ADRP(DisasContext *ctx, arg_ADRP *a)
{
    if (a->ha == 0) {
        return true;
    }
    tcg_gen_movi_i64(cpu_rb[a->ha], ctx->base.pc_next);
    tcg_gen_andi_i64(cpu_rb[a->ha], cpu_rb[a->ha], ~(int64_t)0xFFF);
    tcg_gen_addi_i64(cpu_rb[a->ha], cpu_rb[a->ha], a->imms18 << 12);
    return true;
}

static bool trans_ADDrd(DisasContext *ctx, arg_ADDrd *a)
{
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 flag = tcg_constant_i64(0);
    TCGv_i64 negative = tcg_constant_i64(-1);
    int64_t is_negative = 0x8000000000000000;
    tcg_gen_add2_i64(cpu_rd[a->hb], cpu_rd[a->ha],
                     cpu_rd[a->hc], zero, cpu_rd[a->hd], zero);
    tcg_gen_movcond_i64(TCG_COND_EQ, cpu_rd[a->ha], cpu_rd[a->ha], zero,
                        zero, negative);
    tcg_gen_andi_i64(flag, cpu_rd[a->hb], is_negative);
    tcg_gen_movcond_i64(TCG_COND_EQ, cpu_rd[a->ha], flag, zero,
                        zero, negative);
    if (a->ha == 0) {
        tcg_gen_movi_i64(cpu_rd[a->ha], 0);
    }
    if (a->hb == 0) {
        tcg_gen_movi_i64(cpu_rd[a->hb], 0);
    }
    return true;
}

static bool trans_SUBrd(DisasContext *ctx, arg_SUBrd *a)
{
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 flag = tcg_constant_i64(0);
    TCGv_i64 negative = tcg_constant_i64(-1);
    int64_t is_negative = 0x8000000000000000;
    tcg_gen_sub2_i64(cpu_rd[a->hb], cpu_rd[a->ha],
                     cpu_rd[a->hc], zero, cpu_rd[a->hd], zero);
    tcg_gen_movcond_i64(TCG_COND_EQ, cpu_rd[a->ha], cpu_rd[a->ha], zero,
                        zero, negative);
    tcg_gen_andi_i64(flag, cpu_rd[a->hb], is_negative);
    tcg_gen_movcond_i64(TCG_COND_EQ, cpu_rd[a->ha], flag, zero,
                        zero, negative);
    if (a->ha == 0) {
        tcg_gen_movi_i64(cpu_rd[a->ha], 0);
    }
    if (a->hb == 0) {
        tcg_gen_movi_i64(cpu_rd[a->hb], 0);
    }
    return true;
}

static bool trans_MULS(DisasContext *ctx, arg_MULS *a)
{
    tcg_gen_muls2_i64(cpu_rd[a->hb], cpu_rd[a->ha],
                      cpu_rd[a->hc], cpu_rd[a->hd]);
    if (a->ha == 0) {
        tcg_gen_movi_i64(cpu_rd[a->ha], 0);
    }
    if (a->hb == 0) {
        tcg_gen_movi_i64(cpu_rd[a->hb], 0);
    }
    return true;
}

static bool trans_MULU(DisasContext *ctx, arg_MULU *a)
{
    tcg_gen_mulu2_i64(cpu_rd[a->hb], cpu_rd[a->ha],
                      cpu_rd[a->hc], cpu_rd[a->hd]);
    if (a->ha == 0) {
        tcg_gen_movi_i64(cpu_rd[a->ha], 0);
    }
    if (a->hb == 0) {
        tcg_gen_movi_i64(cpu_rd[a->hb], 0);
    }
    return true;
}

static bool trans_DIVS(DisasContext *ctx, arg_DIVS *a)
{
    TCGLabel* label_not_zero = gen_new_label();
    TCGv_i64 zero = tcg_constant_i64(0);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rd[a->hd], zero, label_not_zero);
    gen_exception(DADAO_EXCP_FPER);
    gen_set_label(label_not_zero);
    TCGv_i64 temp1 = tcg_temp_new_i64();
    TCGv_i64 temp2 = tcg_temp_new_i64();
    tcg_gen_div_i64(temp1, cpu_rd[a->hc], cpu_rd[a->hd]);
    tcg_gen_rem_i64(temp2, cpu_rd[a->hc], cpu_rd[a->hd]);
    if (a->hb != 0) {
        tcg_gen_mov_i64(cpu_rd[a->hb], temp1);
    }
    if (a->ha != 0) {
        tcg_gen_mov_i64(cpu_rd[a->ha], temp2);
    }
    return true;
}

static bool trans_DIVU(DisasContext *ctx, arg_DIVU *a)
{
    TCGLabel* label_not_zero = gen_new_label();
    TCGv_i64 zero = tcg_constant_i64(0);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rd[a->hd], zero, label_not_zero);
    gen_exception(DADAO_EXCP_FPER);
    gen_set_label(label_not_zero);
    TCGv_i64 temp1 = tcg_temp_new_i64();
    TCGv_i64 temp2 = tcg_temp_new_i64();
    tcg_gen_divu_i64(temp1, cpu_rd[a->hc], cpu_rd[a->hd]);
    tcg_gen_remu_i64(temp2, cpu_rd[a->hc], cpu_rd[a->hd]);
    if (a->hb != 0) {
        tcg_gen_mov_i64(cpu_rd[a->hb], temp1);
    }
    if (a->ha != 0) {
        tcg_gen_mov_i64(cpu_rd[a->ha], temp2);
    }
    return true;
}

static bool trans_CMPSi(DisasContext *ctx, arg_CMPSi *a)
{
    if (a->ha == 0) {
        return true;
    }
    TCGv_i64 imm = tcg_constant_i64(a->imms12);
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 pos = tcg_constant_i64(1);
    TCGv_i64 neg = tcg_constant_i64(-1);
    TCGv_i64 temp = tcg_temp_new_i64();
    tcg_gen_movcond_i64(TCG_COND_GT, temp, cpu_rd[a->hb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rd[a->ha], cpu_rd[a->hb], imm,
                        neg, temp);
    return true;
}

static bool trans_CMPUi(DisasContext *ctx, arg_CMPUi *a)
{
    if (a->ha == 0) {
        return true;
    }
    TCGv_i64 imm = tcg_constant_i64(a->immu12);
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 pos = tcg_constant_i64(1);
    TCGv_i64 neg = tcg_constant_i64(-1);
    TCGv_i64 temp = tcg_temp_new_i64();
    tcg_gen_movcond_i64(TCG_COND_GTU, temp, cpu_rd[a->hb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->ha], cpu_rd[a->hb], imm,
                        neg, temp);
    return true;
}

static bool trans_CMPSr(DisasContext *ctx, arg_CMPSr *a)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 pos = tcg_constant_i64(1);
    TCGv_i64 neg = tcg_constant_i64(-1);
    TCGv_i64 temp = tcg_temp_new_i64();
    tcg_gen_movcond_i64(TCG_COND_GT, temp, cpu_rd[a->hc],
                        cpu_rd[a->hd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rd[a->hb], cpu_rd[a->hc],
                        cpu_rd[a->hd], neg, temp);
    return true;
}

static bool trans_CMPUr(DisasContext *ctx, arg_CMPUr *a)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 pos = tcg_constant_i64(1);
    TCGv_i64 neg = tcg_constant_i64(-1);
    TCGv_i64 temp = tcg_temp_new_i64();
    tcg_gen_movcond_i64(TCG_COND_GTU, temp, cpu_rd[a->hc],
                        cpu_rd[a->hd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->hb], cpu_rd[a->hc],
                        cpu_rd[a->hd], neg, temp);
    return true;
}

static bool trans_CMP(DisasContext *ctx, arg_CMP *a)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 pos = tcg_constant_i64(1);
    TCGv_i64 neg = tcg_constant_i64(-1);
    TCGv_i64 temp = tcg_temp_new_i64();
    tcg_gen_movcond_i64(TCG_COND_GTU, temp, cpu_rb[a->hc],
                        cpu_rb[a->hd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->hb], cpu_rb[a->hc],
                        cpu_rb[a->hd], neg, temp);
    return true;
}

static bool trans_ADDrb(DisasContext *ctx, arg_ADDrb *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_add_i64(cpu_rb[a->hb], cpu_rb[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_SUBrb(DisasContext *ctx, arg_SUBrb *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_sub_i64(cpu_rb[a->hb], cpu_rb[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_AND(DisasContext *ctx, arg_AND *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_and_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_ORR(DisasContext *ctx, arg_ORR *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_or_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_XOR(DisasContext *ctx, arg_XOR *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_xor_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_SHLUr(DisasContext *ctx, arg_SHLUr *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shl_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_SHRSr(DisasContext *ctx, arg_SHRSr *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_sar_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_SHRUr(DisasContext *ctx, arg_SHRUr *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shr_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    return true;
}

static bool trans_EXTSr(DisasContext *ctx, arg_EXTSr *a)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_add_i64(imm, cpu_rd[0], cpu_rd[a->hd]);
    tcg_gen_shl_i64(cpu_rd[a->hb], cpu_rd[a->hc], imm);
    tcg_gen_sar_i64(cpu_rd[a->hb], cpu_rd[a->hb], imm);
    return true;
}

static bool trans_EXTZr(DisasContext *ctx, arg_EXTZr *a)
{
    if (a->hb == 0) {
        return true;
    }
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_add_i64(imm, cpu_rd[0], cpu_rd[a->hd]);
    tcg_gen_shl_i64(cpu_rd[a->hb], cpu_rd[a->hc], imm);
    tcg_gen_shr_i64(cpu_rd[a->hb], cpu_rd[a->hb], imm);
    return true;
}

static bool trans_XNOR(DisasContext *ctx, arg_XNOR *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_xor_i64(cpu_rd[a->hb], cpu_rd[a->hc], cpu_rd[a->hd]);
    tcg_gen_xori_i64(cpu_rd[a->hb], cpu_rd[a->hb], 0xffffffffffffffff);
    return true;
}

static bool trans_SHLUi(DisasContext *ctx, arg_SHLUi *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shli_i64(cpu_rd[a->hb], cpu_rd[a->hc], a->immu6);
    return true;
}

static bool trans_SHRSi(DisasContext *ctx, arg_SHRSi *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_sari_i64(cpu_rd[a->hb], cpu_rd[a->hc], a->immu6);
    return true;
}

static bool trans_SHRUi(DisasContext *ctx, arg_SHRUi *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shri_i64(cpu_rd[a->hb], cpu_rd[a->hc], a->immu6);
    return true;
}

static bool trans_EXTSi(DisasContext *ctx, arg_EXTSi *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shli_i64(cpu_rd[a->hb], cpu_rd[a->hc], a->immu6);
    tcg_gen_sari_i64(cpu_rd[a->hb], cpu_rd[a->hb], a->immu6);
    return true;
}

static bool trans_EXTZi(DisasContext *ctx, arg_EXTZi *a)
{
    if (a->hb == 0) {
        return true;
    }
    tcg_gen_shli_i64(cpu_rd[a->hb], cpu_rd[a->hc], a->immu6);
    tcg_gen_shri_i64(cpu_rd[a->hb], cpu_rd[a->hb], a->immu6);
    return true;
}

static bool trans_fcvt_all(DisasContext *ctx, arg_disas_dadao2 *a,
                           TCGv_i64* cpu_hb, TCGv_i64* cpu_hc,
                           void (*fn)(TCGv_i64, TCGv_env, TCGv_i64))
{
    gen_helper_set_rounding_mode(cpu_env);
    if (a->hb + a->immu6 > 64 || a->hc + a->immu6 > 64 || a->immu6 == 0) {
        return false;
    }
    if (a->hb == 0 && a->immu6 == 1 && cpu_hb != cpu_rf) {
        return true;
    }
    if (a->hb == 0 && cpu_hb != cpu_rf) {
        a->hb++;
	a->hc++;
	a->immu6--;
    }
    if (a->hb <= a->hc) {
        while (a->immu6--) {
            fn(cpu_hb[a->hb++], cpu_env, cpu_hc[a->hc++]);
        }
    } else {
        a->hb += a->immu6; a->hc += a->immu6;
	a->hb--; a->hc--;
        while (a->immu6--) {
            fn(cpu_hb[a->hb--], cpu_env, cpu_hc[a->hc--]);
        }
    }
    return true;
}

static bool trans_fop1_all(DisasContext *ctx, arg_disas_dadao2 *a,
                           void (*fn)(TCGv_i64, TCGv_env, TCGv_i64))
{
    gen_helper_set_rounding_mode(cpu_env);
    fn(cpu_rf[a->hb], cpu_env, cpu_rf[a->hc]);
    return true;
}

static bool trans_fop2_all(DisasContext *ctx, arg_disas_dadao7 *a,
                           void (*fn)(TCGv_i64, TCGv_env, TCGv_i64, TCGv_i64))
{
    gen_helper_set_rounding_mode(cpu_env);
    fn(cpu_rf[a->hb], cpu_env, cpu_rf[a->hc], cpu_rf[a->hd]);
    return true;
}

static bool trans_fcmp_all(DisasContext *ctx, arg_disas_dadao7 *a,
                           void (*fn)(TCGv_i64, TCGv_env, TCGv_i64, TCGv_i64))
{
    fn(cpu_rd[a->hb], cpu_env, cpu_rf[a->hc], cpu_rf[a->hd]);
    return true;
}

static bool trans_FT2FO(DisasContext *ctx, arg_FT2FO *a)
{
    return trans_fcvt_all(ctx, a, cpu_rf, cpu_rf, gen_helper_ft2fo);
}

static bool trans_FO2FT(DisasContext *ctx, arg_FO2FT *a)
{
    return trans_fcvt_all(ctx, a, cpu_rf, cpu_rf, gen_helper_fo2ft);
}

static bool trans_FT2RD(DisasContext *ctx, arg_FT2RD *a)
{
    return trans_fcvt_all(ctx, a, cpu_rd, cpu_rf, gen_helper_ft2rd);
}

static bool trans_FO2RD(DisasContext *ctx, arg_FO2RD *a)
{
    return trans_fcvt_all(ctx, a, cpu_rd, cpu_rf, gen_helper_fo2rd);
}

static bool trans_RD2FT(DisasContext *ctx, arg_RD2FT *a)
{
    return trans_fcvt_all(ctx, a, cpu_rf, cpu_rd, gen_helper_rd2ft);
}

static bool trans_RD2FO(DisasContext *ctx, arg_RD2FO *a)
{
    return trans_fcvt_all(ctx, a, cpu_rf, cpu_rd, gen_helper_rd2fo);
}

static bool trans_FTADD(DisasContext *ctx, arg_FTADD *a)
{
    return trans_fop2_all(ctx, a, gen_helper_ftadd);
}

static bool trans_FTSUB(DisasContext *ctx, arg_FTSUB *a)
{
    return trans_fop2_all(ctx, a, gen_helper_ftsub);
}

static bool trans_FTMUL(DisasContext *ctx, arg_FTMUL *a)
{
    return trans_fop2_all(ctx, a, gen_helper_ftmul);
}

static bool trans_FTDIV (DisasContext *ctx, arg_FTDIV *a)
{
    return trans_fop2_all(ctx, a, gen_helper_ftdiv);
}

static bool trans_FTABS(DisasContext *ctx, arg_FTABS *a)
{
    return trans_fop1_all(ctx, a, gen_helper_ftabs);
}

static bool trans_FTNEG(DisasContext *ctx, arg_FTNEG *a)
{
    return trans_fop1_all(ctx, a, gen_helper_ftneg);
}

static bool trans_FTSQRT(DisasContext *ctx, arg_FTSQRT *a)
{
    return trans_fop1_all(ctx, a, gen_helper_ftsqrt);
}

static bool trans_FOADD(DisasContext *ctx, arg_FOADD *a)
{
    return trans_fop2_all(ctx, a, gen_helper_foadd);
}

static bool trans_FOSUB(DisasContext *ctx, arg_FOSUB *a)
{
    return trans_fop2_all(ctx, a, gen_helper_fosub);
}

static bool trans_FOMUL(DisasContext *ctx, arg_FOMUL *a)
{
    return trans_fop2_all(ctx, a, gen_helper_fomul);
}

static bool trans_FODIV(DisasContext *ctx, arg_FODIV *a)
{
    return trans_fop2_all(ctx, a, gen_helper_fodiv);
}

static bool trans_FOABS(DisasContext *ctx, arg_FOABS *a)
{
    return trans_fop1_all(ctx, a, gen_helper_foabs);
}

static bool trans_FONEG (DisasContext *ctx, arg_FONEG  *a)
{
    return trans_fop1_all(ctx, a, gen_helper_foneg);
}

static bool trans_FOSQRT(DisasContext *ctx, arg_FOSQRT *a)
{
    return trans_fop1_all(ctx, a, gen_helper_fosqrt);
}

static bool trans_FTCUN(DisasContext *ctx, arg_FTCUN *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcun);
}

static bool trans_FTCOR(DisasContext *ctx, arg_FTCOR *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcor);
}

static bool trans_FTCNE(DisasContext *ctx, arg_FTCNE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcne);
}

static bool trans_FTCEQ(DisasContext *ctx, arg_FTCEQ *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftceq);
}

static bool trans_FTCLT(DisasContext *ctx, arg_FTCLT *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftclt);
}

static bool trans_FTCGE(DisasContext *ctx, arg_FTCGE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcge);
}

static bool trans_FTCGT(DisasContext *ctx, arg_FTCGT *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcgt);
}

static bool trans_FTCLE(DisasContext *ctx, arg_FTCLE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_ftcle);
}

static bool trans_FOCUN(DisasContext *ctx, arg_FOCUN *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focun);
}

static bool trans_FOCOR(DisasContext *ctx, arg_FOCOR *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focor);
}

static bool trans_FOCNE(DisasContext *ctx, arg_FOCNE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focne);
}

static bool trans_FOCEQ(DisasContext *ctx, arg_FOCEQ *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_foceq);
}

static bool trans_FOCLT(DisasContext *ctx, arg_FOCLT *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_foclt);
}

static bool trans_FOCGE(DisasContext *ctx, arg_FOCGE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focge);
}

static bool trans_FOCGT(DisasContext *ctx, arg_FOCGT *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focgt);
}

static bool trans_FOCLE(DisasContext *ctx, arg_FOCLE *a)
{
    return trans_fcmp_all(ctx, a, gen_helper_focle);
}

/* control flow instructions */

static bool trans_SWYM(DisasContext *ctx, arg_SWYM *a)
{
    return true;
}

static bool trans_UNIMP(DisasContext *ctx, arg_UNIMP *a)
{
    return false;
}

static bool trans_JUMPi(DisasContext *ctx, arg_JUMPi *a)
{
    ctx->base.is_jmp = DISAS_JUMP;
    ctx->jmp_pc_imm = ctx->base.pc_next + a->imms24 * 4;
    return true;
}

static bool trans_JUMPr(DisasContext *ctx, arg_JUMPr *a)
{
    tcg_gen_mov_i64(jmp_pc, cpu_rb[a->ha]);
    tcg_gen_add_i64(jmp_pc, jmp_pc, cpu_rd[a->hb]);
    tcg_gen_addi_i64(jmp_pc, jmp_pc, a->imms12 * 4);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static void push_return_address(DisasContext *ctx)
{
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGLabel* reg_ras_not_full = gen_new_label();
    TCGLabel* rasp_set = gen_new_label();
    tcg_gen_brcond_i64(TCG_COND_EQ, cpu_ra[1], zero, reg_ras_not_full);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rasp, zero, rasp_set);
    gen_exception(DADAO_EXCP_TRIP);
    gen_set_label(rasp_set);
    tcg_gen_addi_i64(cpu_rasp, cpu_rasp, -8);
    tcg_gen_qemu_st_i64(cpu_ra[1], cpu_rasp, ctx->mem_idx, MO_TEUQ);
    gen_set_label(reg_ras_not_full);
    for (int i = 1; i < 63; ++i) {
        tcg_gen_mov_i64(cpu_ra[i], cpu_ra[i + 1]);
    }
    tcg_gen_movi_i64(cpu_ra[63], ctx->base.pc_next + 4);
}

static void pop_return_address(DisasContext *ctx)
{
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGLabel* reg_ras_not_empty = gen_new_label();
    TCGLabel* rasp_set = gen_new_label();
    TCGLabel* done = gen_new_label();
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_ra[63], zero, reg_ras_not_empty);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rasp, zero, rasp_set);
    gen_exception(DADAO_EXCP_TRIP);
    gen_set_label(rasp_set);
    tcg_gen_qemu_ld_i64(jmp_pc, cpu_rasp, ctx->mem_idx, MO_TEUQ);
    tcg_gen_addi_i64(cpu_rasp, cpu_rasp, 8);
    tcg_gen_br(done);
    gen_set_label(reg_ras_not_empty);
    tcg_gen_mov_i64(jmp_pc, cpu_ra[63]);
    for (int i = 62; i >= 1; --i) {
        tcg_gen_mov_i64(cpu_ra[i + 1], cpu_ra[i]);
    }
    tcg_gen_mov_i64(cpu_ra[1], zero);
    gen_set_label(done);
}

static bool trans_CALLi(DisasContext *ctx, arg_CALLi *a)
{
    push_return_address(ctx);
    return trans_JUMPi(ctx, a);
}

static bool trans_CALLr(DisasContext *ctx, arg_CALLr *a)
{
    push_return_address(ctx);
    return trans_JUMPr(ctx, a);
}

static bool trans_RET(DisasContext *ctx, arg_RET *a)
{
    if (a->ha != 0)
        tcg_gen_movi_i64(cpu_rd[a->ha], a->imms18);
    pop_return_address(ctx);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_br_all(DisasContext *ctx, arg_disas_dadao5 *a, TCGCond cond)
{
    TCGv_i64 zero = tcg_constant_i64(0);
    TCGv_i64 next = tcg_constant_i64(ctx->base.pc_next + 4);
    TCGv_i64 dest = tcg_constant_i64(ctx->base.pc_next + a->imms18 * 4);
    tcg_gen_movcond_i64(cond, jmp_pc, cpu_rd[a->ha], zero, dest, next);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_br_eq_ne(DisasContext* ctx, arg_disas_dadao0* a, TCGCond cond)
{
    TCGv_i64 next = tcg_constant_i64(ctx->base.pc_next + 4);
    TCGv_i64 dest = tcg_constant_i64(ctx->base.pc_next + a->imms12 * 4);
    tcg_gen_movcond_i64(cond, jmp_pc, cpu_rd[a->ha], cpu_rd[a->hb], dest, next);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_BRN(DisasContext *ctx, arg_BRN *a)
{
    return trans_br_all(ctx, a, TCG_COND_LT);
}

static bool trans_BRNN(DisasContext *ctx, arg_BRNN *a)
{
    return trans_br_all(ctx, a, TCG_COND_GE);
}

static bool trans_BRZ(DisasContext *ctx, arg_BRZ *a)
{
    return trans_br_all(ctx, a, TCG_COND_EQ);
}

static bool trans_BRNZ(DisasContext *ctx, arg_BRNZ *a)
{
    return trans_br_all(ctx, a, TCG_COND_NE);
}

static bool trans_BRP(DisasContext *ctx, arg_BRP *a)
{
    return trans_br_all(ctx, a, TCG_COND_GT);
}

static bool trans_BRNP(DisasContext *ctx, arg_BRNP *a)
{
    return trans_br_all(ctx, a, TCG_COND_LE);
}

static bool trans_BREQ(DisasContext *ctx, arg_BREQ *a)
{
    return trans_br_eq_ne(ctx, a, TCG_COND_EQ);
}

static bool trans_BRNE(DisasContext *ctx, arg_BRNE *a)
{
    return trans_br_eq_ne(ctx, a, TCG_COND_NE);
}

static bool trans_TRAP(DisasContext *ctx, arg_TRAP *a)
{
    if (a->ha != 0 || a->immu18 != 0) {
        return false;
    }
    gen_exception(DADAO_EXCP_TRAP);
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}

static bool trans_ESCAPE(DisasContext *ctx, arg_ESCAPE *a)
{
    return false;
}

static bool trans_CPCO(DisasContext *ctx, arg_CPCO *a)
{
    return false;
}

static bool trans_CPRD(DisasContext *ctx, arg_CPRD *a)
{
    return false;
}

static bool trans_CPWR(DisasContext *ctx, arg_CPWR *a)
{
    return false;
}

static bool trans_CPLD(DisasContext *ctx, arg_CPLD *a)
{
    return false;
}

static bool trans_CPST(DisasContext *ctx, arg_CPST *a)
{
    return false;
}

/* AMO instructions */

static bool trans_FENCE(DisasContext *ctx, arg_FENCE *a)
{
    return true;
}

static bool trans_LRO_NN(DisasContext *ctx, arg_LRO_NN *a)
{
    if ((a->hd == 0) || (a->hb != 0))	return false;
    if (a->hc == 0)	return true;
    TCGv_i64 src = tcg_temp_new_i64();

    /* Put addr in load_res, data in load_val. */
    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_qemu_ld_i64(load_val, src, ctx->mem_idx, MO_TEUQ);
    tcg_gen_mov_i64(load_res, src);
    tcg_gen_mov_i64(cpu_rd[a->hc], load_val);

    return true;
}

static bool trans_LRO_NR(DisasContext *ctx, arg_LRO_NR *a)
{
    if ((a->hd == 0) || (a->hb != 0))	return false;
    if (a->hc == 0)     return true;
    TCGv_i64 src = tcg_temp_new_i64();

    /* Put addr in load_res, data in load_val. */
    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_STRL);
    tcg_gen_qemu_ld_i64(load_val, src, ctx->mem_idx, MO_TEUQ);
    tcg_gen_mov_i64(load_res, src);
    tcg_gen_mov_i64(cpu_rd[a->hc], load_val);

    return true;
}

static bool trans_LRO_AN(DisasContext *ctx, arg_LRO_AN *a)
{
    if ((a->hd == 0) || (a->hb != 0))	return false;
    if (a->hc == 0)     return true;
    TCGv_i64 src = tcg_temp_new_i64();

    /* Put addr in load_res, data in load_val. */
    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_qemu_ld_i64(load_val, src, ctx->mem_idx, MO_TEUQ);
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_LDAQ);
    tcg_gen_mov_i64(load_res, src);
    tcg_gen_mov_i64(cpu_rd[a->hc], load_val);

    return true;
}

static bool trans_LRO_AR(DisasContext *ctx, arg_LRO_AR *a)
{
    if ((a->hd == 0) || (a->hb != 0))	return false;
    if (a->hc == 0)     return true;
    TCGv_i64 src = tcg_temp_new_i64();

    /* Put addr in load_res, data in load_val. */
    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_STRL);
    tcg_gen_qemu_ld_i64(load_val, src, ctx->mem_idx, MO_TEUQ);
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_LDAQ);
    tcg_gen_mov_i64(load_res, src);
    tcg_gen_mov_i64(cpu_rd[a->hc], load_val);

    return true;
}

static bool trans_SCO_NN(DisasContext *ctx, arg_SCO_NN *a)
{
    if ((a->hd == 0) || (a->hb == 0))	return false;
    TCGv_i64 src = tcg_temp_new_i64();
    TCGv_i64 dat = tcg_temp_new_i64();
    TCGLabel *l1 = gen_new_label();
    TCGLabel *l2 = gen_new_label();

    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_brcond_i64(TCG_COND_NE, load_res, src, l1);

    tcg_gen_atomic_cmpxchg_i64(src, load_res, load_val, cpu_rd[a->hc],
                               ctx->mem_idx, MO_TEUQ);

    tcg_gen_setcond_i64(TCG_COND_NE, dat, src, load_val);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);
    tcg_gen_br(l2);

    gen_set_label(l1);

    tcg_gen_mb(TCG_MO_ALL);
    tcg_gen_movi_i64(dat, 1);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);

    gen_set_label(l2);
    tcg_gen_movi_i64(load_res, -1);

    return true;
}

static bool trans_SCO_NR(DisasContext *ctx, arg_SCO_NR *a)
{
    if ((a->hd == 0) || (a->hb == 0))	return false;
    TCGv_i64 src = tcg_temp_new_i64();
    TCGv_i64 dat = tcg_temp_new_i64();
    TCGLabel *l1 = gen_new_label();
    TCGLabel *l2 = gen_new_label();

    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_brcond_i64(TCG_COND_NE, load_res, src, l1);

    tcg_gen_atomic_cmpxchg_i64(src, load_res, load_val, cpu_rd[a->hc],
                               ctx->mem_idx, MO_TEUQ);

    tcg_gen_setcond_i64(TCG_COND_NE, dat, src, load_val);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);
    tcg_gen_br(l2);

    gen_set_label(l1);

    tcg_gen_mb(TCG_MO_ALL + TCG_BAR_STRL);
    tcg_gen_movi_i64(dat, 1);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);

    gen_set_label(l2);
    tcg_gen_movi_i64(load_res, -1);

    return true;
}

static bool trans_SCO_AN(DisasContext *ctx, arg_SCO_AN *a)
{
    if ((a->hd == 0) || (a->hb == 0))	return false;
    TCGv_i64 src = tcg_temp_new_i64();
    TCGv_i64 dat = tcg_temp_new_i64();
    TCGLabel *l1 = gen_new_label();
    TCGLabel *l2 = gen_new_label();

    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_brcond_i64(TCG_COND_NE, load_res, src, l1);

    tcg_gen_atomic_cmpxchg_i64(src, load_res, load_val, cpu_rd[a->hc],
                               ctx->mem_idx, MO_TEUQ);

    tcg_gen_setcond_i64(TCG_COND_NE, dat, src, load_val);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);
    tcg_gen_br(l2);

    gen_set_label(l1);

    tcg_gen_mb(TCG_MO_ALL + TCG_BAR_LDAQ);
    tcg_gen_movi_i64(dat, 1);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);

    gen_set_label(l2);
    tcg_gen_movi_i64(load_res, -1);

    return true;
}

static bool trans_SCO_AR(DisasContext *ctx, arg_SCO_AR *a)
{
    if ((a->hd == 0) || (a->hb == 0))	return false;
    TCGv_i64 src = tcg_temp_new_i64();
    TCGv_i64 dat = tcg_temp_new_i64();
    TCGLabel *l1 = gen_new_label();
    TCGLabel *l2 = gen_new_label();

    tcg_gen_mov_i64(src, cpu_rb[a->hd]);
    tcg_gen_brcond_i64(TCG_COND_NE, load_res, src, l1);

    tcg_gen_atomic_cmpxchg_i64(src, load_res, load_val, cpu_rd[a->hc],
                               ctx->mem_idx, MO_TEUQ);

    tcg_gen_setcond_i64(TCG_COND_NE, dat, src, load_val);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);
    tcg_gen_br(l2);

    gen_set_label(l1);

    tcg_gen_mb(TCG_MO_ALL + TCG_BAR_LDAQ + TCG_BAR_STRL);
    tcg_gen_movi_i64(dat, 1);
    tcg_gen_mov_i64(cpu_rd[a->hb], dat);

    gen_set_label(l2);
    tcg_gen_movi_i64(load_res, -1);

    return true;
}

static void dadao_tr_init_disas_context(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPUDADAOState *env = cs->env_ptr;
    
    ctx->mem_idx = cpu_mmu_index(env, false);
    ctx->jmp_pc_imm = -1;
}

static void dadao_tr_tb_start(DisasContextBase *dcbase, CPUState *cs)
{
}

static void dadao_tr_insn_start(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    tcg_gen_insn_start(ctx->base.pc_next);
}

static void dadao_tr_translate_insn(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    DADAOCPU *cpu = DADAO_CPU(cs);
    uint32_t insn = translator_ldl(&cpu->env, &ctx->base, ctx->base.pc_next);

    if (!disas_dadao(ctx, insn)) {
        gen_exception(DADAO_EXCP_ILLI);
        ctx->base.is_jmp = DISAS_NORETURN;
        return;
    }

    if (ctx->base.is_jmp == DISAS_NEXT) {
        ctx->base.pc_next += 4;
        tcg_gen_movi_i64(cpu_pc, ctx->base.pc_next);
    }
}

static void dadao_tr_tb_stop(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    target_ulong jmp_dest;

    /* For DISAS_TOO_MANY, jump to the next insn.  */
    jmp_dest = ctx->base.pc_next;

    switch (ctx->base.is_jmp) {
    case DISAS_NORETURN:
        break;
    case DISAS_JUMP:
        jmp_dest = ctx->jmp_pc_imm;
        if (jmp_dest == -1) {
            /* The jump destination is indirect/computed; use jmp_pc.  */
            tcg_gen_mov_tl(cpu_pc, jmp_pc);
            tcg_gen_discard_tl(jmp_pc);
            tcg_gen_lookup_and_goto_ptr();
            break;
        }
        /* The jump destination is direct; use jmp_pc_imm.
           However, we will have stored into jmp_pc as well;
           we know now that it wasn't needed.  */
        tcg_gen_discard_tl(jmp_pc);
        /* fall through */

    case DISAS_TOO_MANY:
        if (translator_use_goto_tb(&ctx->base, jmp_dest)) {
            tcg_gen_goto_tb(0);
            tcg_gen_movi_tl(cpu_pc, jmp_dest);
            tcg_gen_exit_tb(ctx->base.tb, 0);
            break;
        }
        tcg_gen_movi_tl(cpu_pc, jmp_dest);
        tcg_gen_lookup_and_goto_ptr();
        break;
    default:
        g_assert_not_reached();
    }
}

static void dadao_tr_disas_log(const DisasContextBase *dcbase, CPUState *cs, FILE *logfile)
{
    DisasContext *dc = container_of(dcbase, DisasContext, base);

    fprintf(logfile, "IN: %s\n", lookup_symbol(dc->base.pc_first));
    target_disas(logfile, cs, dc->base.pc_first, dc->base.tb->size);
}

static const TranslatorOps dadao_tr_ops = {
    .init_disas_context = dadao_tr_init_disas_context,
    .tb_start           = dadao_tr_tb_start,
    .insn_start         = dadao_tr_insn_start,
    .translate_insn     = dadao_tr_translate_insn,
    .tb_stop            = dadao_tr_tb_stop,
    .disas_log          = dadao_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int *max_insns,
                           target_ulong pc, void *host_pc)
{
    DisasContext ctx;

    translator_loop(cs, tb, max_insns, pc, host_pc,
                    &dadao_tr_ops, &ctx.base);
}

void dadao_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    DADAOCPU *cpu = DADAO_CPU(cs);
    CPUDADAOState *env = &cpu->env;
    for (int i = 0; i < 64; i += 4) {
        if (env->rd[i + 0] || env->rd[i + 1] || env->rd[i + 2] || env->rd[i + 3]) {
            qemu_log("[RD%02d]=%016lx ", i + 0, env->rd[i + 0]);
            qemu_log("[RD%02d]=%016lx ", i + 1, env->rd[i + 1]);
            qemu_log("[RD%02d]=%016lx ", i + 2, env->rd[i + 2]);
            qemu_log("[RD%02d]=%016lx ", i + 3, env->rd[i + 3]);
            qemu_log("\n");
        }
    }
    for (int i = 0; i < 64; i += 4) {
        if (env->rb[i + 0] || env->rb[i + 1] || env->rb[i + 2] || env->rb[i + 3]) {
            qemu_log("[RB%02d]=%016lx ", i + 0, env->rb[i + 0]);
            qemu_log("[RB%02d]=%016lx ", i + 1, env->rb[i + 1]);
            qemu_log("[RB%02d]=%016lx ", i + 2, env->rb[i + 2]);
            qemu_log("[RB%02d]=%016lx ", i + 3, env->rb[i + 3]);
            qemu_log("\n");
        }
    }
    for (int i = 0; i < 64; i += 4) {
        if (env->rf[i + 0] || env->rf[i + 1] || env->rf[i + 2] || env->rf[i + 3]) {
            qemu_log("[RF%02d]=%016lx ", i + 0, env->rf[i + 0]);
            qemu_log("[RF%02d]=%016lx ", i + 1, env->rf[i + 1]);
            qemu_log("[RF%02d]=%016lx ", i + 2, env->rf[i + 2]);
            qemu_log("[RF%02d]=%016lx ", i + 3, env->rf[i + 3]);
            qemu_log("\n");
        }
    }
    for (int i = 0; i < 64; i += 4) {
        if (env->ra[i + 0] || env->ra[i + 1] || env->ra[i + 2] || env->ra[i + 3]) {
            qemu_log("[RA%02d]=%016lx ", i + 0, env->ra[i + 0]);
            qemu_log("[RA%02d]=%016lx ", i + 1, env->ra[i + 1]);
            qemu_log("[RA%02d]=%016lx ", i + 2, env->ra[i + 2]);
            qemu_log("[RA%02d]=%016lx ", i + 3, env->ra[i + 3]);
            qemu_log("\n");
        }
    }
    qemu_log("\n");
}

