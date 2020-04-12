/*
 * DADAO virtual CPU header
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Liang Shuhao <1700012741@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

#ifndef DADAO_CPU_H
#define DADAO_CPU_H

#include "cpu-qom.h"
/* keep order */
#include "exec/cpu-defs.h"

typedef struct CPUDADAOState {
    /* General-purpose registers.  */
    uint64_t regds[64]; /* data registers */
    uint64_t regps[64]; /* pointer registers */
    uint64_t regas[64]; /* return address registers */
    uint64_t regpc;

    /* System control coprocessor (cp0) */
    struct {
        uint32_t c0_cpuid;
        uint32_t c0_cachetype;
        uint32_t c1_sys;         /* System control register.  */
        uint32_t c2_base;        /* MMU translation table base.  */
        uint32_t c3_faultstatus; /* Fault status registers.  */
        uint32_t c4_faultaddr;   /* Fault address registers.  */
        uint32_t c5_cacheop;     /* Cache operation registers.  */
        uint32_t c6_tlbop;       /* TLB operation registers. */
    } cp0;

    /* Internal CPU feature flags.  */
    uint32_t features;

} CPUDADAOState;

/**
 * DADAOCPU:
 * @env: #CPUDADAOState
 *
 * A DADAO CPU.
 */
struct DADAOCPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/

    CPUNegativeOffsetState neg;
    CPUDADAOState env;
};

void dadao_cpu_do_interrupt(CPUState *cpu);
bool dadao_cpu_exec_interrupt(CPUState *cpu, int int_req);
void dadao_cpu_dump_state(CPUState *cpu, FILE *f, int flags);
hwaddr dadao_cpu_get_phys_page_debug(CPUState *cpu, vaddr addr);

#define DADAO_EXCP_PRIV (1)
#define DADAO_EXCP_ITRAP (2)
#define DADAO_EXCP_DTRAP (3)
#define DADAO_EXCP_INTR (4)

/* Return the current ASR value.  */
target_ulong cpu_asr_read(CPUDADAOState *env1);
/* Set the ASR.  Note that some bits of mask must be all-set or all-clear.  */
void cpu_asr_write(CPUDADAOState *env1, target_ulong val, target_ulong mask);

#define cpu_signal_handler dadao_cpu_signal_handler

int dadao_cpu_signal_handler(int host_signum, void *pinfo, void *puc);

/* MMU modes definitions */
#define MMU_MODE0_SUFFIX _kernel
#define MMU_MODE1_SUFFIX _user
#define MMU_USER_IDX 1
static inline int cpu_mmu_index(CPUDADAOState *env, bool ifetch)
{
    return 0;
}

typedef CPUDADAOState CPUArchState;
typedef DADAOCPU ArchCPU;

#include "exec/cpu-all.h"

#define DADAO_CPU_TYPE_SUFFIX "-" TYPE_DADAO_CPU
#define DADAO_CPU_TYPE_NAME(model) model DADAO_CPU_TYPE_SUFFIX
#define CPU_RESOLVING_TYPE TYPE_DADAO_CPU

static inline void cpu_get_tb_cpu_state(CPUDADAOState *env, target_ulong *pc,
    target_ulong *cs_base, uint32_t *flags)
{
    *pc = env->regpc;
    *cs_base = 0;
    *flags = 0;
}

bool dadao_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
    MMUAccessType access_type, int mmu_idx,
    bool probe, uintptr_t retaddr);
void dadao_translate_init(void);
void switch_mode(CPUDADAOState *, int);

#endif /* DADAO_CPU_H */
