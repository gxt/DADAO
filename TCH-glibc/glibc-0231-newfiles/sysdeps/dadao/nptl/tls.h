/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef _SYSDEPS_DADAO_NPTL_TLS_H_
#define _SYSDEPS_DADAO_NPTL_TLS_H_

#include <dl-sysdep.h>

#ifndef __ASSEMBLER__
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <dl-dtv.h>

register struct pthread *__thread_self __asm__ ("rp4");

/* Get system call information.  */
# include <sysdep.h>

/* The TP points to the start of the thread blocks.  */
# define TLS_DTV_AT_TP	1
# define TLS_TCB_AT_TP	0

/* Get the thread descriptor definition.  */
# include <nptl/descr.h>

typedef struct
{
  dtv_t *dtv;
  void *__private;
} tcbhead_t;

/* This is the size of the initial TCB.  */
# define TLS_INIT_TCB_SIZE		sizeof (tcbhead_t)

/* Alignment requirements for the initial TCB.  */
# define TLS_INIT_TCB_ALIGN		__alignof__ (tcbhead_t)

/* This is the size of the TCB.  */
# define TLS_TCB_SIZE			sizeof (tcbhead_t)

/* Alignment requirements for the TCB.  */
# define TLS_TCB_ALIGN			__alignof__ (struct pthread)

/* This is the size we need before TCB.  */
# define TLS_PRE_TCB_SIZE		sizeof (struct pthread)

/* Install the dtv pointer.  The pointer passed is to the element with
   index -1 which contain the length.  */
# define INSTALL_DTV(tcbp, dtvp)	(((tcbhead_t *) (tcbp))->dtv = (dtvp) + 1)

/* Install new dtv for current thread.  */
# define INSTALL_NEW_DTV(dtv)		(THREAD_DTV() = (dtv))

/* Return dtv of given thread descriptor.  */
# define GET_DTV(tcbp)			(((tcbhead_t *) (tcbp))->dtv)

/* Code to initially initialize the thread pointer.  */
# define TLS_INIT_TP(tcbp)		({__thread_self = (tcbp); NULL;})

/* Return the address of the dtv for the current thread.  */
# define THREAD_DTV()			(((tcbhead_t *)__thread_self)->dtv)

/* Return the thread descriptor for the current thread.  */
# define THREAD_SELF			((struct pthread *) ((char *) __thread_self - TLS_PRE_TCB_SIZE))

/* Value passed to 'clone' for initialization of the thread register.  */
# define TLS_DEFINE_INIT_TP(tp, pd)	void *tp = (char *) (pd) + TLS_PRE_TCB_SIZE

/* Informs libthread_db that the thread pointer is rp4, which is used
 * to know how to do THREAD_SELF.  */
# define DB_THREAD_SELF			REGISTER (64, 64, 68 * 8, -TLS_PRE_TCB_SIZE)

/* Access to data in the thread descriptor is easy.  */
# define THREAD_GETMEM(descr, member) \
  descr->member
# define THREAD_GETMEM_NC(descr, member, idx) \
  descr->member[idx]
# define THREAD_SETMEM(descr, member, value) \
  descr->member = (value)
# define THREAD_SETMEM_NC(descr, member, idx, value) \
  descr->member[idx] = (value)

/* Get and set the global scope generation counter in struct pthread.  */
# define THREAD_GSCOPE_IN_TCB      1
# define THREAD_GSCOPE_FLAG_UNUSED 0
# define THREAD_GSCOPE_FLAG_USED   1
# define THREAD_GSCOPE_FLAG_WAIT   2
# define THREAD_GSCOPE_RESET_FLAG() \
  do									     \
    { int __res								     \
	= atomic_exchange_rel (&THREAD_SELF->header.gscope_flag,	     \
			       THREAD_GSCOPE_FLAG_UNUSED);		     \
      if (__res == THREAD_GSCOPE_FLAG_WAIT)				     \
	lll_futex_wake (&THREAD_SELF->header.gscope_flag, 1, LLL_PRIVATE);   \
    }									     \
  while (0)
# define THREAD_GSCOPE_SET_FLAG() \
  do									     \
    {									     \
      THREAD_SELF->header.gscope_flag = THREAD_GSCOPE_FLAG_USED;	     \
      atomic_write_barrier ();						     \
    }									     \
  while (0)
# define THREAD_GSCOPE_WAIT() \
  GL(dl_wait_lookup_done) ()

#endif /* __ASSEMBLER__ */

#endif	/* _SYSDEPS_DADAO_NPTL_TLS_H_ */
