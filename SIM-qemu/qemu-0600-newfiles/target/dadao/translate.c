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
#include "exec/gen-icount.h"

#include "trace-tcg.h"
#include "exec/log.h"

typedef struct DisasContext {
    DisasContextBase base;
    uint32_t mem_idx;
} DisasContext;

/* is_jmp field values */
#define DISAS_JUMP DISAS_TARGET_0 

static TCGv_i64 cpu_rd[64];
static TCGv_i64 cpu_rb[64];
static TCGv_i64 cpu_rf[64];
static TCGv_i64 cpu_ra[64];
static TCGv_i32 cpu_trap_num;

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
    int i;
    for (i = 0; i < 64; i++) {
        cpu_rd[i] = tcg_global_mem_new_i64(cpu_env, 
                                           offsetof(CPUDADAOState, rd[i]), 
                                           regnames[i + 64 * 0]);
    }
    for (i = 0; i < 64; i++) {
        cpu_rb[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rb[i]), 
                                           regnames[i + 64 * 1]);
    }
    for (i = 0; i < 64; i++) {
        cpu_rf[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rf[i]), 
                                           regnames[i + 64 * 2]);
    }
    for (i = 0; i < 64; i++) {
        cpu_ra[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, ra[i]), 
                                           regnames[i + 64 * 3]);
    }
    cpu_trap_num = tcg_global_mem_new_i32(cpu_env,
                                          offsetof(CPUDADAOState, trap_num),
                                          "trap_num");
}

/*
static int times_4(DisasContext *ctx, int x)
{
    return x * 4;
}
*/

bool disas_dadao(DisasContext *ctx, uint32_t insn);

#include "decode-dadao.inc.c"

static void gen_exception(int excp)
{
    TCGv_i32 tmp = tcg_const_i32(excp);
    gen_helper_exception(cpu_env, tmp);
    tcg_temp_free_i32(tmp);
}

/* load store instructions */

static bool trans_ld_all(DisasContext *ctx, arg_rrii *a, 
                         TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha == 0) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rb[a->rb], a->imm);
    tcg_gen_qemu_ld_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_st_all(DisasContext *ctx, arg_rrii *a, 
                         TCGv_i64* cpu_ha, MemOp mop)
{
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rb[a->rb], a->imm);
    tcg_gen_qemu_st_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_ldm_all(DisasContext *ctx, arg_rrri *a,
                          TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha == 0 || a->ha + a->cnt >= 64) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rb[a->rb], cpu_rd[a->rd]);
    tcg_gen_qemu_ld_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    while (a->cnt--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_ld_i64(cpu_ha[++a->ha], addr, ctx->mem_idx, mop);
    }
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_stm_all(DisasContext *ctx, arg_rrri *a,
                          TCGv_i64* cpu_ha, MemOp mop)
{
    if (a->ha + a->cnt >= 64) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rb[a->rb], cpu_rd[a->rd]);
    tcg_gen_qemu_st_i64(cpu_ha[a->ha], addr, ctx->mem_idx, mop);
    while (a->cnt--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_st_i64(cpu_ha[++a->ha], addr, ctx->mem_idx, mop);
    }
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_ldbs(DisasContext *ctx, arg_ldbs *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_SB);
}

static bool trans_ldws(DisasContext *ctx, arg_ldws *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TESW);
}

static bool trans_ldts(DisasContext *ctx, arg_ldts *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TESL);
}

