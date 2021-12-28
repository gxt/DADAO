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

static TCGv_i64 cpu_rg[64];
static TCGv_i64 cpu_rp[64];
static TCGv_i64 cpu_rf[64];
static TCGv_i64 cpu_rr[64];
static TCGv_i32 cpu_trap_num;

#define cpu_pc cpu_rp[0]

static const char *regnames[] = {
    "zero", "rg01", "rg02", "rg03", "rg04", "rg05", "rg06", "rg07",
    "rg08", "rg09", "rg10", "rg11", "rg12", "rg13", "rg14", "rg15",
    "rg16", "rg17", "rg18", "rg19", "rg20", "rg21", "rg22", "rg23",
    "rg24", "rg25", "rg26", "rg27", "rg28", "rg29", "rg30", "rg31",
    "rg32", "rg33", "rg34", "rg35", "rg36", "rg37", "rg38", "rg39",
    "rg40", "rg41", "rg42", "rg43", "rg44", "rg45", "rg46", "rg47",
    "rg48", "rg49", "rg50", "rg51", "rg52", "rg53", "rg54", "rg55",
    "rg56", "rg57", "rg58", "rg59", "rg60", "rg61", "rg62", "rg63",

    "rp00", "rp01", "rp02", "rp03", "rp04", "rp05", "rp06", "rp07",
    "rp08", "rp09", "rp10", "rp11", "rp12", "rp13", "rp14", "rp15",
    "rp16", "rp17", "rp18", "rp19", "rp20", "rp21", "rp22", "rp23",
    "rp24", "rp25", "rp26", "rp27", "rp28", "rp29", "rp30", "rp31",
    "rp32", "rp33", "rp34", "rp35", "rp36", "rp37", "rp38", "rp39",
    "rp40", "rp41", "rp42", "rp43", "rp44", "rp45", "rp46", "rp47",
    "rp48", "rp49", "rp50", "rp51", "rp52", "rp53", "rp54", "rp55",
    "rp56", "rp57", "rp58", "rp59", "rp60", "rp61", "rp62", "rp63",

    "rf00", "rf01", "rf02", "rf03", "rf04", "rf05", "rf06", "rf07",
    "rf08", "rf09", "rf10", "rf11", "rf12", "rf13", "rf14", "rf15",
    "rf16", "rf17", "rf18", "rf19", "rf20", "rf21", "rf22", "rf23",
    "rf24", "rf25", "rf26", "rf27", "rf28", "rf29", "rf30", "rf31",
    "rf32", "rf33", "rf34", "rf35", "rf36", "rf37", "rf38", "rf39",
    "rf40", "rf41", "rf42", "rf43", "rf44", "rf45", "rf46", "rf47",
    "rf48", "rf49", "rf50", "rf51", "rf52", "rf53", "rf54", "rf55",
    "rf56", "rf57", "rf58", "rf59", "rf60", "rf61", "rf62", "rf63",

    "rr00", "rr01", "rr02", "rr03", "rr04", "rr05", "rr06", "rr07",
    "rr08", "rr09", "rr10", "rr11", "rr12", "rr13", "rr14", "rr15",
    "rr16", "rr17", "rr18", "rr19", "rr20", "rr21", "rr22", "rr23",
    "rr24", "rr25", "rr26", "rr27", "rr28", "rr29", "rr30", "rr31",
    "rr32", "rr33", "rr34", "rr35", "rr36", "rr37", "rr38", "rr39",
    "rr40", "rr41", "rr42", "rr43", "rr44", "rr45", "rr46", "rr47",
    "rr48", "rr49", "rr50", "rr51", "rr52", "rr53", "rr54", "rr55",
    "rr56", "rr57", "rr58", "rr59", "rr60", "rr61", "rr62", "rr63",
};

