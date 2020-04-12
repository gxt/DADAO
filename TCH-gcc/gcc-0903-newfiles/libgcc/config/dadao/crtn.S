/*
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 */

% This must be the last file on the link-line, allocating global registers
% from the top.

% Register $254 is the stack-pointer.
sp GREG

% Register $253 is frame-pointer.  It's not supposed to be used in most
% functions.
fp GREG

% $252 is the static chain register; nested functions receive the
% context of the surrounding function through a pointer passed in this
% register.
static_chain GREG
struct_value_reg GREG

% These registers are used to pass state at an exceptional return (C++).
eh_state_3 GREG
eh_state_2 GREG
eh_state_1 GREG
eh_state_0 GREG

#ifdef __DADAO_ABI_GNU__

% Allocate global registers used by the GNU ABI.
gnu_parm_reg_16 GREG
gnu_parm_reg_15 GREG
gnu_parm_reg_14 GREG
gnu_parm_reg_13 GREG
gnu_parm_reg_12 GREG
gnu_parm_reg_11 GREG
gnu_parm_reg_10 GREG
gnu_parm_reg_9 GREG
gnu_parm_reg_8 GREG
gnu_parm_reg_7 GREG
gnu_parm_reg_6 GREG
gnu_parm_reg_5 GREG
gnu_parm_reg_4 GREG
gnu_parm_reg_3 GREG
gnu_parm_reg_2 GREG
gnu_parm_reg_1 GREG

#endif /* __DADAO_ABI_GNU__ */

% Provide last part of _init and _fini.

% The return address is stored in the topmost stored register in the
% register-stack.  We ignore the current value in rJ.  It is probably
% garbage because each fragment of _init and _fini may have their own idea
% of the current stack frame, if they're cut out from a "real" function
% like in gcc/crtstuff.c.

	.section .init,"ax",@progbits
	GETA	$255,0F
	PUT	rJ,$255
	POP	0,0
0H	PUT	rJ,$0
	POP	0,0
	
	.section .fini,"ax",@progbits
	GETA	$255,0F
	PUT	rJ,$255
	POP	0,0
0H	PUT	rJ,$0
	POP	0,0
