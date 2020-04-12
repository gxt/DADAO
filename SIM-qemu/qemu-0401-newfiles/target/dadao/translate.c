/*
 * DADAO Translation
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Liang Shuhao <1700012741@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 *
 * Function hierachy:
 *     QEMU Runtime
 *          |
 *   gen_intermediate_code
 *          |
 *   disas_dadao_insn
 *          |
 *   do_arithmatic/load/store/...
 *          |
 *   gen_st8/ld16/exception/...
 *          |
 *   tcg_gen_and/or/mult..._i64
 */
#include "qemu/osdep.h"
/* keep */
#include "cpu.h"
#include "disas/disas.h"
#include "exec/cpu_ldst.h"
#include "exec/exec-all.h"
#include "exec/translator.h"
#include "qemu/log.h"
#include "qemu/qemu-print.h"
#include "tcg-op.h"
/* this */
#include "exec/helper-gen.h"
#include "exec/helper-proto.h"
/* order */
#include "exec/log.h"
#include "trace-tcg.h"

/* Disassemble info records.  */
typedef struct DisasContext {
    target_ulong pc;
    int is_jmp;
    /* Nonzero if this instruction has been conditionally skipped.  */
    int condjmp;
    /* The label that will be jumped to when the instruction is skipped.  */
    TCGLabel *condlabel;
    struct TranslationBlock *tb;
    int singlestep_enabled;
#ifndef CONFIG_USER_ONLY
    int user;
#endif
} DisasContext;

#ifndef CONFIG_USER_ONLY
#define IS_USER(s) (s->user)
#else
#define IS_USER(s) 1
#endif

/* is_jmp field values */
#define DISAS_JUMP DISAS_TARGET_0    /* only pc was modified dynamically */
#define DISAS_UPDATE DISAS_TARGET_1  /* cpu state was modified dynamically */
#define DISAS_TB_JUMP DISAS_TARGET_2 /* only pc was modified statically */
/* These instructions trap after executing, so defer them until after the
   conditional executions state has been updated.  */
#define DISAS_SYSCALL DISAS_TARGET_3

/* Make a copy of regiters as TCG variables for efficiency */
static TCGv_i64 cpu_RD[64];
static TCGv_i64 cpu_RP[64];
static TCGv_i64 cpu_RA[64];
static TCGv_i64 cpu_PC;

#include "exec/gen-icount.h"

static const char *regnames[] = {
    "zero", "rd01", "rd02", "rd03", "rd04", "rd05", "rd06", "rd07",
    "rd08", "rd09", "rd10", "rd11", "rd12", "rd13", "rd14", "rd15",
    "rd16", "rd17", "rd18", "rd19", "rd20", "rd21", "rd22", "rd23",
    "rd24", "rd25", "rd26", "rd27", "rd28", "rd29", "rd30", "rd31",
    "rd32", "rd33", "rd34", "rd35", "rd36", "rd37", "rd38", "rd39",
    "rd40", "rd41", "rd42", "rd43", "rd44", "rd45", "rd46", "rd47",
    "rd48", "rd49", "rd50", "rd51", "rd52", "rd53", "rd54", "rd55",
    "rd56", "rd57", "rd58", "rd59", "rd60", "rd61", "rd62", "rd63",
    "null", "rp01", "rp02", "rp03", "rp04", "rp05", "rp06", "rp07",
    "rp08", "rp09", "rp10", "rp11", "rp12", "rp13", "rp14", "rp15",
    "rp16", "rp17", "rp18", "rp19", "rp20", "rp21", "rp22", "rp23",
    "rp24", "rp25", "rp26", "rp27", "rp28", "rp29", "rp30", "rp31",
    "rp32", "rp33", "rp34", "rp35", "rp36", "rp37", "rp38", "rp39",
    "rp40", "rp41", "rp42", "rp43", "rp44", "rp45", "rp46", "rp47",
    "rp48", "rp49", "rp50", "rp51", "rp52", "rp53", "rp54", "rp55",
    "rp56", "rp57", "rp58", "rp59", "rp60", "rp61", "rp62", "rp63",
    "ra00", "ra01", "ra02", "ra03", "ra04", "ra05", "ra06", "ra07",
    "ra08", "ra09", "ra10", "ra11", "ra12", "ra13", "ra14", "ra15",
    "ra16", "ra17", "ra18", "ra19", "ra20", "ra21", "ra22", "ra23",
    "ra24", "ra25", "ra26", "ra27", "ra28", "ra29", "ra30", "ra31",
    "ra32", "ra33", "ra34", "ra35", "ra36", "ra37", "ra38", "ra39",
    "ra40", "ra41", "ra42", "ra43", "ra44", "ra45", "ra46", "ra47",
    "ra48", "ra49", "ra50", "ra51", "ra52", "ra53", "ra54", "ra55",
    "ra56", "ra57", "ra58", "ra59", "ra60", "ra61", "ra62", "ra63",
    "pc"};

