#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define __TEST_CASE_HEAD__								\
	setrd	rd16, 0;									\
	setrd	rd17, 0;									\
	setrd	rd18, 0;									\
	setrd	rd19, 0;									\
	setrb	rb16, 0;									\
	setrb	rb17, 0;									\
	setrb	rb18, 0;									\
	setrb	rb19, 0;									\
	setrd	RD_EXP1, 0xBEEF;							\
	setrd	RD_EXP2, 0xBEEF;							\
	setrd	RD_RET1, 0xDEAD;							\
	setrd	RD_RET2, 0xDEAD;							\

#define __TEST_CASE_TAIL__								\
	addi	RD_PASS, RD_PASS, 1;

#define __TEST_CASE( testnum, expect1, code... )		\
test_ ## testnum:										\
	__TEST_CASE_HEAD__									\
	setrd	RD_NUMR, testnum;							\
	code;												\
	setrd	RD_EXP1, expect1;							\
	cmpu	RD_FLAG, RD_RET1, RD_EXP1;					\
	brnz	RD_FLAG, ___fail;							\
	__TEST_CASE_TAIL__

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - logic insns
#-----------------------------------------------------------------------

#define _TEST_ORRI( testnum, inst, dest, src1, immu6, _DEST, _SRC1 )			\
	__TEST_CASE(	testnum, dest,												\
		setrd		rd ## _SRC1, src1;											\
		inst		rd ## _DEST, rd ## _SRC1, immu6;							\
		rd2rd		RD_RET1, rd ## _DEST, 1;									\
	)

#define TEST_ORRI_12( testnum, inst, dest, src1, immu6 )	_TEST_ORRI( testnum, inst, dest, src1, immu6, 16, 17 )
#define TEST_ORRI_11( testnum, inst, dest, src1, immu6 )	_TEST_ORRI( testnum, inst, dest, src1, immu6, 16, 16 )
#define TEST_ORRI_10( testnum, inst, dest, src1, immu6 )	_TEST_ORRI( testnum, inst, dest, src1, immu6, 16,  0 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - multiple regs insns
#-----------------------------------------------------------------------

#define _TEST_ORRI_R1( testnum, inst, dst, src, _RGHB, _RGHC, _DST, _SRC )		\
	__TEST_CASE(	testnum, dst,									\
		set ## _RGHB	_RGHB ## _DST, 0xdead;									\
		set ## _RGHC	_RGHC ## _SRC, src;										\
		inst			_RGHB ## _DST, _RGHC ## _SRC, 1;						\
		_RGHB ## 2rd	RD_RET1, _RGHB ## _DST, 1;								\
	)

#define TEST_ORRI_R1_12( testnum, inst, dst, src, rgd, rgs )		_TEST_ORRI_R1( testnum, inst, dst, src, rgd, rgs, 16, 17 )
#define TEST_ORRI_R1_11( testnum, inst, dst, src, rgd, rgs )		_TEST_ORRI_R1( testnum, inst, dst, src, rgd, rgs, 16, 16 )
#define TEST_ORRI_R1_10( testnum, inst, dst, src, rgd, rgs )		_TEST_ORRI_R1( testnum, inst, dst, src, rgd, rgs, 16,  0 )

#define _TEST_ORRI_R8( testnum, inst, _RGHB, _RGHC, _DST0, _SRC, _DST ... )		\
test_ ## testnum:																\
		__TEST_CASE_HEAD__														\
		setrd			RD_NUMR, testnum;										\
	.irp	rn, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31;				\
		set ## _RGHC	_RGHC##\rn, \rn;										\
	.endr;																		\
		inst			_RGHB ## _DST0, _RGHC ## _SRC, 8;						\
		setrd			RD_EXP1, _SRC;											\
	.irp	rn, _DST;															\
		_RGHB ## 2rd	RD_RET1, _RGHB##\rn, 1;									\
		cmpu			RD_FLAG, RD_RET1, RD_EXP1;								\
		brnz			RD_FLAG, ___fail;										\
		addi			RD_EXP1, RD_EXP1, 1;									\
	.endr;																		\
		__TEST_CASE_TAIL__

