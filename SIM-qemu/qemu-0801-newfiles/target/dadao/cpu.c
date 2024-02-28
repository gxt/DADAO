/*
 * QEMU DADAO CPU
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */

#include "qemu/osdep.h"
/* keep this order */
#include "cpu.h"
#include "exec/exec-all.h"
#include "migration/vmstate.h"
#include "qapi/error.h"
#include "tcg/tcg.h"

static void dadao_cpu_set_pc(CPUState *cs, vaddr value)
{
    DADAOCPU *cpu = DADAO_CPU(cs);

    cpu->env.REG_PC = value;
}

static vaddr dadao_cpu_get_pc(CPUState *cs)
{
    DADAOCPU *cpu = DADAO_CPU(cs);

    return cpu->env.REG_PC;
}

static void dadao_cpu_synchronize_from_tb(CPUState *cs, const TranslationBlock *tb)
{
    DADAOCPU *cpu = DADAO_CPU(cs);

    tcg_debug_assert(!(cs->tcg_cflags & CF_PCREL));
    cpu->env.REG_PC = tb->pc;
}

static bool dadao_cpu_has_work(CPUState *cs)
{
    return 0;
}

static inline void set_feature(CPUDADAOState *env, int feature)
{
    env->features |= feature;
}

static void dadao_restore_state_to_opc(CPUState *cs,
                                          const TranslationBlock *tb,
                                          const uint64_t *data)
{
    DADAOCPU *cpu = DADAO_CPU(cs);
    cpu->env.REG_PC = data[0];
}

static void dadao_disas_set_info(CPUState *cpu, disassemble_info *info)
{
    info->print_insn = print_insn_dadao;
}

/* CPU models */

static ObjectClass *dadao_cpu_class_by_name(const char *cpu_model)
{
    ObjectClass *oc;
    char *typename;

    typename = g_strdup_printf(DADAO_CPU_TYPE_NAME("%s"), cpu_model);
    oc = object_class_by_name(typename);
    g_free(typename);
    if (oc != NULL && (!object_class_dynamic_cast(oc, TYPE_DADAO_CPU) ||
                          object_class_is_abstract(oc))) {
        oc = NULL;
    }
    return oc;
}

static void dadao_any_cpu_initfn(Object *obj)
{
    DADAOCPU *cpu = DADAO_CPU(obj);
    CPUDADAOState *env = &cpu->env;

    env->cp0.c0_cpuid = 0xffffffff;

    set_feature(env, 0);
}

static void dadao_cpu_realizefn(DeviceState *dev, Error **errp)
{
    CPUState *cs = CPU(dev);
    DADAOCPUClass *ucc = DADAO_CPU_GET_CLASS(dev);
    Error *local_err = NULL;

    cpu_exec_realizefn(cs, &local_err);
    if (local_err != NULL) {
        error_propagate(errp, local_err);
        return;
    }

    qemu_init_vcpu(cs);

    ucc->parent_realize(dev, errp);
}

static void dadao_cpu_initfn(Object *obj)
{
    DADAOCPU *cpu = DADAO_CPU(obj);
    CPUDADAOState *env = &cpu->env;

    cpu_set_cpustate_pointers(cpu);

#ifdef CONFIG_USER_ONLY
    env->REG_PC = 0;
#else
    env->REG_PC = 0x03000000;
#endif
}

static const VMStateDescription vmstate_dadao_cpu = {
    .name = "cpu",
    .unmigratable = 1,
};

#include "hw/core/sysemu-cpu-ops.h"

static const struct SysemuCPUOps dadao_sysemu_ops = {
    .get_phys_page_debug = dadao_cpu_get_phys_page_debug,
};

#include "hw/core/tcg-cpu-ops.h"

static struct TCGCPUOps dadao_tcg_ops = {
    .initialize = dadao_translate_init,
    .synchronize_from_tb = dadao_cpu_synchronize_from_tb,
    .restore_state_to_opc = dadao_restore_state_to_opc,
#ifndef CONFIG_USER_ONLY
    .cpu_exec_interrupt = dadao_cpu_exec_interrupt,
    .tlb_fill = dadao_cpu_tlb_fill,
    .do_interrupt = dadao_cpu_do_interrupt,
#endif
};

static void dadao_cpu_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    CPUClass *cc = CPU_CLASS(oc);
    DADAOCPUClass *ucc = DADAO_CPU_CLASS(oc);

    device_class_set_parent_realize(dc, dadao_cpu_realizefn,
        &ucc->parent_realize);

    cc->class_by_name = dadao_cpu_class_by_name;
    cc->has_work = dadao_cpu_has_work;
    cc->dump_state = dadao_cpu_dump_state;
    cc->set_pc = dadao_cpu_set_pc;
    cc->get_pc = dadao_cpu_get_pc;
    dc->vmsd = &vmstate_dadao_cpu;
    cc->sysemu_ops = &dadao_sysemu_ops;
    cc->disas_set_info = dadao_disas_set_info;
    cc->tcg_ops = &dadao_tcg_ops;
}

#define DEFINE_DADAO_CPU_TYPE(cpu_model, initfn) \
    {                                            \
        .parent = TYPE_DADAO_CPU,                \
        .instance_init = initfn,                 \
        .name = DADAO_CPU_TYPE_NAME(cpu_model),  \
    }

static const TypeInfo dadao_cpu_type_infos[] = {
    {
        .name = TYPE_DADAO_CPU,
        .parent = TYPE_CPU,
        .instance_size = sizeof(DADAOCPU),
        .instance_init = dadao_cpu_initfn,
        .abstract = true,
        .class_size = sizeof(DADAOCPUClass),
        .class_init = dadao_cpu_class_init,
    },
    DEFINE_DADAO_CPU_TYPE("any", dadao_any_cpu_initfn),
};

DEFINE_TYPES(dadao_cpu_type_infos)
