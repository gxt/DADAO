/*
 * QEMU DADAO CPU
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *  Liang Shuhao <1700012741@pku.edu.cn>
 *  Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef TARGET_DADAO_CPU_QOM_H
#define TARGET_DADAO_CPU_QOM_H

#include "hw/core/cpu.h"
#include "qom/object.h"

#define TYPE_DADAO_CPU "dadao-cpu"

OBJECT_DECLARE_CPU_TYPE(DADAOCPU, DADAOCPUClass, DADAO_CPU)

#define DADAO_CPU_CLASS(klass) \
    OBJECT_CLASS_CHECK(DADAOCPUClass, (klass), TYPE_DADAO_CPU)
#define DADAO_CPU(obj) \
    OBJECT_CHECK(DADAOCPU, (obj), TYPE_DADAO_CPU)
#define DADAO_CPU_GET_CLASS(obj) \
    OBJECT_GET_CLASS(DADAOCPUClass, (obj), TYPE_DADAO_CPU)

/**
 * DADAOCPUClass:
 * @parent_realize: The parent class' realize handler.
 *
 * A DADAO CPU model.
 */
struct DADAOCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/

    DeviceRealize parent_realize;
};

#endif
