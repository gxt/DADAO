/*
 * Softmmu related functions
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Liang Shuhao <1700012741@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 */
#ifdef CONFIG_USER_ONLY
#error This file only exist under softmmu circumstance
#endif

#include "qemu/osdep.h"
/* keep */
#include "cpu.h"
/* this */
#include "exec/exec-all.h"
/* order */
#include "qemu/error-report.h"

#undef DEBUG_DADAO

#ifdef DEBUG_DADAO
#define DPRINTF(fmt, ...) printf("%s: " fmt , __func__, ## __VA_ARGS__)
#else
#define DPRINTF(fmt, ...) do {} while (0)
#endif

#define SUPERPAGE_SIZE             (1 << 22)
#define DADAO_PAGETABLE_READ        (1 << 8)
#define DADAO_PAGETABLE_WRITE       (1 << 7)
#define DADAO_PAGETABLE_EXEC        (1 << 6)
#define DADAO_PAGETABLE_EXIST       (1 << 2)
#define PAGETABLE_TYPE(x)          ((x) & 3)


/* Map CPU modes onto saved register banks.  */
static inline int bank_number(CPUDADAOState *env, int mode)
{
    cpu_abort(env_cpu(env), "Bad mode %x\n", mode);
    return -1;
}

void switch_mode(CPUDADAOState *env, int mode)
{
    return;
}

/* Handle a CPU exception.  */
void dadao_cpu_do_interrupt(CPUState *cs)
{
    DADAOCPU *cpu = DADAO_CPU(cs);
    CPUDADAOState *env = &cpu->env;
    int new_mode = 0;

    switch_mode(env, new_mode);
}

static int get_phys_addr_dadao(CPUDADAOState *env, uint32_t address,
        int access_type, int is_user, uint32_t *phys_ptr, int *prot,
        target_ulong *page_size)
{
    CPUState *cs = env_cpu(env);
    int code;
    uint32_t table;
    uint32_t desc;
    uint32_t phys_addr;

    /* Pagetable walk.  */
    /* Lookup l1 descriptor.  */
    table = env->cp0.c2_base & 0xfffff000;
    table |= (address >> 20) & 0xffc;
    desc = ldl_phys(cs->as, table);
    code = 0;
    switch (PAGETABLE_TYPE(desc)) {
    case 3:
        /* Superpage  */
        if (!(desc & DADAO_PAGETABLE_EXIST)) {
            code = 0x0b; /* superpage miss */
            goto do_fault;
        }
        phys_addr = (desc & 0xffc00000) | (address & 0x003fffff);
        *page_size = SUPERPAGE_SIZE;
        break;
    case 0:
        /* Lookup l2 entry.  */
        if (is_user) {
            DPRINTF("PGD address %x, desc %x\n", table, desc);
        }
        if (!(desc & DADAO_PAGETABLE_EXIST)) {
            code = 0x05; /* second pagetable miss */
            goto do_fault;
        }
        table = (desc & 0xfffff000) | ((address >> 10) & 0xffc);
        desc = ldl_phys(cs->as, table);
        /* 4k page.  */
        if (is_user) {
            DPRINTF("PTE address %x, desc %x\n", table, desc);
        }
        if (!(desc & DADAO_PAGETABLE_EXIST)) {
            code = 0x08; /* page miss */
            goto do_fault;
        }
        switch (PAGETABLE_TYPE(desc)) {
        case 0:
            phys_addr = (desc & 0xfffff000) | (address & 0xfff);
            *page_size = TARGET_PAGE_SIZE;
            break;
        default:
            cpu_abort(cs, "wrong page type!");
        }
        break;
    default:
        cpu_abort(cs, "wrong page type!");
    }

    *phys_ptr = phys_addr;
    *prot = 0;
    /* Check access permissions.  */
    if (desc & DADAO_PAGETABLE_READ) {
        *prot |= PAGE_READ;
    } else {
        if (is_user && (access_type == 0)) {
            code = 0x11; /* access unreadable area */
            goto do_fault;
        }
    }

    if (desc & DADAO_PAGETABLE_WRITE) {
        *prot |= PAGE_WRITE;
    } else {
        if (is_user && (access_type == 1)) {
            code = 0x12; /* access unwritable area */
            goto do_fault;
        }
    }

    if (desc & DADAO_PAGETABLE_EXEC) {
        *prot |= PAGE_EXEC;
    } else {
        if (is_user && (access_type == 2)) {
            code = 0x13; /* access unexecutable area */
            goto do_fault;
        }
    }

do_fault:
    return code;
}

bool dadao_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
                       MMUAccessType access_type, int mmu_idx,
                       bool probe, uintptr_t retaddr)
{
    DADAOCPU *cpu = DADAO_CPU(cs);
    CPUDADAOState *env = &cpu->env;
    uint32_t phys_addr;
    target_ulong page_size;
    int prot;
    int ret, is_user;

    ret = 1;
    is_user = mmu_idx == MMU_USER_IDX;

    if ((env->cp0.c1_sys & 1) == 0) {
        /* MMU disabled.  */
        phys_addr = address;
        prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        page_size = TARGET_PAGE_SIZE;
        ret = 0;
    } else {
        if ((address & (1 << 31)) || (is_user)) {
            ret = get_phys_addr_dadao(env, address, access_type, is_user,
                                    &phys_addr, &prot, &page_size);
            if (is_user) {
                DPRINTF("user space access: ret %x, address %" VADDR_PRIx ", "
                        "access_type %x, phys_addr %x, prot %x\n",
                        ret, address, access_type, phys_addr, prot);
            }
        } else {
            /*IO memory */
            phys_addr = address | (1 << 31);
            prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
            page_size = TARGET_PAGE_SIZE;
            ret = 0;
        }
    }

    if (ret == 0) {
        /* Map a single page.  */
        phys_addr &= TARGET_PAGE_MASK;
        address &= TARGET_PAGE_MASK;
        tlb_set_page(cs, address, phys_addr, prot, mmu_idx, page_size);
        return true;
    }

    if (probe) {
        return false;
    }

    env->cp0.c3_faultstatus = ret;
    env->cp0.c4_faultaddr = address;
    if (access_type == 2) {
        cs->exception_index = DADAO_EXCP_ITRAP;
    } else {
        cs->exception_index = DADAO_EXCP_DTRAP;
    }
    cpu_loop_exit_restore(cs, retaddr);
}

hwaddr dadao_cpu_get_phys_page_debug(CPUState *cs, vaddr addr)
{
    error_report("function dadao_cpu_get_phys_page_debug not "
                    "implemented, aborting");
    return -1;
}