#define TEST_ORRI_R8_C1( testnum, inst, rgd, rgs )		_TEST_ORRI_R8( testnum, inst, rgd, rgs,  8, 16,  8, 9,10,11,12,13,14,15 )
#define TEST_ORRI_R8_C2( testnum, inst, rgd, rgs )		_TEST_ORRI_R8( testnum, inst, rgd, rgs, 12, 16, 12,13,14,15,16,17,18,19 )
#define TEST_ORRI_R8_C3( testnum, inst, rgd, rgs )		_TEST_ORRI_R8( testnum, inst, rgd, rgs, 20, 16, 20,21,22,23,24,25,26,27 )
#define TEST_ORRI_R8_C4( testnum, inst, rgd, rgs )		_TEST_ORRI_R8( testnum, inst, rgd, rgs, 24, 16, 24,25,26,27,28,29,30,31 )
#define TEST_ORRI_R8_C5( testnum, inst, rgd, rgs )		_TEST_ORRI_R8( testnum, inst, rgd, rgs, 20, 20, 20,21,22,23,24,25,26,27 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - floating pointer insns
#	TODO: SHOULD watch accured exception
#-----------------------------------------------------------------------

#define _TEST_FRRI_R1( testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, _RGHB, _RGHC, _DST, _SRC )	\
test_ ## testnum:																				\
		__TEST_CASE_HEAD__																		\
		setrd			RD_NUMR, testnum;														\
		setrb			RB_SRC1, test_ ## testnum ## _src;										\
		inst_ld_src		_RGHC ## _SRC, RB_SRC1, 0;												\
		inst			_RGHB ## _DST, _RGHC ## _SRC, 1;										\
		jump			lbl_ ## testnum ## _cmp;												\
		.balign			8;																		\
test_ ## testnum ## _dst:																		\
		type_dst		val_dst;																\
		.balign			8;																		\
test_ ## testnum ## _src:																		\
		type_src		val_src;																\
		.balign			4;																		\
lbl_ ## testnum ## _cmp:																		\
		_RGHB ## 2rd	RD_RET1, _RGHB ## _DST, 1;												\
		setrb			RB_DST1, test_ ## testnum ## _dst;										\
		inst_ld_dst		_RGHB ## _DST, RB_DST1, 0;												\
		_RGHB ## 2rd	RD_EXP1, _RGHB ## _DST, 1;												\
		cmpu			RD_FLAG, RD_RET1, RD_EXP1;												\
		brnz			RD_FLAG, ___fail;														\
		__TEST_CASE_TAIL__
 
#define TEST_FRRI_R1_12( testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_R1(   testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16, 17 )
#define TEST_FRRI_R1_11( testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_R1(   testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16, 16 )
#define TEST_FRRI_R1_10( testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_R1(   testnum, inst, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16,  0 )
 
#-----------------------------------------------------------------------
# DADAO MACROS for ORRR
#-----------------------------------------------------------------------

#define _TEST_ORRR( testnum, inst, dest, src1, src2, _RGHB, _RGHC, _RGHD, _DEST, _SRC1, _SRC2 )		\
	__TEST_CASE(	testnum, dest,																	\
	set ## _RGHC	_RGHC ## _SRC1, src1;															\
	set ## _RGHD	_RGHD ## _SRC2, src2;															\
	inst			_RGHB ## _DEST, _RGHC ## _SRC1, _RGHD ## _SRC2;									\
	_RGHB ## 2rd	RD_RET1, _RGHB ## _DEST, 1;														\
	)

#define TEST_ORRR_DDD_123( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16, 17, 18 )
#define TEST_ORRR_DDD_112( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16, 16, 18 )
#define TEST_ORRR_DDD_121( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16, 17, 16 )
#define TEST_ORRR_DDD_111( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16, 16, 16 )
#define TEST_ORRR_DDD_102( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16,  0, 18 )
#define TEST_ORRR_DDD_120( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16, 17,  0 )
#define TEST_ORRR_DDD_100( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd, 16,  0,  0 )
#define TEST_ORRR_DDD_012( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rd, rd,  0, 17, 18 )

