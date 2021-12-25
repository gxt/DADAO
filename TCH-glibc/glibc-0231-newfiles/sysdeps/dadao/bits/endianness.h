/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_DADAO_BITS_ENDIANNESS_H_
#define _SYSDEPS_DADAO_BITS_ENDIANNESS_H_

#ifndef _BITS_ENDIAN_H
# error "Never use <bits/endianness.h> directly; include <endian.h> instead."
#endif

/* DADAO only support big endian */
#define __BYTE_ORDER	__BIG_ENDIAN

#endif /* _SYSDEPS_DADAO_BITS_ENDIANNESS_H_ */
