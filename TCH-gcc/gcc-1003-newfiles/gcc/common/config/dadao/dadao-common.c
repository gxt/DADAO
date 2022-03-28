/* Common hooks for DADAO.
   Copyright (C) 2000-2019 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"

/* Change optimizations to be performed, depending on the
   optimization level.  */

static const struct default_options dadao_option_optimization_table[] =
  {
    { OPT_LEVELS_2_PLUS, OPT_finline_small_functions , NULL, 0 },
    { OPT_LEVELS_2_PLUS, OPT_fcse_follow_jumps , NULL, 0 },
    { OPT_LEVELS_2_PLUS, OPT_fcode_hoisting , NULL, 0 },
    { OPT_LEVELS_2_PLUS, OPT_fcaller_saves, NULL, 0 },
    { OPT_LEVELS_2_PLUS, OPT_fgcse , NULL, 0 },

    { OPT_LEVELS_NONE, 0, NULL, 0 }
  };

#undef TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE dadao_option_optimization_table

struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
