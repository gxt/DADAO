# This shell script emits a C file. -*- C -*-
#   Copyright (C) 2001-2019 Free Software Foundation, Inc.
#
# This file is part of the GNU Binutils.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
# MA 02110-1301, USA.
#

# This file is sourced from elf.em and used to define DADAO and ELF
# specific things.

fragment <<EOF

#include "elf/dadao.h"

static void gld${EMULATION_NAME}_before_parse (void);

/* Set up handling of linker-allocated global registers.  */

static void
dadao_before_allocation (void)
{
  /* Call the default first.  */
  gld${EMULATION_NAME}_before_allocation ();

  /* There's a needrelax.em which uses this ..._before_allocation-hook and
     just has the statement below as payload.  It's more of a hassle to
     use that than to just include these two lines and take the
     maintenance burden to keep them in sync.  (Of course we lose the
     maintenance burden of checking that it still does what we need.)  */

  /* Force -relax on (regardless of whether we're doing a relocatable
     link).  */
  /* ENABLE_RELAXATION; */
}

static void
dadao_before_parse (void)
{
  link_info.check_relocs_after_open_input = TRUE;
  gld${EMULATION_NAME}_before_parse ();

  config.separate_code = `if test "x${SEPARATE_CODE}" = xyes ; then echo TRUE ; else echo FALSE ; fi`;
}
EOF

LDEMUL_BEFORE_PARSE=dadao_before_parse
LDEMUL_BEFORE_ALLOCATION=dadao_before_allocation
