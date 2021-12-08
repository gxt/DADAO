divert(-1)

define(__error, `exit(1);')

define(insn, `dnl
ifelse(	$#, `9', `', `__error')dnl
  { "$1",	ifelse(	$2, `iiii',	1,
			$2, `zzzz',	0,
			$2, `oiii',	1,
			$2, `orr0',	2,
			$2, `orri',	3,
			$2, `orrr',	3,
			$2, `riii',	2,
			$2, `rjii',	2,
			$2, `rrii',	3,
			$2, `rrri',	3,
			$2, `rrrr',	4, `__error')dnl
,	$3,	$4,	ifelse( $5, `-', _OP(none), _OP($5))dnl
,	ifelse( $6, `-', _OP(none), _OP($6))dnl
,	ifelse( $7, `-', _OP(none), _OP($7))dnl
,	ifelse( $8, `-', _OP(none), _OP($8))dnl
,	_T($9)},')

include(dadao-opcodes.m4)
