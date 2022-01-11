/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

#include <sysdep.h>
#include <libc-symbols.h>

	.text
ENTRY(_dl_runtime_resolve)
	swym
	ret
END(_dl_runtime_resolve)

#ifndef	PROF
ENTRY(_dl_runtime_profile)
	swym
	ret
END(_dl_runtime_profile)
#endif
	.previous
