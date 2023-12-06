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

static void dadao_cpu_set_pc(CPUState *cs, vaddr value)
{
    DADAOCPU *cpu = DADAO_CPU(cs);

    cpu->env.REG_PC = value;
}

static bool dadao_cpu_has_work(CPUState *cs)
{
    return 0;
}

static inline void set_feature(CPUDADAOState *env, int feature)
{
    env->features |= feature;
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

#include "hw/core/tcg-cpu-ops.h"

static struct TCGCPUOps dadao_tcg_ops = {
    .initialize = dadao_translate_init,
    .cpu_exec_interrupt = dadao_cpu_exec_interrupt,
    .tlb_fill = dadao_cpu_tlb_fill,
#ifndef CONFIG_USER_ONLY
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
#ifndef CONFIG_USER_ONLY
    cc->get_phys_page_debug = dadao_cpu_get_phys_page_debug;
#endif
    dc->vmsd = &vmstate_dadao_cpu;
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