/* Map TCG globals to CPU context.  */
void dadao_translate_init(void)
{
    int i;
    for (i = 0; i < 64; i++)
        cpu_RD[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUDADAOState, regds[i]), regnames[i]);
    for (i = 0; i < 64; i++)
        cpu_RP[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUDADAOState, regps[i]), regnames[i + 64]);
    for (i = 0; i < 64; i++)
        cpu_RA[i] = tcg_global_mem_new_i64(cpu_env,
            offsetof(CPUDADAOState, regas[i]), regnames[i + 128]);
    cpu_PC = tcg_global_mem_new_i64(cpu_env,
        offsetof(CPUDADAOState, regpc), regnames[192]);
}

static int num_temps; /* for gen_* debug only */

/* Allocate a temporary variable.  */
static TCGv_i64 new_tmp(void)
{
    num_temps++;
    return tcg_temp_new_i64();
}

/* Release a temporary variable.  */
static void free_tmp(TCGv_i64 tmp)
{
    tcg_temp_free(tmp);
    num_temps--;
}

/* Instruction decoders.  */
#define DADAO_INSN_OPCODE(insn) (((insn) >> 30) & 0x3)
#define DADAO_INSN_FUNCT(insn) (((insn) >> 27) & 0x7)
#define DADAO_INSN_OPERAND(insn) (((insn) >> 24) & 0x7)
#define DADAO_INSN_FD2(insn) (((insn) >> 18) & 0x3f)
#define DADAO_INSN_FD3(insn) (((insn) >> 12) & 0x3f)
#define DADAO_INSN_FD4(insn) (((insn) >> 6) & 0x3f)
#define DADAO_INSN_FD5(insn) ((insn)&0x3f)
#define DADAO_SIGNEXT(a, b) (((int64_t)(a) << (64 - (b))) >> (64 - (b)))
#define DADAO_INSN_IMM12(insn) ((uint64_t)(insn)&0xFFF)
#define DADAO_INSN_IMM16(insn) ((uint64_t)(insn)&0xFFFF)

#define ILLEGAL cpu_abort(env_cpu(env),         \
    "Illegal DADAO instruction %x at line %d!", \
    insn, __LINE__)

