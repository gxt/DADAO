/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef __DADAO_ASM_SEGMENT_H__
#define __DADAO_ASM_SEGMENT_H__

#ifndef __ASSEMBLY__

typedef struct {
	unsigned long seg;
} mm_segment_t;

#endif /* __ASSEMBLY__ */

#endif /* __DADAO_ASM_SEGMENT_H__ */
