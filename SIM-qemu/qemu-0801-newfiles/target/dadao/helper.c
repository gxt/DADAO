/*
 * Helper functions for DADAO
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2021:
 *  Hao Chenqi <hchenqi@pku.edu.cn>
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */

#include "qemu/osdep.h"
/* keep order */
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/gdbstub.h"
#include "exec/helper-proto.h"
#include "qemu/host-utils.h"
#ifndef CONFIG_USER_ONLY
#include "ui/console.h"
#endif

#undef DEBUG_DADAO

#ifdef DEBUG_DADAO
#define DPRINTF(fmt, ...) printf("%s: " fmt, __func__, ##__VA_ARGS__)
#else
#define DPRINTF(fmt, ...) \
    do {                  \
    } while (0)
#endif

#ifndef CONFIG_USER_ONLY
void helper_cp0_set(CPUDADAOState *env, uint32_t val, uint32_t creg,
                    uint32_t cop)
{
    /*
     * movc pp.nn, rn, #imm9
     *      rn: UCOP_REG_D
     *      nn: UCOP_REG_N
     *          1: sys control reg.
     *          2: page table base reg.
     *          3: data fault status reg.
     *          4: insn fault status reg.
     *          5: cache op. reg.
     *          6: tlb op. reg.
     *      imm9: split UCOP_IMM10 with bit5 is 0
     */
    switch (creg) {
    case 1:
        if (cop != 0) {
            goto unrecognized;
        }
        env->cp0.c1_sys = val;
        break;
    case 2:
        if (cop != 0) {
            goto unrecognized;
        }
        env->cp0.c2_base = val;
        break;
    case 3:
        if (cop != 0) {
            goto unrecognized;
        }
        env->cp0.c3_faultstatus = val;
        break;
    case 4:
        if (cop != 0) {
            goto unrecognized;
        }
        env->cp0.c4_faultaddr = val;
        break;
    case 5:
        switch (cop) {
        case 28:
            DPRINTF("Invalidate Entire I&D cache\n");
            return;
        case 20:
            DPRINTF("Invalidate Entire Icache\n");
            return;
        case 12:
            DPRINTF("Invalidate Entire Dcache\n");
            return;
        case 10:
            DPRINTF("Clean Entire Dcache\n");
            return;
        case 14:
            DPRINTF("Flush Entire Dcache\n");
            return;
        case 13:
            DPRINTF("Invalidate Dcache line\n");
            return;
        case 11:
            DPRINTF("Clean Dcache line\n");
            return;
        case 15:
            DPRINTF("Flush Dcache line\n");
            return;
        }
        break;
    case 6:
        if ((cop <= 6) && (cop >= 2)) {
            /* invalid all tlb */
            tlb_flush(env_cpu(env));
            return;
        }
        break;
    default:
        goto unrecognized;
    }
    return;
unrecognized:
    DPRINTF("Wrong register (%d) or wrong operation (%d) in cp0_set!\n",
        creg, cop);
}

uint32_t helper_cp0_get(CPUDADAOState *env, uint32_t creg, uint32_t cop)
{
    /*
     * movc rd, pp.nn, #imm9
     *      rd: UCOP_REG_D
     *      nn: UCOP_REG_N
     *          0: cpuid and cachetype
     *          1: sys control reg.
     *          2: page table base reg.
     *          3: data fault status reg.
     *          4: insn fault status reg.
     *      imm9: split UCOP_IMM10 with bit5 is 0
     */
    switch (creg) {
    case 0:
        switch (cop) {
        case 0:
            return env->cp0.c0_cpuid;
        case 1:
            return env->cp0.c0_cachetype;
        }
        break;
    case 1:
        if (cop == 0) {
            return env->cp0.c1_sys;
        }
        break;
    case 2:
        if (cop == 0) {
            return env->cp0.c2_base;
        }
        break;
    case 3:
        if (cop == 0) {
            return env->cp0.c3_faultstatus;
        }
        break;
    case 4:
        if (cop == 0) {
            return env->cp0.c4_faultaddr;
        }
        break;
    }
    DPRINTF("Wrong register (%d) or wrong operation (%d) in cp0_set!\n",
        creg, cop);
    return 0;
}

#ifdef CONFIG_CURSES

/* KEY_EVENT is defined in wincon.h and in curses.h. Avoid redefinition. */
#undef KEY_EVENT
#include <curses.h>
#undef KEY_EVENT

/*
 * FIXME:
 *     1. curses windows will be blank when switching back
 *     2. backspace is not handled yet
 */
static void putc_on_screen(unsigned char ch)
{
    static WINDOW *localwin;
    static int init;

    if (!init) {
        /* Assume 80 * 30 screen to minimize the implementation */
        localwin = newwin(30, 80, 0, 0);
        scrollok(localwin, TRUE);
        init = TRUE;
    }

    if (isprint(ch)) {
        wprintw(localwin, "%c", ch);
    } else {
        switch (ch) {
        case '\n':
            wprintw(localwin, "%c", ch);
            break;
        case '\r':
            /* If '\r' is put before '\n', the curses window will destroy the
             * last print line. And meanwhile, '\n' implifies '\r' inside. */
            break;
        default: /* Not handled, so just print it hex code */
            wprintw(localwin, "-- 0x%x --", ch);
        }
    }

    wrefresh(localwin);
}
#else
#define putc_on_screen(c) \
    do {                  \
    } while (0)
#endif

void helper_cp1_putc(target_ulong x)
{
    putc_on_screen((unsigned char)x); /* Output to screen */
    DPRINTF("%c", x);                 /* Output to stdout */
}
#endif

bool dadao_cpu_exec_interrupt(CPUState *cs, int interrupt_request)
{
    if (interrupt_request & CPU_INTERRUPT_HARD) {

        if (0) {
            cs->exception_index = DADAO_EXCP_INTR;
            dadao_cpu_do_interrupt(cs);
            return true;
        }
    }
    return false;
}

void HELPER(exception)(CPUDADAOState *env, uint32_t excp)
{
    DADAOCPU *cpu = env_archcpu(env);
    CPUState *cs = CPU(cpu);
    cs->exception_index = excp;
    cpu_loop_exit(cs);
}

target_ulong cpu_asr_read(CPUDADAOState *env)
{
    return 0;
}