static void do_arith_2op(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    uint32_t fd2 = DADAO_INSN_FD2(insn);
    uint32_t fd3 = DADAO_INSN_FD3(insn);
    TCGv_i64 src, dst;
    if (!(fd2 & 0x20)) { /* mov */
        uint32_t fd4 = DADAO_INSN_FD4(insn);
        uint32_t fd5 = DADAO_INSN_FD4(insn);
        switch (fd2 & (fd3 << 6)) {
        case 0x1: /* rp2rd */
            src = cpu_RP[fd4];
            dst = cpu_RD[fd5];
            break;
        case 0x8: /* rd2rp */
            src = cpu_RD[fd4];
            dst = cpu_RP[fd5];
            break;
        case 0x0: /* rd2rd */
            src = cpu_RD[fd4];
            dst = cpu_RD[fd5];
            break;
        case 0x9: /* rp2rp */
            src = cpu_RP[fd4];
            dst = cpu_RP[fd5];
            break;
        default:
            ILLEGAL;
        }
        tcg_gen_mov_i64(dst, src);
    } else { /* 1op with imm */
        int64_t imm = DADAO_INSN_IMM12(insn);
        switch (fd2 & 7) {
        case 0x0: /* rd */
            src = dst = cpu_RD[fd3];
            break;
        case 0x1: /* rp */
            src = dst = cpu_RP[fd3];
            break;
        default:
            ILLEGAL;
        }
        switch (fd2 >> 3) {
        case 0x4: /* add */
            tcg_gen_addi_i64(dst, src, imm);
            break;
        case 0x5: /* sub */
            tcg_gen_subi_i64(dst, src, imm);
            break;
        case 0x6: /* and */
            tcg_gen_andi_i64(dst, src, imm);
            break;
        case 0x7: /* or */
            tcg_gen_ori_i64(dst, src, imm);
            break;
        }
    }
}

static void do_arith_movi(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    TCGv_i64 dst = cpu_RD[DADAO_INSN_FD2(insn)];
    int64_t imm = DADAO_INSN_IMM16(insn);
    int offset = (3 - ((insn >> 16) & 3)) << 4;
    uint64_t mask = ((uint64_t)0xFFFF) << offset;
    tcg_gen_andi_i64(dst, dst, ~mask);
    tcg_gen_ori_i64(dst, dst, imm << offset);
}

static void do_arith_3op(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    uint32_t fd2 = DADAO_INSN_FD2(insn);
    uint32_t fd5 = DADAO_INSN_FD5(insn);
    TCGv_i64 dst = cpu_RD[DADAO_INSN_FD3(insn)];
    TCGv_i64 src = cpu_RD[DADAO_INSN_FD4(insn)];
    TCGv_i64 tar;
    switch (fd2 >> 4) {
    case 0x0: /* shift */
        tar = new_tmp();
        switch (fd2 >> 2) {
        case 0x0: /* with imm */
            tcg_gen_movi_i64(tar, fd5);
            break;
        case 0x1: /* with reg */
            tcg_gen_mov_i64(tar, cpu_RD[fd5]);
            tcg_gen_andi_i64(tar, tar, 0x3F);
            break;
        default:
            ILLEGAL;
        }
        switch (fd2 & 0x3) {
        case 0x0: /* left */
            tcg_gen_shl_i64(dst, src, tar);
            break;
        case 0x2: /* arith right */
            tcg_gen_sar_i64(dst, src, tar);
            break;
        case 0x3: /* logic right */
            tcg_gen_shr_i64(dst, src, tar);
            break;
        case 0x4: /* circular right */
            tcg_gen_rotr_i64(dst, src, tar);
            break;
        }
        free_tmp(tar);
        break;
    case 0x2: /* math/bitwise */
        tar = cpu_RD[fd5];
        switch (fd2 & 0xF) {
        case 0x0: /* add */
            tcg_gen_add_i64(dst, src, tar);
            break;
        case 0x1: /* sub */
            tcg_gen_sub_i64(dst, src, tar);
            break;
        case 0x2: /* and */
            tcg_gen_and_i64(dst, src, tar);
            break;
        case 0x3: /* or */
            tcg_gen_or_i64(dst, src, tar);
            break;
        case 0x4: /* xor */
            tcg_gen_xor_i64(dst, src, tar);
            break;
        default:
            ILLEGAL;
        }
        break;
    default:
        ILLEGAL;
    }
}