#define TEST_ORRR_BBD_123( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rb, rb, rd, 16, 17, 18 )
#define TEST_ORRR_BBD_112( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rb, rb, rd, 16, 16, 18 )
#define TEST_ORRR_BBD_120( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rb, rb, rd, 16, 17,  0 )

#define TEST_ORRR_DBB_123( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rb, rb, 16, 17, 18 )
#define TEST_ORRR_DBB_122( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rb, rb, 16, 17, 17 )
#define TEST_ORRR_DBB_012( testnum, inst, dest, src1, src2 )	_TEST_ORRR( testnum, inst, dest, src1, src2, rd, rb, rb,  0, 16, 17 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRR - floating pointer insns
#	TODO: SHOULD watch accured exception
#-----------------------------------------------------------------------

#define _TEST_FRRR_FFF( testnum, inst, inst_ld, ftype, val_dst, val_src1, val_src2, _DST, _SRC1, _SRC2 )	\
test_ ## testnum:																				\
		__TEST_CASE_HEAD__																		\
		setrd			RD_NUMR, testnum;														\
		setrb			RB_SRC1, test_ ## testnum ## _src1;										\
		inst_ld			rf ## _SRC1, RB_SRC1, 0;												\
		setrb			RB_SRC2, test_ ## testnum ## _src2;										\
		inst_ld			rf ## _SRC2, RB_SRC2, 0;												\
		inst			rf ## _DST, rf ## _SRC1, rf ## _SRC2;									\
		jump			lbl_ ## testnum ## _cmp;												\
		.balign			8;																		\
test_ ## testnum ## _dst:																		\
		ftype			val_dst;																\
		.balign			8;																		\
test_ ## testnum ## _src1:																		\
		ftype			val_src1;																\
		.balign			8;																		\
test_ ## testnum ## _src2:																		\
		ftype			val_src2;																\
		.balign			4;																		\
lbl_ ## testnum ## _cmp:																		\
		rf2rd			RD_RET1, rf ## _DST, 1;													\
		setrb			RB_DST1, test_ ## testnum ## _dst;										\
		inst_ld			rf ## _DST, RB_DST1, 0;													\
		rf2rd			RD_EXP1, rf ## _DST, 1;													\
		cmpu			RD_FLAG, RD_RET1, RD_EXP1;												\
		brnz			RD_FLAG, ___fail;														\
		__TEST_CASE_TAIL__

#define TEST_FRRR_FFF_FT_123( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldft, .single, val_dst, val_src1, val_src2, 16, 17, 18 )
#define TEST_FRRR_FFF_FT_112( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldft, .single, val_dst, val_src1, val_src2, 16, 16, 18 )
#define TEST_FRRR_FFF_FT_121( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldft, .single, val_dst, val_src1, val_src2, 16, 17, 16 )
#define TEST_FRRR_FFF_FT_111( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldft, .single, val_dst, val_src1, val_src2, 16, 16, 16 )

#define TEST_FRRR_FFF_FO_123( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldfo, .double, val_dst, val_src1, val_src2, 16, 17, 18 )
#define TEST_FRRR_FFF_FO_112( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldfo, .double, val_dst, val_src1, val_src2, 16, 16, 18 )
#define TEST_FRRR_FFF_FO_121( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldfo, .double, val_dst, val_src1, val_src2, 16, 17, 16 )
#define TEST_FRRR_FFF_FO_111( testnum, inst, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   testnum, inst, ldfo, .double, val_dst, val_src1, val_src2, 16, 16, 16 )

#-----------------------------------------------------------------------
# DADAO MACROS for RIII
#-----------------------------------------------------------------------

#define _TEST_RIII( testnum, inst, dest, imm18, _DEST )							\
    __TEST_CASE(	testnum, dest,												\
		inst		rb ## _DEST, imm18;											\
		rb2rd		RD_RET1, rb ## _DEST, 1;									\
    )

