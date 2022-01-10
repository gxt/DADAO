/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* Set flags signalling availability of kernel features based on given
   kernel version number.  DADAO version. */

#include_next <kernel-features.h>

#undef	__ASSUME_SYSVIPC_DEFAULT_IPC_64