static void do_arith_set(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    uint32_t fd2 = DADAO_INSN_FD2(insn);
    TCGv_i64 dst = cpu_RD[DADAO_INSN_FD3(insn)];
    TCGv_i64 src = cpu_RD[DADAO_INSN_FD4(insn)];
    TCGv_i64 tar = cpu_RD[DADAO_INSN_FD5(insn)];
    switch (fd2) {
    case 0x0:
        gen_helper_seq(dst, src, tar);
        break;
    case 0x1:
        gen_helper_sne(dst, src, tar);
        break;
    case 0x2:
        gen_helper_slt(dst, src, tar);
        break;
    case 0x3:
        gen_helper_sgt(dst, src, tar);
        break;
    case 0x4:
        gen_helper_sle(dst, src, tar);
        break;
    case 0x5:
        gen_helper_sge(dst, src, tar);
        break;
    case 0x6:
        gen_helper_sab(dst, src, tar);
        break;
    case 0x7:
        gen_helper_sae(dst, src, tar);
        break;
    case 0x8:
        gen_helper_sbl(dst, src, tar);
        break;
    case 0x9:
        gen_helper_sbe(dst, src, tar);
        break;
    default:
        ILLEGAL;
    }
}

static void do_arithmatic(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    switch (DADAO_INSN_OPERAND(insn)) {
    case 0x0: /* 2op (rd/rp) */
        do_arith_2op(env, s, insn);
        break;
    case 0x1: /* movi (rd) */
        do_arith_movi(env, s, insn);
        break;
    case 0x2: /* 3op (rd) */
        do_arith_3op(env, s, insn);
        break;
    case 0x3: /* set (rd) */
        do_arith_set(env, s, insn);
        break;
    default:
        ILLEGAL;
    }
}

static void do_load(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    TCGv_i64 dst = cpu_RD[DADAO_INSN_FD2(insn)];
    TCGv_i64 addr;
    uint32_t imm;

    addr = new_tmp();
    tcg_gen_mov_i64(addr, cpu_RP[DADAO_INSN_FD3(insn)]);
    tcg_gen_movi_i64(dst, 0);

    if (!(DADAO_INSN_OPERAND(insn) & 0x4)) { /* rp1 + imm12 */
        imm = DADAO_INSN_IMM12(insn);
        imm = DADAO_SIGNEXT(imm, 12);
        switch (DADAO_INSN_OPERAND(insn)) {
        case 0x2:
            tcg_gen_qemu_ld32u(dst, addr, imm);
            break;
        case 0x3:
            tcg_gen_qemu_ld64(dst, addr, imm);
            break;
        default:
            ILLEGAL;
        }
    } else { /* rp1 + rd2 ror imm6 */
        imm = DADAO_INSN_FD5(insn);
        tcg_gen_rotri_i64(addr, addr, imm);
        switch (DADAO_INSN_OPERAND(insn)) {
        case 0x4:
            tcg_gen_qemu_ld8u(dst, addr, 0);
            break;
        case 0x5:
            tcg_gen_qemu_ld16u(dst, addr, 0);
            break;
        case 0x6:
            tcg_gen_qemu_ld32u(dst, addr, 0);
            break;
        case 0x7:
            tcg_gen_qemu_ld64(dst, addr, 0);
            break;
        }
    }

    free_tmp(addr);
}

static void do_store(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    TCGv_i64 src = cpu_RD[DADAO_INSN_FD2(insn)];
    TCGv_i64 addr;
    uint32_t imm;

    addr = new_tmp();
    tcg_gen_mov_i64(addr, cpu_RP[DADAO_INSN_FD3(insn)]);

    if (!(DADAO_INSN_OPERAND(insn) & 0x4)) { /* rp1 + imm12 */
        imm = DADAO_INSN_IMM12(insn);
        imm = DADAO_SIGNEXT(imm, 12);
        switch (DADAO_INSN_OPERAND(insn)) {
        case 0x2:
            tcg_gen_qemu_st32(src, addr, imm);
            break;
        case 0x3:
            tcg_gen_qemu_st64(src, addr, imm);
            break;
        default:
            ILLEGAL;
        }
    } else { /* rp1 + rd2 ror imm6 */
        imm = DADAO_INSN_FD5(insn);
        tcg_gen_rotri_i64(addr, addr, imm);
        switch (DADAO_INSN_OPERAND(insn)) {
        case 0x4:
            tcg_gen_qemu_st8(src, addr, 0);
            break;
        case 0x5:
            tcg_gen_qemu_st16(src, addr, 0);
            break;
        case 0x6:
            tcg_gen_qemu_st32(src, addr, 0);
            break;
        case 0x7:
            tcg_gen_qemu_st64(src, addr, 0);
            break;
        }
    }

    free_tmp(addr);
}