#define TEST_RIII( testnum, inst, dest, imm18 )				_TEST_RIII( testnum, inst, dest, imm18, 16 )

#-----------------------------------------------------------------------
# DADAO MACROS for RRII
#-----------------------------------------------------------------------

#define _TEST_RRII( testnum, inst, dest, src1, imm12, _RGHA, _RGHB, _DEST, _SRC1 )		\
    __TEST_CASE(	testnum, dest,														\
		set ## _RGHB	_RGHB ## _SRC1, src1;											\
		inst			_RGHA ## _DEST, _RGHB ## _SRC1, imm12;							\
		_RGHA ## 2rd	RD_RET1, _RGHA ## _DEST, 1;										\
    )

#define TEST_RRII_DD_12( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rd, 16, 17 )
#define TEST_RRII_DD_11( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rd, 16, 16 )
#define TEST_RRII_DD_10( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rd, 16,  0 )
#define TEST_RRII_DD_01( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rd,  0, 16 )

#define TEST_RRII_BB_12( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rb, rb, 16, 17 )
#define TEST_RRII_BB_11( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rb, rb, 16, 16 )

#define TEST_RRII_LD_D_12( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rb, 16, 17 )
#define TEST_RRII_LD_D_01( testnum, inst, dest, src1, imm12 )		_TEST_RRII( testnum, inst, dest, src1, imm12, rd, rb,  0, 17 )

#define _TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else, _SRC1, _SRC2 )	\
test_ ## testnum:														\
	__TEST_CASE_HEAD__													\
	setrd	RD_NUMR, testnum;											\
	setrd	rd ## _SRC1, src1;											\
	setrd	rd ## _SRC2, src2;											\
	inst	rd ## _SRC1, rd ## _SRC2, lbl_ ## testnum ## _taken;		\
	jump	lbl_ ## testnum ## lbl_else;								\
lbl_ ## testnum ## _taken:												\
	jump	lbl_ ## testnum ## lbl_then;								\
lbl_ ## testnum ## _fail:												\
	jump	___fail;													\
lbl_ ## testnum ## _pass:												\
	setrd	RD_FLAG, RD_ZERO;											\
	__TEST_CASE_TAIL__

#define TEST_RRII_BR_12( testnum, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else, 16, 17 )
#define TEST_RRII_BR_11( testnum, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else, 16, 16 )
#define TEST_RRII_BR_10( testnum, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else, 16,  0 )
#define TEST_RRII_BR_01( testnum, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else,  0, 16 )
#define TEST_RRII_BR_00( testnum, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR( testnum, inst, src1, src2, lbl_then, lbl_else,  0,  0 )

#-----------------------------------------------------------------------
# DADAO MACROS for RRRR
#-----------------------------------------------------------------------

#define _TEST_RRRR_RRWR( testnum, inst, dest, cond1, cond2, orig, src, _COND1, _COND2, _DEST, _SRC )		\
	__TEST_CASE(	testnum, dest,																			\
		setrd		rd ## _COND1, cond1;																	\
		setrd		rd ## _COND2, cond2;																	\
		setrd		rd ## _DEST, orig;																		\
		setrd		rd ## _SRC, src;																		\
		inst		rd ## _COND1, rd ## _COND2, rd ## _DEST, rd ## _SRC;									\
		rd2rd		RD_RET1, rd ## _DEST, 1;																\
	)

#define TEST_RRRR_RRWR_1234( testnum, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( testnum, inst, dest, cond1, cond2, orig, src, 16, 17, 18, 19 )
#define TEST_RRRR_RRWR_0123( testnum, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( testnum, inst, dest, cond1, cond2, orig, src,  0, 17, 18, 19 )
#define TEST_RRRR_RRWR_1023( testnum, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( testnum, inst, dest, cond1, cond2, orig, src, 16,  0, 18, 19 )
#define TEST_RRRR_RRWR_1230( testnum, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( testnum, inst, dest, cond1, cond2, orig, src, 16, 17, 18,  0 )

