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

static bool trans_trap(DisasContext *ctx, arg_trap *a)
{
    tcg_gen_movi_i32(cpu_trap_num, a->num);
    gen_exception(DADAO_EXCP_TRAP);
    ctx->base.is_jmp = DISAS_NORETURN;
    return true;
}

static void dadao_tr_init_disas_context(DisasContextBase *dcbase, CPUState *cs)
{
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