static void do_branch(CPUDADAOState *env, DisasContext *s, uint32_t insn)
{
    switch (DADAO_INSN_OPERAND(insn)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 7:
    default:
        ILLEGAL;
    }
}

/* Entry to decode an instruction */
static void disas_dadao_insn(CPUDADAOState *env, DisasContext *s)
{
    unsigned int insn;

    insn = cpu_ldl_code(env, s->pc);
    s->pc += 4;

    switch (DADAO_INSN_OPCODE(insn)) {
    case 0x0: /* user mode */
        switch (DADAO_INSN_FUNCT(insn)) {
        case 0x1:
            do_arithmatic(env, s, insn);
            break;
        case 0x4:
            do_load(env, s, insn);
            break;
        case 0x5:
            do_store(env, s, insn);
            break;
        case 0x7:
            do_branch(env, s, insn);
            break;
        default:
            ILLEGAL;
        }
    case 0x3: /* system mode */
    default:
        ILLEGAL;
    }
}

/* Entry to generating intermediate code for basic block 'tb' */
void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    CPUDADAOState *env = cs->env_ptr;
    DisasContext dc;
    target_ulong pc_start;
    uint32_t page_start;
    int num_insns;

    /* generate intermediate code */
    num_temps = 0;

    pc_start = tb->pc;

    dc.tb = tb;

    dc.is_jmp = DISAS_NEXT;
    dc.pc = pc_start;
    dc.singlestep_enabled = cs->singlestep_enabled;
    dc.condjmp = 0;
    page_start = pc_start & TARGET_PAGE_MASK;
    num_insns = 0;

    gen_tb_start(tb);
    do {
        tcg_gen_insn_start(dc.pc);
        num_insns++;

        disas_dadao_insn(env, &dc);

        if (num_temps) { /* check for TCG temp number */
            fprintf(stderr, "Internal resource leak before %08lx\n", dc.pc);
            num_temps = 0;
        }

        if (dc.condjmp && !dc.is_jmp) { /* check for branch */
            gen_set_label(dc.condlabel);
            dc.condjmp = 0;
        }
    } while (!dc.is_jmp && !tcg_op_buf_full() &&
             !cs->singlestep_enabled &&
             dc.pc - page_start < TARGET_PAGE_SIZE &&
             num_insns < max_insns);

    gen_tb_end(tb, num_insns);

#ifdef DEBUG_DISAS
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM) && qemu_log_in_addr_range(pc_start)) {
        qemu_log_lock();
        qemu_log("----------------\n");
        qemu_log("IN: %s\n", lookup_symbol(pc_start));
        log_target_disas(cs, pc_start, dc.pc - pc_start);
        qemu_log("\n");
        qemu_log_unlock();
    }
#endif

    tb->size = dc.pc - pc_start;
    tb->icount = num_insns;
}

static const char *cpu_mode_names[16] = {
    "USER", "REAL", "INTR", "PRIV", "UM14", "UM15", "UM16", "TRAP",
    "UM18", "UM19", "UM1A", "EXTN", "UM1C", "UM1D", "UM1E", "SUSR"};

void dadao_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    DADAOCPU *cpu = DADAO_CPU(cs);
    CPUDADAOState *env = &cpu->env;
    int i;
    uint32_t psr;

    for (i = 0; i < 32; i++) {
        qemu_fprintf(f, "R%02d=%08lx", i, env->regds[i]);
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
    env->regpc = data[0];
}
