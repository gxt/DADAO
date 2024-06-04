/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

/* Machine-specific POSIX semaphore type layouts.  */
#ifndef _SEMAPHORE_H
# error "Never use <bits/semaphore.h> directly; include <semaphore.h> instead."
#endif

#define __SIZEOF_SEM_T	32

/* Value returned if `sem_open' failed.  */
#define SEM_FAILED	((sem_t *) 0)

typedef union
{
  char __size[__SIZEOF_SEM_T];
  long int __align;
} sem_t;
