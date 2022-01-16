/*
 * Definitions of target machine for GNU compiler, for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef TARGET_OS_CPP_BUILTINS
#define TARGET_OS_CPP_BUILTINS()				\
    do {							\
	builtin_define ("__dadao__");				\
	builtin_define ("__DADAO__");				\
	builtin_define ("__gnu_linux__");			\
	builtin_define_std ("linux");				\
	builtin_define_std ("unix");				\
	builtin_assert ("system=linux");			\
	builtin_assert ("system=unix");				\
	builtin_assert ("system=posix");			\
    } while (0)
#endif

#undef LIB_SPEC
#define LIB_SPEC \
  "%{pthread:-lpthread} \
   %{shared:-lc} \
   %{!shared: %{profile:-lc_p}%{!profile:-lc}}"

#undef CPP_SPEC
#define CPP_SPEC "%{posix:-D_POSIX_SOURCE} %{pthread:-D_REENTRANT}"

/* Show that we need a GP when profiling.  */
#undef TARGET_PROFILING_NEEDS_GP
#define TARGET_PROFILING_NEEDS_GP 1

/* Don't care about faults in the prologue.  */
#undef TARGET_CAN_FAULT_IN_PROLOGUE
#define TARGET_CAN_FAULT_IN_PROLOGUE 1

#undef WCHAR_TYPE
#define WCHAR_TYPE "int"

#ifdef SINGLE_LIBC
#define OPTION_GLIBC  (DEFAULT_LIBC == LIBC_GLIBC)
#define OPTION_UCLIBC (DEFAULT_LIBC == LIBC_UCLIBC)
#define OPTION_BIONIC (DEFAULT_LIBC == LIBC_BIONIC)
#undef OPTION_MUSL
#define OPTION_MUSL   (DEFAULT_LIBC == LIBC_MUSL)
#else
#define OPTION_GLIBC  (linux_libc == LIBC_GLIBC)
#define OPTION_UCLIBC (linux_libc == LIBC_UCLIBC)
#define OPTION_BIONIC (linux_libc == LIBC_BIONIC)
#undef OPTION_MUSL
#define OPTION_MUSL   (linux_libc == LIBC_MUSL)
#endif

/* Determine what functions are present at the runtime;
   this includes full c99 runtime and sincos.  */
#undef TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION linux_libc_has_function

#define TARGET_POSIX_IO

/* Use --as-needed -lgcc_s for eh support.  */
#ifdef HAVE_LD_AS_NEEDED
#define USE_LD_AS_NEEDED 1
#endif

/* Define if long doubles should be mangled as 'g'.  */
#define TARGET_ALTERNATE_LONG_DOUBLE_MANGLING