/* Map TCG globals to CPU context.  */
void dadao_translate_init(void)
{
    int i;
    for (i = 0; i < 64; i++) {
        cpu_rg[i] = tcg_global_mem_new_i64(cpu_env, 
                                           offsetof(CPUDADAOState, rg[i]), 
                                           regnames[i + 64 * 0]);
    }
    for (i = 0; i < 64; i++) {
        cpu_rp[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rp[i]), 
                                           regnames[i + 64 * 1]);
    }
    for (i = 0; i < 64; i++) {
        cpu_rf[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rf[i]), 
                                           regnames[i + 64 * 2]);
    }
    for (i = 0; i < 64; i++) {
        cpu_rr[i] = tcg_global_mem_new_i64(cpu_env,
                                           offsetof(CPUDADAOState, rr[i]), 
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
                         TCGv_i64* cpu_ra, MemOp mop)
{
    if (a->ra == 0) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rp[a->rp], a->imm);
    tcg_gen_qemu_ld_i64(cpu_ra[a->ra], addr, ctx->mem_idx, mop);
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_st_all(DisasContext *ctx, arg_rrii *a, 
                         TCGv_i64* cpu_ra, MemOp mop)
{
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_addi_i64(addr, cpu_rp[a->rp], a->imm);
    tcg_gen_qemu_st_i64(cpu_ra[a->ra], addr, ctx->mem_idx, mop);
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_ldm_all(DisasContext *ctx, arg_rrri *a,
                          TCGv_i64* cpu_ra, MemOp mop)
{
    if (a->ra == 0 || a->ra + a->cnt >= 64) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rp[a->rp], cpu_rg[a->rg]);
    tcg_gen_qemu_ld64(cpu_ra[a->ra], addr, ctx->mem_idx);
    while (a->cnt--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_ld_i64(cpu_ra[++a->ra], addr, ctx->mem_idx, mop);
    }
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_stm_all(DisasContext *ctx, arg_rrri *a,
                          TCGv_i64* cpu_ra, MemOp mop)
{
    if (a->ra + a->cnt >= 64) {
        return false;
    }
    TCGv_i64 addr = tcg_temp_new_i64();
    tcg_gen_add_i64(addr, cpu_rp[a->rp], cpu_rg[a->rg]);
    tcg_gen_qemu_st64(cpu_ra[a->ra], addr, ctx->mem_idx);
    while (a->cnt--) {
        tcg_gen_addi_i64(addr, addr, 1 << (mop & 3));
        tcg_gen_qemu_st_i64(cpu_ra[++a->ra], addr, ctx->mem_idx, mop);
    }
    tcg_temp_free_i64(addr);
    return true;
}

static bool trans_ldbs(DisasContext *ctx, arg_ldbs *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_SB);
}

static bool trans_ldws(DisasContext *ctx, arg_ldws *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_TESW);
}

static bool trans_ldts(DisasContext *ctx, arg_ldts *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_TESL);
}

static bool trans_ldo(DisasContext *ctx, arg_ldo *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_ldbu(DisasContext *ctx, arg_ldbu *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_UB);
}

static bool trans_ldwu(DisasContext *ctx, arg_ldwu *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_TEUW);
}

static bool trans_ldtu(DisasContext *ctx, arg_ldtu *a)
{
    return trans_ld_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_ldrp(DisasContext *ctx, arg_ldrp *a)
{
    return trans_ld_all(ctx, a, cpu_rp, MO_TEQ);
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
    return trans_st_all(ctx, a, cpu_rg, MO_UB);
}

static bool trans_stw(DisasContext *ctx, arg_stw *a)
{
    return trans_st_all(ctx, a, cpu_rg, MO_TEUW);
}

static bool trans_stt(DisasContext *ctx, arg_stt *a)
{
    return trans_st_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_sto(DisasContext *ctx, arg_sto *a)
{
    return trans_st_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_strp(DisasContext *ctx, arg_strp *a)
{
    return trans_st_all(ctx, a, cpu_rp, MO_TEQ);
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
    return trans_ldm_all(ctx, a, cpu_rg, MO_SB);
}

static bool trans_ldmws(DisasContext *ctx, arg_ldmws *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TESW);
}

static bool trans_ldmts(DisasContext *ctx, arg_ldmts *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TESL);
}

static bool trans_ldmo(DisasContext *ctx, arg_ldmo *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_ldmbu(DisasContext *ctx, arg_ldmbu *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_UB);
}

static bool trans_ldmwu(DisasContext *ctx, arg_ldmwu *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEUW);
}