#define _TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, _COND, _DEST, _SRC1, _SRC2 )				\
	__TEST_CASE(	testnum, dest,																			\
		setrd		rd ## _COND, cond;																		\
		setrd		rd ## _SRC1, src1;																		\
		setrd		rd ## _SRC2, src2;																		\
		inst		rd ## _COND, rd ## _DEST, rd ## _SRC1, rd ## _SRC2;										\
		rd2rd		RD_RET1, rd ## _DEST, 1;																\
	)

#define TEST_RRRR_RWRR_1234( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 17, 18, 19 )
#define TEST_RRRR_RWRR_1123( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 16, 18, 19 )
#define TEST_RRRR_RWRR_1223( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 18, 18, 19 )
#define TEST_RRRR_RWRR_1232( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 17, 18, 17 )
#define TEST_RRRR_RWRR_0123( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2,  0, 17, 18, 19 )
#define TEST_RRRR_RWRR_1203( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 17,  0, 19 )
#define TEST_RRRR_RWRR_1230( testnum, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( testnum, inst, dest, cond, src1, src2, 16, 17, 18,  0 )

#define _TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, _DEST1, _DEST2, _SRC1, _SRC2 )			\
test_ ## testnum:														\
	__TEST_CASE_HEAD__													\
	setrd	RD_NUMR, testnum;											\
	setrd	rd ## _SRC1, src1;											\
	setrd	rd ## _SRC2, src2;											\
	inst	rd ## _DEST1, rd ## _DEST2, rd ## _SRC1, rd ## _SRC2;		\
	rd2rd	RD_RET1, rd ## _DEST1, 1;									\
	rd2rd	RD_RET2, rd ## _DEST2, 1;									\
	setrd	RD_EXP1, dest1;												\
	setrd	RD_EXP2, dest2;												\
	cmpu	RD_FLAG, RD_RET1, RD_EXP1;									\
	brnz	RD_FLAG, ___fail;											\
	cmpu	RD_FLAG, RD_RET2, RD_EXP2;									\
	brnz	RD_FLAG, ___fail;											\
	__TEST_CASE_TAIL__

#define TEST_RRRR_WWRR_1234( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 18, 19 )
#define TEST_RRRR_WWRR_1213( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 16, 19 )
#define TEST_RRRR_WWRR_1223( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 17, 19 )
#define TEST_RRRR_WWRR_1231( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 18, 16 )
#define TEST_RRRR_WWRR_1232( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 18, 17 )
#define TEST_RRRR_WWRR_1211( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 16, 16 )
#define TEST_RRRR_WWRR_1222( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 17, 17 )
#define TEST_RRRR_WWRR_1203( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17,  0, 19 )
#define TEST_RRRR_WWRR_1230( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17, 18,  0 )
#define TEST_RRRR_WWRR_1200( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16, 17,  0,  0 )
#define TEST_RRRR_WWRR_0123( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2,  0, 17, 18, 19 )
#define TEST_RRRR_WWRR_1023( testnum, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( testnum, inst, dest1, dest2, src1, src2, 16,  0, 18, 19 )

#-----------------------------------------------------------------------
# DADAO MACROS for RWII
#-----------------------------------------------------------------------

#define _TEST_RWII( testnum, inst, wyde, dest, orig, imm16, _RGHA, _DEST )		\
    __TEST_CASE(	testnum, dest,											\
		set ## _RGHA	_RGHA ## _DEST, orig;								\
		inst			_RGHA ## _DEST, wyde, imm16;						\
		_RGHA ## 2rd	RD_RET1, _RGHA ## _DEST, 1;							\
    )

#define TEST_RWII_D( testnum, inst, wyde, dest, orig, imm16 )		_TEST_RWII( testnum, inst, wyde, dest, orig, imm16, rd, 16 )
#define TEST_RWII_B( testnum, inst, wyde, dest, orig, imm16 )		_TEST_RWII( testnum, inst, wyde, dest, orig, imm16, rb, 16 )
#define TEST_RWII_F( testnum, inst, wyde, dest, orig, imm16 )		_TEST_RWII( testnum, inst, wyde, dest, orig, imm16, rf, 16 )

#endif /* __TEST_MACROS_SCALAR_H */