static bool trans_ldo(DisasContext *ctx, arg_ldo *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_ldbu(DisasContext *ctx, arg_ldbu *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_ldwu(DisasContext *ctx, arg_ldwu *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_ldtu(DisasContext *ctx, arg_ldtu *a)
{
    return trans_ld_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_ldrb(DisasContext *ctx, arg_ldrb *a)
{
    return trans_ld_all(ctx, a, cpu_rb, MO_TEQ);
}

static bool trans_ldft(DisasContext *ctx, arg_ldft *a)
{
    return trans_ld_all(ctx, a, cpu_rf, MO_TEUL);
}

static bool trans_ldfo(DisasContext *ctx, arg_ldfo *a)
{
    return trans_ld_all(ctx, a, cpu_rf, MO_TEQ);
}

static bool trans_stb(DisasContext *ctx, arg_stb *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_stw(DisasContext *ctx, arg_stw *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_stt(DisasContext *ctx, arg_stt *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_sto(DisasContext *ctx, arg_sto *a)
{
    return trans_st_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_strb(DisasContext *ctx, arg_strb *a)
{
    return trans_st_all(ctx, a, cpu_rb, MO_TEQ);
}

static bool trans_stft(DisasContext *ctx, arg_stft *a)
{
    return trans_st_all(ctx, a, cpu_rf, MO_TEUL);
}

static bool trans_stfo(DisasContext *ctx, arg_stfo *a)
{
    return trans_st_all(ctx, a, cpu_rf, MO_TEQ);
}

static bool trans_ldmbs(DisasContext *ctx, arg_ldmbs *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_SB);
}

static bool trans_ldmws(DisasContext *ctx, arg_ldmws *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TESW);
}

static bool trans_ldmts(DisasContext *ctx, arg_ldmts *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TESL);
}

static bool trans_ldmo(DisasContext *ctx, arg_ldmo *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_ldmbu(DisasContext *ctx, arg_ldmbu *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_ldmwu(DisasContext *ctx, arg_ldmwu *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_ldmtu(DisasContext *ctx, arg_ldmtu *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_ldmrb(DisasContext *ctx, arg_ldmrb *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_ldmft(DisasContext *ctx, arg_ldmft *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_ldmfo(DisasContext *ctx, arg_ldmfo *a)
{
    return trans_ldm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_ldmra(DisasContext *ctx, arg_ldmra *a)
{
    return trans_ldm_all(ctx, a, cpu_ra, MO_TEQ);
}

static bool trans_stmb(DisasContext *ctx, arg_stmb *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_UB);
}

static bool trans_stmw(DisasContext *ctx, arg_stmw *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUW);
}

static bool trans_stmt(DisasContext *ctx, arg_stmt *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_stmo(DisasContext *ctx, arg_stmo *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_stmrb(DisasContext *ctx, arg_stmrb *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_stmft(DisasContext *ctx, arg_stmft *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEUL);
}

static bool trans_stmfo(DisasContext *ctx, arg_stmfo *a)
{
    return trans_stm_all(ctx, a, cpu_rd, MO_TEQ);
}

static bool trans_stmra(DisasContext *ctx, arg_stmra *a)
{
    return trans_stm_all(ctx, a, cpu_ra, MO_TEQ);
}

/* register assignment instructions */

static bool trans_hb2ha_all(DisasContext* ctx, arg_orri* a,
                            TCGv_i64* cpu_hb, TCGv_i64* cpu_ha) 
{
    if (a->ha == 0 || a->ha + a->cnt >= 64 || a->hb + a->cnt >= 64) {
        return false;
    }
    if (cpu_hb != cpu_ha) {
        while (a->cnt-- >= 0) {
            tcg_gen_mov_i64(cpu_ha[a->ha++], cpu_hb[a->hb++]);
        }
    } else {
        if (a->ha < a->hb) {
            while (a->cnt-- >= 0) {
                tcg_gen_mov_i64(cpu_ha[a->ha++], cpu_hb[a->hb++]);
            }
        } else if (a->ha > a->hb) {
            a->ha += a->cnt; a->hb += a->cnt;
            while (a->cnt-- >= 0) {
                tcg_gen_mov_i64(cpu_ha[a->ha--], cpu_hb[a->hb--]);
            }
        } else {
            /* a->ha == a->hb, do nothing */
        }
    }
    return true;
}

static bool trans_cs_all(DisasContext* ctx, arg_rrrr* a, TCGCond cond)
{
    if (a->rdha == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_movcond_i64(cond, cpu_rd[a->rdha], cpu_rd[a->rdhb], zero,
                        cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_cs_od_ev(DisasContext* ctx, arg_rrrr* a, bool is_od)
{
    if (a->rdha == 0) {
        return false;
    }
    TCGv_i64 bit0 = tcg_const_i64(1);
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_and_i64(bit0, bit0, cpu_rd[a->rdhb]);
    tcg_gen_movcond_i64(is_od ? TCG_COND_NE : TCG_COND_EQ, cpu_rd[a->rdha],
                        bit0, zero, cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_temp_free_i64(bit0);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_rd2rd(DisasContext* ctx, arg_rd2rd* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rd);
}

static bool trans_rd2rb(DisasContext* ctx, arg_rd2rb* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rb);
}

static bool trans_rb2rd(DisasContext* ctx, arg_rb2rd* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rb, cpu_rd);
}

static bool trans_rb2rb(DisasContext* ctx, arg_rb2rb* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rb, cpu_rb);
}

static bool trans_rd2rf(DisasContext* ctx, arg_rd2rf* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_rf);
}

static bool trans_rf2rd(DisasContext* ctx, arg_rf2rd* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rf, cpu_rd);
}

static bool trans_rf2rf(DisasContext* ctx, arg_rf2rf* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rf, cpu_rf);
}

static bool trans_rd2ra(DisasContext* ctx, arg_rd2ra* a)
{
    return trans_hb2ha_all(ctx, a, cpu_rd, cpu_ra);
}

static bool trans_ra2rd(DisasContext* ctx, arg_ra2rd* a)
{
    return trans_hb2ha_all(ctx, a, cpu_ra, cpu_rd);
}

static bool trans_csn(DisasContext *ctx, arg_csn *a)
{
    return trans_cs_all(ctx, a, TCG_COND_LT);
}

static bool trans_csnn(DisasContext *ctx, arg_csnn *a)
{
    return trans_cs_all(ctx, a, TCG_COND_GE);
}

static bool trans_csz(DisasContext *ctx, arg_csz *a)
{
    return trans_cs_all(ctx, a, TCG_COND_EQ);
}

static bool trans_csnz(DisasContext *ctx, arg_csnz *a)
{
    return trans_cs_all(ctx, a, TCG_COND_NE);
}

static bool trans_csp(DisasContext *ctx, arg_csp *a)
{
    return trans_cs_all(ctx, a, TCG_COND_GT);
}

static bool trans_csnp(DisasContext *ctx, arg_csnp *a)
{
    return trans_cs_all(ctx, a, TCG_COND_LE);
}

static bool trans_csod(DisasContext *ctx, arg_csod *a)
{
    return trans_cs_od_ev(ctx, a, true);
}

static bool trans_csev(DisasContext *ctx, arg_csev *a)
{
    return trans_cs_od_ev(ctx, a, false);
}

static bool trans_setow(DisasContext *ctx, arg_setow *a)
{
    if (a->rd == 0) {
        return false;
    }
    int64_t arg = ((int64_t)a->imm << (a->j * 16)) |
                  ~((int64_t)0xFFFF << (a->j * 16));
    tcg_gen_movi_i64(cpu_rd[a->rd], arg);
    return true;
}

static bool trans_setzw(DisasContext *ctx, arg_setzw *a)
{
    if (a->rd == 0) {
        return false;
    }
    int64_t arg = (int64_t)a->imm << (a->j * 16);
    tcg_gen_movi_i64(cpu_rd[a->rd], arg);
    return true;
}

static bool trans_orw(DisasContext *ctx, arg_orw *a)
{
    if (a->rd == 0) {
        return false;
    }
    int64_t arg = (int64_t)a->imm << (a->j * 16);
    tcg_gen_ori_i64(cpu_rd[a->rd], cpu_rd[a->rd], arg);
    return true;
}

static bool trans_andnw(DisasContext *ctx, arg_andnw *a)
{
    if (a->rd == 0) {
        return false;
    }
    int64_t arg = ((int64_t)a->imm << (a->j * 16)) |
                  ~((int64_t)0xFFFF << (a->j * 16));
    int64_t xor = (int64_t)0xFFFF << (a->j * 16);
    tcg_gen_andi_i64(cpu_rd[a->rd], cpu_rd[a->rd], arg);
    tcg_gen_xori_i64(cpu_rd[a->rd], cpu_rd[a->rd], xor);
    return true;
}

/* arithmetic instructions */

static bool trans_addi(DisasContext *ctx, arg_addi *a)
{
    if (a->rd == 0) {
        return false;
    }
    tcg_gen_addi_i64(cpu_rd[a->rd], cpu_rd[a->rd], a->imm);
    return true;
}

static bool trans_addrbi(DisasContext *ctx, arg_addrbi *a)
{
    if (a->rb == 0) {
        return false;
    }
    tcg_gen_addi_i64(cpu_rb[a->rb], cpu_rb[a->rb], a->imm);
    return true;
}

static bool trans_add(DisasContext *ctx, arg_add *a)
{
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_add2_i64(cpu_rd[a->rdhb], cpu_rd[a->rdha],
                     cpu_rd[a->rdhc], zero, cpu_rd[a->rdhd], zero);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_sub(DisasContext *ctx, arg_sub *a)
{
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_sub2_i64(cpu_rd[a->rdhb], cpu_rd[a->rdha],
                     cpu_rd[a->rdhc], zero, cpu_rd[a->rdhd], zero);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_muls(DisasContext *ctx, arg_muls *a)
{
    tcg_gen_muls2_i64(cpu_rd[a->rdhb], cpu_rd[a->rdha],
                      cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_mulu(DisasContext *ctx, arg_mulu *a)
{
    tcg_gen_mulu2_i64(cpu_rd[a->rdhb], cpu_rd[a->rdha],
                      cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_divs(DisasContext *ctx, arg_divs *a)
{
    TCGLabel* label_not_zero = gen_new_label();
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rd[a->rdhd], zero, label_not_zero);
    gen_exception(DADAO_EXCP_INTR);
    gen_set_label(label_not_zero);
    tcg_gen_div_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_gen_rem_i64(cpu_rd[a->rdha], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_divu(DisasContext *ctx, arg_divu *a)
{
    TCGLabel* label_not_zero = gen_new_label();
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rd[a->rdhd], zero, label_not_zero);
    gen_exception(DADAO_EXCP_INTR);
    gen_set_label(label_not_zero);
    tcg_gen_divu_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_gen_remu_i64(cpu_rd[a->rdha], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_cmpsi(DisasContext *ctx, arg_cmpsi *a)
{
    if (a->rdha == 0) {
        return false;
    }
    TCGv_i64 imm = tcg_const_i64(a->imm);
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GT, cpu_rd[a->rdha], cpu_rd[a->rdhb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rd[a->rdha], cpu_rd[a->rdhb], imm,
                        neg, cpu_rd[a->rdha]);
    tcg_temp_free_i64(imm);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmpui(DisasContext *ctx, arg_cmpui *a)
{
    if (a->rdha == 0) {
        return false;
    }
    TCGv_i64 imm = tcg_const_i64(a->imm);
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GTU, cpu_rd[a->rdha], cpu_rd[a->rdhb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->rdha], cpu_rd[a->rdhb], imm,
                        neg, cpu_rd[a->rdha]);
    tcg_temp_free_i64(imm);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmps(DisasContext *ctx, arg_cmps *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GT, cpu_rd[a->rdhb], cpu_rd[a->rdhc],
                        cpu_rd[a->rdhd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rd[a->rdhb], cpu_rd[a->rdhc],
                        cpu_rd[a->rdhd], neg, cpu_rd[a->rdhb]);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmpu(DisasContext *ctx, arg_cmpu *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GTU, cpu_rd[a->rdhb], cpu_rd[a->rdhc],
                        cpu_rd[a->rdhd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->rdhb], cpu_rd[a->rdhc],
                        cpu_rd[a->rdhd], neg, cpu_rd[a->rdhb]);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmprb(DisasContext *ctx, arg_cmprb *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GTU, cpu_rd[a->rdhb], cpu_rd[a->rbhc],
                        cpu_rd[a->rbhd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rd[a->rdhb], cpu_rd[a->rbhc],
                        cpu_rd[a->rbhd], neg, cpu_rd[a->rdhb]);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_addrb(DisasContext *ctx, arg_addrb *a)
{
    tcg_gen_add_i64(cpu_rb[a->rbhb], cpu_rb[a->rbhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_subrb(DisasContext *ctx, arg_subrb *a)
{
    tcg_gen_sub_i64(cpu_rb[a->rbhb], cpu_rb[a->rbhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_and(DisasContext *ctx, arg_and *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    tcg_gen_and_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_orr(DisasContext *ctx, arg_orr *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    tcg_gen_or_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_xor(DisasContext *ctx, arg_xor *a)
{
    if (a->rdhb == 0) {
        return false;
    }
    tcg_gen_xor_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_shlu(DisasContext *ctx, arg_shlu *a)
{
    tcg_gen_shl_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_shrs(DisasContext *ctx, arg_shrs *a)
{
    tcg_gen_sar_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_shru(DisasContext *ctx, arg_shru *a)
{
    tcg_gen_shr_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], cpu_rd[a->rdhd]);
    return true;
}

static bool trans_exts(DisasContext *ctx, arg_exts *a)
{
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_subfi_i64(imm, 63, cpu_rd[a->rdhd]);
    tcg_gen_shl_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], imm);
    tcg_gen_sar_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_extz(DisasContext *ctx, arg_extz *a)
{
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_subfi_i64(imm, 63, cpu_rd[a->rdhd]);
    tcg_gen_shl_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], imm);
    tcg_gen_shr_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_not(DisasContext *ctx, arg_not *a)
{
    TCGv_i64 imm = tcg_const_i64((int64_t)~0);
    tcg_gen_shl_i64(imm, imm, cpu_rd[a->rdhd]);
    tcg_gen_xor_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_shlui(DisasContext *ctx, arg_shlui *a)
{
    tcg_gen_shli_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], a->imm);
    return true;
}

static bool trans_shrsi(DisasContext *ctx, arg_shrsi *a)
{
    tcg_gen_sari_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], a->imm);
    return true;
}

static bool trans_shrui(DisasContext *ctx, arg_shrui *a)
{
    tcg_gen_shri_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], a->imm);
    return true;
}

static bool trans_extsi(DisasContext *ctx, arg_extsi *a)
{
    tcg_gen_shli_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], 63 - a->imm);
    tcg_gen_sari_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], 63 - a->imm);
    return true;
}

static bool trans_extzi(DisasContext *ctx, arg_extzi *a)
{
    tcg_gen_shli_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], 63 - a->imm);
    tcg_gen_shri_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], 63 - a->imm);
    return true;
}

static bool trans_noti(DisasContext *ctx, arg_noti *a)
{
    tcg_gen_xori_i64(cpu_rd[a->rdhb], cpu_rd[a->rdhc], (int64_t)~0 << a->imm);
    return true;
}

/* control flow instructions */

static bool trans_swym(DisasContext *ctx, arg_swym *a)
{
    return true;
}

static bool trans_jump(DisasContext *ctx, arg_jump *a)
{
    tcg_gen_movi_i64(cpu_pc, ctx->base.pc_next + a->imm * 4);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_jumpaa(DisasContext *ctx, arg_jumpaa *a)
{
    tcg_gen_movi_i64(cpu_pc, a->rb);
    tcg_gen_add_i64(cpu_pc, cpu_pc, cpu_rd[a->rd]);
    tcg_gen_addi_i64(cpu_pc, cpu_pc, a->imm * 4);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static void push_return_address(DisasContext *ctx)
{
    TCGv_i64 zero = tcg_const_local_i64(0);
    TCGLabel* reg_ras_not_full = gen_new_label();
    TCGLabel* rasp_set = gen_new_label();
    tcg_gen_brcond_i64(TCG_COND_EQ, cpu_ra[1], zero, reg_ras_not_full);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rasp, zero, rasp_set);
    gen_exception(DADAO_EXCP_TRIP);
    gen_set_label(rasp_set);
    tcg_gen_addi_i64(cpu_rasp, cpu_rasp, -8);
    tcg_gen_qemu_st_i64(cpu_ra[1], cpu_rasp, ctx->mem_idx, MO_TEQ);
    gen_set_label(reg_ras_not_full);
    for (int i = 1; i < 63; ++i) {
        tcg_gen_mov_i64(cpu_ra[i], cpu_ra[i + 1]);
    }
    tcg_gen_movi_i64(cpu_ra[63], ctx->base.pc_next + 4);
    tcg_temp_free_i64(zero);
}

static void pop_return_address(DisasContext *ctx)
{
    TCGv_i64 zero = tcg_const_local_i64(0);
    TCGLabel* reg_ras_not_empty = gen_new_label();
    TCGLabel* rasp_set = gen_new_label();
    TCGLabel* done = gen_new_label();
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_ra[63], zero, reg_ras_not_empty);
    tcg_gen_brcond_i64(TCG_COND_NE, cpu_rasp, zero, rasp_set);
    gen_exception(DADAO_EXCP_TRIP);
    gen_set_label(rasp_set);
    tcg_gen_qemu_ld_i64(cpu_pc, cpu_rasp, ctx->mem_idx, MO_TEQ);
    tcg_gen_addi_i64(cpu_rasp, cpu_rasp, 8);
    tcg_gen_br(done);
    gen_set_label(reg_ras_not_empty);
    tcg_gen_mov_i64(cpu_pc, cpu_ra[63]);
    for (int i = 62; i >= 1; --i) {
        tcg_gen_mov_i64(cpu_ra[i + 1], cpu_ra[i]);
    }
    tcg_gen_mov_i64(cpu_ra[1], zero);
    gen_set_label(done);
    tcg_temp_free_i64(zero);
}

static bool trans_call(DisasContext *ctx, arg_call *a)
{
    push_return_address(ctx);
    return trans_jump(ctx, a);
}

static bool trans_callaa(DisasContext *ctx, arg_callaa *a)
{
    if (a->rb == 0 && a->rd == 0 && a->imm == 0) {
        goto trans_ret;
    }

    push_return_address(ctx);
    return trans_jumpaa(ctx, a);

trans_ret:
    pop_return_address(ctx);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_br_all(DisasContext *ctx, arg_riii *a, TCGCond cond)
{
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 next = tcg_const_i64(ctx->base.pc_next + 4);
    TCGv_i64 dest = tcg_const_i64(ctx->base.pc_next + a->imm * 4);
    tcg_gen_movcond_i64(cond, cpu_pc, cpu_rd[a->rd], zero, dest, next);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(next);
    tcg_temp_free_i64(dest);
    ctx->base.is_jmp = DISAS_JUMP;
    return true;
}

static bool trans_br_od_ev(DisasContext* ctx, arg_riii* a, bool is_od)
{
    TCGv_i64 bit0 = tcg_const_i64(1);
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 next = tcg_const_i64(ctx->base.pc_next + 4);
    TCGv_i64 dest = tcg_const_i64(ctx->base.pc_next + a->imm * 4);
    tcg_gen_and_i64(bit0, bit0, cpu_rd[a->rd]);
    tcg_gen_movcond_i64(is_od ? TCG_COND_NE : TCG_COND_EQ, cpu_pc,
                        bit0, zero, dest, next);
    tcg_temp_free_i64(bit0);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(next);
    tcg_temp_free_i64(dest);
    return true;
}

static bool trans_brn(DisasContext *ctx, arg_brn *a)
{
    return trans_br_all(ctx, a, TCG_COND_LT);
}

static bool trans_brnn(DisasContext *ctx, arg_brnn *a)
{
    return trans_br_all(ctx, a, TCG_COND_GE);
}

static bool trans_brz(DisasContext *ctx, arg_brz *a)
{
    return trans_br_all(ctx, a, TCG_COND_EQ);
}

static bool trans_brnz(DisasContext *ctx, arg_brnz *a)
{
    return trans_br_all(ctx, a, TCG_COND_NE);
}

static bool trans_brp(DisasContext *ctx, arg_brp *a)
{
    return trans_br_all(ctx, a, TCG_COND_GT);
}

static bool trans_brnp(DisasContext *ctx, arg_brnp *a)
{
    return trans_br_all(ctx, a, TCG_COND_LE);
}

static bool trans_brod(DisasContext *ctx, arg_brod *a)
{
    return trans_br_od_ev(ctx, a, true);
}

static bool trans_brev(DisasContext *ctx, arg_brev *a)
{
    return trans_br_od_ev(ctx, a, false);
}

static bool trans_trap(DisasContext *ctx, arg_trap *a)
{
    tcg_gen_movi_i32(cpu_trap_num, a->num);
    gen_exception(DADAO_EXCP_TRAP);
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}

static bool trans_cpcoop(DisasContext *ctx, arg_cpcoop *a)
{
    return false;
}

static bool trans_rd2cr(DisasContext *ctx, arg_rd2cr *a)
{
    return false;
}

static bool trans_cr2rd(DisasContext *ctx, arg_cr2rd *a)
{
    return false;
}

static bool trans_cpload(DisasContext *ctx, arg_cpload *a)
{
    return false;
}

static bool trans_cpstor(DisasContext *ctx, arg_cpstor *a)
{
    return false;
}

static void dadao_tr_init_disas_context(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPUDADAOState *env = cs->env_ptr;
    
    ctx->mem_idx = cpu_mmu_index(env, false);
}

static void dadao_tr_tb_start(DisasContextBase *dcbase, CPUState *cs)
{
}

static void dadao_tr_insn_start(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    tcg_gen_insn_start(ctx->base.pc_next);
}

static bool dadao_tr_breakpoint_check(DisasContextBase *dcbase, CPUState *cs,
                                      const CPUBreakpoint *bp)
{
    return true;
}

static void dadao_tr_translate_insn(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    DADAOCPU *cpu = DADAO_CPU(cs);
    uint32_t insn = translator_ldl(&cpu->env, ctx->base.pc_next);

    tcg_gen_movi_i64(cpu_pc, ctx->base.pc_next);
    
    if (!disas_dadao(ctx, insn)) {
        gen_exception(DADAO_EXCP_ILLI);
        ctx->base.is_jmp = DISAS_NORETURN;
    }

    ctx->base.pc_next += 4;
}

static void dadao_tr_tb_stop(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    if (ctx->base.is_jmp == DISAS_NORETURN) {
        return;
    }

    if (ctx->base.is_jmp == DISAS_JUMP) {
        tcg_gen_lookup_and_goto_ptr();
        return;
    }

    tcg_gen_exit_tb(NULL, 0);
}

static void dadao_tr_disas_log(const DisasContextBase *dcbase, CPUState *cs)
{
}

static const TranslatorOps dadao_tr_ops = {
    .init_disas_context = dadao_tr_init_disas_context,
    .tb_start           = dadao_tr_tb_start,
    .insn_start         = dadao_tr_insn_start,
    .breakpoint_check   = dadao_tr_breakpoint_check,
    .translate_insn     = dadao_tr_translate_insn,
    .tb_stop            = dadao_tr_tb_stop,
    .disas_log          = dadao_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    DisasContext ctx;

    translator_loop(&dadao_tr_ops, &ctx.base, cs, tb, max_insns);
}

void dadao_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    static const char *cpu_mode_names[16] = {
        "USER", "REAL", "INTR", "PRIV", "UM14", "UM15", "UM16", "TRAP",
        "UM18", "UM19", "UM1A", "EXTN", "UM1C", "UM1D", "UM1E", "SUSR"
    };

    DADAOCPU *cpu = DADAO_CPU(cs);
    CPUDADAOState *env = &cpu->env;
    int i;
    uint32_t psr;

    for (i = 0; i < 32; i++) {
        qemu_fprintf(f, "R%02d=%08lx", i, env->rd[i]);
        if ((i % 4) == 3) {
            qemu_fprintf(f, "\n");
        } else {
            qemu_fprintf(f, " ");
        }
    }
    psr = cpu_asr_read(env);
    qemu_fprintf(f, "PSR=%08x %c%c%c%c %s\n",
                 psr,
                 psr & (1 << 31) ? 'N' : '-',
                 psr & (1 << 30) ? 'Z' : '-',
                 psr & (1 << 29) ? 'C' : '-',
                 psr & (1 << 28) ? 'V' : '-',
                 cpu_mode_names[psr & 0xf]);
}

void restore_state_to_opc(CPUDADAOState *env, TranslationBlock *tb,
                          target_ulong *data)
{
    env->REG_PC = data[0];
}