static bool trans_ldmtu(DisasContext *ctx, arg_ldmtu *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_ldmrp(DisasContext *ctx, arg_ldmrp *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_ldmft(DisasContext *ctx, arg_ldmft *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_ldmfo(DisasContext *ctx, arg_ldmfo *a)
{
    return trans_ldm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_ldmrr(DisasContext *ctx, arg_ldmrr *a)
{
    return trans_ldm_all(ctx, a, cpu_rr, MO_TEQ);
}

static bool trans_stmb(DisasContext *ctx, arg_stmb *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_UB);
}

static bool trans_stmw(DisasContext *ctx, arg_stmw *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEUW);
}

static bool trans_stmt(DisasContext *ctx, arg_stmt *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_stmo(DisasContext *ctx, arg_stmo *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_stmrp(DisasContext *ctx, arg_stmrp *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_stmft(DisasContext *ctx, arg_stmft *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEUL);
}

static bool trans_stmfo(DisasContext *ctx, arg_stmfo *a)
{
    return trans_stm_all(ctx, a, cpu_rg, MO_TEQ);
}

static bool trans_stmrr(DisasContext *ctx, arg_stmrr *a)
{
    return trans_stm_all(ctx, a, cpu_rr, MO_TEQ);
}

/* register assignment instructions */

static bool trans_rb2ra_all(DisasContext* ctx, arg_orri* a,
                            TCGv_i64* cpu_rb, TCGv_i64* cpu_ra) 
{
    if (a->ra == 0 || a->ra + a->cnt >= 64 || a->rb + a->cnt >= 64) {
        return false;
    }
    if (cpu_rb != cpu_ra) {
        while (a->cnt-- >= 0) {
            tcg_gen_mov_i64(cpu_ra[a->ra++], cpu_rb[a->rb++]);
        }
    } else {
        if (a->ra < a->rb) {
            while (a->cnt-- >= 0) {
                tcg_gen_mov_i64(cpu_ra[a->ra++], cpu_rb[a->rb++]);
            }
        } else if (a->ra > a->rb) {
            a->ra += a->cnt; a->rb += a->cnt;
            while (a->cnt-- >= 0) {
                tcg_gen_mov_i64(cpu_ra[a->ra--], cpu_rb[a->rb--]);
            }
        } else {
            /* a->ra == a->rb, do nothing */
        }
    }
    return true;
}

static bool trans_cs_all(DisasContext* ctx, arg_rrrr* a, TCGCond cond)
{
    if (a->rga == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_movcond_i64(cond, cpu_rg[a->rga], cpu_rg[a->rgb], zero,
                        cpu_rg[a->rgc], cpu_rg[a->rgd]);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_cs_od_ev(DisasContext* ctx, arg_rrrr* a, bool is_od)
{
    if (a->rga == 0) {
        return false;
    }
    TCGv_i64 bit0 = tcg_const_i64(1);
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_and_i64(bit0, bit0, cpu_rg[a->rgb]);
    tcg_gen_movcond_i64(is_od ? TCG_COND_NE : TCG_COND_EQ, cpu_rg[a->rga],
                        bit0, zero, cpu_rg[a->rgc], cpu_rg[a->rgd]);
    tcg_temp_free_i64(bit0);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_rg2rg(DisasContext* ctx, arg_rg2rg* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rg, cpu_rg);
}

static bool trans_rg2rp(DisasContext* ctx, arg_rg2rp* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rg, cpu_rp);
}

static bool trans_rg2rf(DisasContext* ctx, arg_rg2rf* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rg, cpu_rf);
}

static bool trans_rp2rg(DisasContext* ctx, arg_rp2rg* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rp, cpu_rg);
}

static bool trans_rp2rp(DisasContext* ctx, arg_rp2rp* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rp, cpu_rp);
}

static bool trans_rf2rg(DisasContext* ctx, arg_rf2rg* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rf, cpu_rg);
}

static bool trans_rf2rf(DisasContext* ctx, arg_rf2rf* a)
{
    return trans_rb2ra_all(ctx, a, cpu_rf, cpu_rf);
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
    if (a->rg == 0) {
        return false;
    }
    int64_t arg = ((int64_t)a->imm << (a->j * 16)) |
                  ~((int64_t)0xFFFF << (a->j * 16));
    tcg_gen_movi_i64(cpu_rg[a->rg], arg);
    return true;
}

static bool trans_setzw(DisasContext *ctx, arg_setzw *a)
{
    if (a->rg == 0) {
        return false;
    }
    int64_t arg = (int64_t)a->imm << (a->j * 16);
    tcg_gen_movi_i64(cpu_rg[a->rg], arg);
    return true;
}

static bool trans_orw(DisasContext *ctx, arg_orw *a)
{
    if (a->rg == 0) {
        return false;
    }
    int64_t arg = (int64_t)a->imm << (a->j * 16);
    tcg_gen_ori_i64(cpu_rg[a->rg], cpu_rg[a->rg], arg);
    return true;
}

static bool trans_andnw(DisasContext *ctx, arg_andnw *a)
{
    if (a->rg == 0) {
        return false;
    }
    int64_t arg = ((int64_t)a->imm << (a->j * 16)) |
                  ~((int64_t)0xFFFF << (a->j * 16));
    int64_t xor = (int64_t)0xFFFF << (a->j * 16);
    tcg_gen_andi_i64(cpu_rg[a->rg], cpu_rg[a->rg], arg);
    tcg_gen_xori_i64(cpu_rg[a->rg], cpu_rg[a->rg], xor);
    return true;
}

/* arithmetic instructions */

static bool trans_addi(DisasContext *ctx, arg_addi *a)
{
    if (a->rg == 0) {
        return false;
    }
    tcg_gen_addi_i64(cpu_rg[a->rg], cpu_rg[a->rg], a->imm);
    return true;
}

static bool trans_addrp(DisasContext *ctx, arg_addrp *a)
{
    if (a->rp == 0) {
        return false;
    }
    tcg_gen_addi_i64(cpu_rp[a->rp], cpu_rp[a->rp], a->imm);
    return true;
}

static bool trans_add(DisasContext *ctx, arg_add *a)
{
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_add2_i64(cpu_rg[a->rgb], cpu_rg[a->rga],
                     cpu_rg[a->rgc], zero, cpu_rg[a->rgd], zero);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_sub(DisasContext *ctx, arg_sub *a)
{
    TCGv_i64 zero = tcg_const_i64(0);
    tcg_gen_sub2_i64(cpu_rg[a->rgb], cpu_rg[a->rga],
                     cpu_rg[a->rgc], zero, cpu_rg[a->rgd], zero);
    tcg_temp_free_i64(zero);
    return true;
}

static bool trans_muls(DisasContext *ctx, arg_muls *a)
{
    tcg_gen_muls2_i64(cpu_rg[a->rgb], cpu_rg[a->rga],
                      cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_mulu(DisasContext *ctx, arg_mulu *a)
{
    tcg_gen_mulu2_i64(cpu_rg[a->rgb], cpu_rg[a->rga],
                      cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_divs(DisasContext *ctx, arg_divs *a)
{
    tcg_gen_div_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    tcg_gen_rem_i64(cpu_rg[a->rga], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_divu(DisasContext *ctx, arg_divu *a)
{
    tcg_gen_divu_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    tcg_gen_remu_i64(cpu_rg[a->rga], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_cmpsi(DisasContext *ctx, arg_cmpsi *a)
{
    if (a->rga == 0) {
        return false;
    }
    TCGv_i64 imm = tcg_const_i64(a->imm);
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GT, cpu_rg[a->rga], cpu_rg[a->rgb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rg[a->rga], cpu_rg[a->rgb], imm,
                        neg, cpu_rg[a->rga]);
    tcg_temp_free_i64(imm);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmpui(DisasContext *ctx, arg_cmpui *a)
{
    if (a->rga == 0) {
        return false;
    }
    TCGv_i64 imm = tcg_const_i64(a->imm);
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GTU, cpu_rg[a->rga], cpu_rg[a->rgb], imm,
                        pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rg[a->rga], cpu_rg[a->rgb], imm,
                        neg, cpu_rg[a->rga]);
    tcg_temp_free_i64(imm);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmps(DisasContext *ctx, arg_cmps *a)
{
    if (a->rgb == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GT, cpu_rg[a->rgb], cpu_rg[a->rgc],
                        cpu_rg[a->rgd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LT, cpu_rg[a->rgb], cpu_rg[a->rgc],
                        cpu_rg[a->rgd], neg, cpu_rg[a->rgb]);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_cmpu(DisasContext *ctx, arg_cmpu *a)
{
    if (a->rgb == 0) {
        return false;
    }
    TCGv_i64 zero = tcg_const_i64(0);
    TCGv_i64 pos = tcg_const_i64(1);
    TCGv_i64 neg = tcg_const_i64(-1);
    tcg_gen_movcond_i64(TCG_COND_GTU, cpu_rg[a->rgb], cpu_rg[a->rgc],
                        cpu_rg[a->rgd], pos, zero);
    tcg_gen_movcond_i64(TCG_COND_LTU, cpu_rg[a->rgb], cpu_rg[a->rgc],
                        cpu_rg[a->rgd], neg, cpu_rg[a->rgb]);
    tcg_temp_free_i64(zero);
    tcg_temp_free_i64(pos);
    tcg_temp_free_i64(neg);
    return true;
}

static bool trans_and(DisasContext *ctx, arg_and *a)
{
    if (a->rgb == 0) {
        return false;
    }
    tcg_gen_and_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_orr(DisasContext *ctx, arg_orr *a)
{
    if (a->rgb == 0) {
        return false;
    }
    tcg_gen_or_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_xor(DisasContext *ctx, arg_xor *a)
{
    if (a->rgb == 0) {
        return false;
    }
    tcg_gen_xor_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_shlu(DisasContext *ctx, arg_shlu *a)
{
    tcg_gen_shl_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_shrs(DisasContext *ctx, arg_shrs *a)
{
    tcg_gen_sar_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_shru(DisasContext *ctx, arg_shru *a)
{
    tcg_gen_shr_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], cpu_rg[a->rgd]);
    return true;
}

static bool trans_exts(DisasContext *ctx, arg_exts *a)
{
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_subfi_i64(imm, 63, cpu_rg[a->rgd]);
    tcg_gen_shl_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], imm);
    tcg_gen_sar_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_extz(DisasContext *ctx, arg_extz *a)
{
    TCGv_i64 imm = tcg_temp_new_i64();
    tcg_gen_subfi_i64(imm, 63, cpu_rg[a->rgd]);
    tcg_gen_shl_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], imm);
    tcg_gen_shr_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_not(DisasContext *ctx, arg_not *a)
{
    TCGv_i64 imm = tcg_const_i64((int64_t)~0);
    tcg_gen_shl_i64(imm, imm, cpu_rg[a->rgd]);
    tcg_gen_xor_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], imm);
    tcg_temp_free_i64(imm);
    return true;
}

static bool trans_shlui(DisasContext *ctx, arg_shlui *a)
{
    tcg_gen_shli_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], a->imm);
    return true;
}

static bool trans_shrsi(DisasContext *ctx, arg_shrsi *a)
{
    tcg_gen_sari_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], a->imm);
    return true;
}

static bool trans_shrui(DisasContext *ctx, arg_shrui *a)
{
    tcg_gen_shri_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], a->imm);
    return true;
}

static bool trans_extsi(DisasContext *ctx, arg_extsi *a)
{
    tcg_gen_shli_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], 63 - a->imm);
    tcg_gen_sari_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], 63 - a->imm);
    return true;
}

static bool trans_extzi(DisasContext *ctx, arg_extzi *a)
{
    tcg_gen_shli_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], 63 - a->imm);
    tcg_gen_shri_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], 63 - a->imm);
    return true;
}

static bool trans_noti(DisasContext *ctx, arg_noti *a)
{
    tcg_gen_xori_i64(cpu_rg[a->rgb], cpu_rg[a->rgc], (int64_t)~0 << a->imm);
    return true;
}

/* control flow instructions */

static bool trans_swym(DisasContext *ctx, arg_swym *a)
{
    return true;
}

static bool trans_ret(DisasContext *ctx, arg_swym *a)
{
    return true;
}

static bool trans_trap(DisasContext *ctx, arg_trap *a)
{
    tcg_gen_movi_i32(cpu_trap_num, a->num);
    gen_exception(DADAO_EXCP_TRAP);
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
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
        qemu_fprintf(f, "R%02d=%08lx", i, env->rg[i]);
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
