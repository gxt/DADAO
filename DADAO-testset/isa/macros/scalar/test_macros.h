#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define __TEST_CASE_HEAD__(tcname)						\
	test_ ## tcname:									\
		setrd			rd16, 0;						\
		setrd			rd17, 0;						\
		setrd			rd18, 0;						\
		setrd			rd19, 0;						\
		setrb			rb16, 0;						\
		setrb			rb17, 0;						\
		setrb			rb18, 0;						\
		setrb			rb19, 0;						\
		setw			RF_FCSR, w0, 0;					\
		setrd			RD_EXP1, 0xBEEF;				\
		setrd			RD_EXP2, 0xBEEF;				\
		setrd			RD_RET1, 0xDEAD;				\
		setrd			RD_RET2, 0xDEAD;				\
		setrd			RD_FLAG, 0;						\
		setrd			RD_NAME, tcname;				\
		addi			RD_PASS, RD_PASS, 1;			\

#define __PASS_FAIL_FCSR__(flags)						\
		setrd			RD_RET2, RF_FCSR;				\
		andi			RD_RET2, RD_RET2, 0x1F;			\
		setrd			RD_EXP2, flags;					\
		brne			RD_EXP2, RD_RET2, ___fail;		\

#define __TEST_CASE( tcname, dest, code... )			\
		__TEST_CASE_HEAD__(tcname)						\
		code;											\
		setrd			RD_EXP1, dest;					\
		brne			RD_EXP1, RD_RET1, ___fail;		\

#define __TEST_FCSR_SET_RMODE( rmode )					\
		setw			RF_FCSR, w1, rmode;				\

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - logic insns
#-----------------------------------------------------------------------

#define _TEST_ORRI( tcname, inst, dest, src1, immu6, _DEST, _SRC1 )			\
	__TEST_CASE(	tcname, dest,											\
		setrd		rd ## _SRC1, src1;										\
		inst		rd ## _DEST, rd ## _SRC1, immu6;						\
		rd2rd		RD_RET1, rd ## _DEST, 1;								\
	)

#define TEST_ORRI_12( tcname, inst, dest, src1, immu6 )	_TEST_ORRI( tcname, inst, dest, src1, immu6, 16, 17 )
#define TEST_ORRI_11( tcname, inst, dest, src1, immu6 )	_TEST_ORRI( tcname, inst, dest, src1, immu6, 16, 16 )
#define TEST_ORRI_10( tcname, inst, dest, src1, immu6 )	_TEST_ORRI( tcname, inst, dest, src1, immu6, 16,  0 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - multiple regs insns
#-----------------------------------------------------------------------

#define _TEST_ORRI_M1( tcname, inst, dst, src, _RGHB, _RGHC, _DST, _SRC )		\
	__TEST_CASE(	tcname, dst,									\
		setrd			RD_TMP, src;								\
		rd2 ## _RGHC	_RGHC ## _SRC, RD_TMP, 1;					\
		inst			_RGHB ## _DST, _RGHC ## _SRC, 1;			\
		_RGHB ## 2rd	RD_RET1, _RGHB ## _DST, 1;					\
	)

#define TEST_ORRI_M1_12( tcname, inst, dst, src, rgd, rgs )		_TEST_ORRI_M1( tcname, inst, dst, src, rgd, rgs, 16, 17 )
#define TEST_ORRI_M1_11( tcname, inst, dst, src, rgd, rgs )		_TEST_ORRI_M1( tcname, inst, dst, src, rgd, rgs, 16, 16 )
#define TEST_ORRI_M1_10( tcname, inst, dst, src, rgd, rgs )		_TEST_ORRI_M1( tcname, inst, dst, src, rgd, rgs, 16,  0 )

#define _TEST_ORRI_M8( tcname, inst, _RGHB, _RGHC, _DST0, _SRC, _DST ... )		\
		__TEST_CASE_HEAD__(tcname)									\
	.irp	rn, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31;	\
		set ## _RGHC	_RGHC##\rn, \rn;							\
	.endr;															\
		inst			_RGHB ## _DST0, _RGHC ## _SRC, 8;			\
		setrd			RD_EXP1, _SRC;								\
	.irp	rn, _DST;												\
		_RGHB ## 2rd	RD_RET1, _RGHB##\rn, 1;						\
		brne			RD_RET1, RD_EXP1, ___fail;					\
		addi			RD_EXP1, RD_EXP1, 1;						\
	.endr;															\

#define TEST_ORRI_M8_C1( tcname, inst, rgd, rgs )		_TEST_ORRI_M8( tcname, inst, rgd, rgs,  8, 16,  8, 9,10,11,12,13,14,15 )
#define TEST_ORRI_M8_C2( tcname, inst, rgd, rgs )		_TEST_ORRI_M8( tcname, inst, rgd, rgs, 12, 16, 12,13,14,15,16,17,18,19 )
#define TEST_ORRI_M8_C3( tcname, inst, rgd, rgs )		_TEST_ORRI_M8( tcname, inst, rgd, rgs, 20, 16, 20,21,22,23,24,25,26,27 )
#define TEST_ORRI_M8_C4( tcname, inst, rgd, rgs )		_TEST_ORRI_M8( tcname, inst, rgd, rgs, 24, 16, 24,25,26,27,28,29,30,31 )
#define TEST_ORRI_M8_C5( tcname, inst, rgd, rgs )		_TEST_ORRI_M8( tcname, inst, rgd, rgs, 20, 20, 20,21,22,23,24,25,26,27 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRI - floating pointer insns
#	TODO: SHOULD watch accured exception
#-----------------------------------------------------------------------

#define _TEST_FRRI_M1( tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, _RGHB, _RGHC, _DST, _SRC )	\
		__TEST_CASE_HEAD__(tcname)									\
		setrb			RB_SRC, test_ ## tcname ## _src;			\
		inst_ld_src		_RGHC ## _SRC, RB_SRC, 0;					\
		inst			_RGHB ## _DST, _RGHC ## _SRC, 1;			\
		jump			lbl_ ## tcname ## _cmp;						\
		.balign			8;											\
test_ ## tcname ## _dst:											\
		type_dst		val_dst;									\
		.balign			8;											\
test_ ## tcname ## _src:											\
		type_src		val_src;									\
		.balign			4;											\
lbl_ ## tcname ## _cmp:												\
		_RGHB ## 2rd	RD_RET1, _RGHB ## _DST, 1;					\
		setrb			RB_DST, test_ ## tcname ## _dst;			\
		inst_ld_dst		_RGHB ## _DST, RB_DST, 0;					\
		_RGHB ## 2rd	RD_EXP1, _RGHB ## _DST, 1;					\
		brne			RD_RET1, RD_EXP1, ___fail;					\
		__PASS_FAIL_FCSR__(flags)									\
 
#define TEST_FRRI_M1_12( tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_M1(   tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16, 17 )
#define TEST_FRRI_M1_11( tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_M1(   tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16, 16 )
#define TEST_FRRI_M1_10( tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs )	\
		_TEST_FRRI_M1(   tcname, inst, flags, inst_ld_dst, type_dst, val_dst, inst_ld_src, type_src, val_src, rgd, rgs, 16,  0 )
 
#-----------------------------------------------------------------------
# DADAO MACROS for ORRR
#-----------------------------------------------------------------------

#define _TEST_ORRR( tcname, inst, dest, src1, src2, _RGHB, _RGHC, _RGHD, _DEST, _SRC1, _SRC2 )		\
	__TEST_CASE(	tcname, dest,										\
	set ## _RGHC	_RGHC ## _SRC1, src1;								\
	set ## _RGHD	_RGHD ## _SRC2, src2;								\
	inst			_RGHB ## _DEST, _RGHC ## _SRC1, _RGHD ## _SRC2;		\
	_RGHB ## 2rd	RD_RET1, _RGHB ## _DEST, 1;							\
	)

#define TEST_ORRR_DDD_123( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16, 17, 18 )
#define TEST_ORRR_DDD_112( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16, 16, 18 )
#define TEST_ORRR_DDD_121( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16, 17, 16 )
#define TEST_ORRR_DDD_111( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16, 16, 16 )
#define TEST_ORRR_DDD_102( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16,  0, 18 )
#define TEST_ORRR_DDD_120( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16, 17,  0 )
#define TEST_ORRR_DDD_100( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd, 16,  0,  0 )
#define TEST_ORRR_DDD_012( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rd, rd,  0, 17, 18 )

#define TEST_ORRR_BBD_123( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rb, rb, rd, 16, 17, 18 )
#define TEST_ORRR_BBD_112( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rb, rb, rd, 16, 16, 18 )
#define TEST_ORRR_BBD_120( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rb, rb, rd, 16, 17,  0 )

#define TEST_ORRR_DBB_123( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rb, rb, 16, 17, 18 )
#define TEST_ORRR_DBB_122( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rb, rb, 16, 17, 17 )
#define TEST_ORRR_DBB_012( tcname, inst, dest, src1, src2 )	_TEST_ORRR( tcname, inst, dest, src1, src2, rd, rb, rb,  0, 16, 17 )

#-----------------------------------------------------------------------
# DADAO MACROS for ORRR - floating pointer insns
#	TODO: SHOULD watch accured exception
#-----------------------------------------------------------------------

#define _TEST_FRRR_FFF( tcname, inst, flags, inst_ld, ftype, val_dst, val_src1, val_src2, _DST, _SRC1, _SRC2 )	\
		__TEST_CASE_HEAD__(tcname)										\
		setrb			RB_SRC, test_ ## tcname ## _src1;				\
		inst_ld			rf ## _SRC1, RB_SRC, 0;							\
		setrb			RB_SRC, test_ ## tcname ## _src2;				\
		inst_ld			rf ## _SRC2, RB_SRC, 0;							\
		inst			rf ## _DST, rf ## _SRC1, rf ## _SRC2;			\
		jump			lbl_ ## tcname ## _cmp;							\
		.balign			8;												\
test_ ## tcname ## _dst:												\
		ftype			val_dst;										\
		.balign			8;												\
test_ ## tcname ## _src1:												\
		ftype			val_src1;										\
		.balign			8;												\
test_ ## tcname ## _src2:												\
		ftype			val_src2;										\
		.balign			4;												\
lbl_ ## tcname ## _cmp:													\
		rf2rd			RD_RET1, rf ## _DST, 1;							\
		setrb			RB_DST, test_ ## tcname ## _dst;				\
		inst_ld			rf ## _DST, RB_DST, 0;							\
		rf2rd			RD_EXP1, rf ## _DST, 1;							\
		brne			RD_RET1, RD_EXP1, ___fail;						\
		__PASS_FAIL_FCSR__(flags)										\

#define TEST_FRRR_FFF_FT_123( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 17, 18 )
#define TEST_FRRR_FFF_FT_112( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 16, 18 )
#define TEST_FRRR_FFF_FT_121( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 17, 16 )
#define TEST_FRRR_FFF_FT_111( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 16, 16 )
#define TEST_FRRR_FFF_FT_120( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 17,  0 )

#define TEST_FRRR_FFF_FO_123( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 17, 18 )
#define TEST_FRRR_FFF_FO_112( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 16, 18 )
#define TEST_FRRR_FFF_FO_121( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 17, 16 )
#define TEST_FRRR_FFF_FO_111( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 16, 16 )
#define TEST_FRRR_FFF_FO_120( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_FFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 17,  0 )

#define _TEST_FRRR_DFF( tcname, inst, flags, inst_ld, ftype, val_dst, val_src1, val_src2, _DST, _SRC1, _SRC2 )	\
		__TEST_CASE_HEAD__(tcname)										\
		setrb			RB_SRC, test_ ## tcname ## _src1;				\
		inst_ld			rf ## _SRC1, RB_SRC, 0;							\
		setrb			RB_SRC, test_ ## tcname ## _src2;				\
		inst_ld			rf ## _SRC2, RB_SRC, 0;							\
		inst			rd ## _DST, rf ## _SRC1, rf ## _SRC2;			\
		jump			lbl_ ## tcname ## _cmp;							\
		.balign			8;												\
test_ ## tcname ## _src1:												\
		ftype			val_src1;										\
		.balign			8;												\
test_ ## tcname ## _src2:												\
		ftype			val_src2;										\
		.balign			4;												\
lbl_ ## tcname ## _cmp:													\
		setrd			RD_RET1, rd ## _DST;							\
		setrd			RD_EXP1, val_dst;								\
		brne			RD_RET1, RD_EXP1, ___fail;						\
		__PASS_FAIL_FCSR__(flags)										\

#define TEST_FRRR_DFF_FT_123( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_DFF(   tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, 16, 17, 18 )

#define TEST_FRRR_DFF_FO_123( tcname, inst, flags, val_dst, val_src1, val_src2 )	\
			_TEST_FRRR_DFF(   tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, 16, 17, 18 )

#define _TEST_FRRR_RWR( tcname, inst, dest, cond, orig, src, _COND, _DEST, _SRC )		\
	__TEST_CASE(	tcname, dest,											\
		setrd		rd ## _COND, cond;										\
		setrf		rf ## _DEST, orig;										\
		setrf		rf ## _SRC, src;										\
		inst		rd ## _COND, rf ## _DEST, rf ## _SRC;					\
		rf2rd		RD_RET1, rf ## _DEST, 1;								\
	)

#define TEST_FRRR_RWR_123( tcname, inst, dest, cond, orig, src )		_TEST_FRRR_RWR( tcname, inst, dest, cond, orig, src, 16, 17, 18 )
#define TEST_FRRR_RWR_012( tcname, inst, dest, cond, orig, src )		_TEST_FRRR_RWR( tcname, inst, dest, cond, orig, src,  0, 17, 18 )

#-----------------------------------------------------------------------
# DADAO MACROS for RIII
#-----------------------------------------------------------------------

#define _TEST_RIII( tcname, inst, imm18, _DEST )						\
	__TEST_CASE_HEAD__(tcname)											\
	setrd	RD_EXP1, RB_NEXT;											\
	inst	rb ## _DEST, imm18;											\
	setrd	RD_RET1, rb ## _DEST;										\
	shru	RD_EXP1, RD_EXP1, 12;										\
	setrd	RD_EXP2, imm18;												\
	add		RD_ZERO, RD_EXP1, RD_EXP1, RD_EXP2;							\
	shlu	RD_EXP1, RD_EXP1, 12;										\
	brne	RD_RET1, RD_EXP1, ___fail;									\

#define TEST_RIII( tcname, inst, imm18 )				_TEST_RIII( tcname, inst, imm18, 16 )

#define _TEST_RIII_BR_HIGH( tcname, inst, src1, lbl_then, lbl_else, _SRC1 )	\
	__TEST_CASE_HEAD__(tcname)											\
	setrd	rd ## _SRC1, src1;											\
	inst	rd ## _SRC1, lbl_ ## tcname ## _taken;						\
	jump	lbl_ ## tcname ## lbl_else;									\
lbl_ ## tcname ## _taken:												\
	jump	lbl_ ## tcname ## lbl_then;									\
lbl_ ## tcname ## _fail:												\
	jump	___fail;													\
lbl_ ## tcname ## _pass:												\
	setrd	RD_FLAG, RD_ZERO;											\

#define TEST_RIII_BR_H( tcname, inst, src1, lbl_then, lbl_else )		_TEST_RIII_BR_HIGH( tcname, inst, src1, lbl_then, lbl_else, 16 )

#define _TEST_RIII_BR_LOW( tcname, inst, src1, lbl_then, lbl_else, _SRC1 )	\
	__TEST_CASE_HEAD__(tcname)											\
	jump	lbl_ ## tcname ## _test;									\
lbl_ ## tcname ## _taken:												\
	jump	lbl_ ## tcname ## lbl_then;									\
lbl_ ## tcname ## _test:												\
	setrd	rd ## _SRC1, src1;											\
	inst	rd ## _SRC1, lbl_ ## tcname ## _taken;						\
	jump	lbl_ ## tcname ## lbl_else;									\
lbl_ ## tcname ## _fail:												\
	jump	___fail;													\
lbl_ ## tcname ## _pass:												\
	setrd	RD_FLAG, RD_ZERO;											\

#define TEST_RIII_BR_L( tcname, inst, src1, lbl_then, lbl_else )		_TEST_RIII_BR_LOW( tcname, inst, src1, lbl_then, lbl_else, 16 )

#-----------------------------------------------------------------------
# DADAO MACROS for RRII
#-----------------------------------------------------------------------

#define _TEST_RRII( tcname, inst, dest, src1, imm12, _RGHA, _RGHB, _DEST, _SRC1 )		\
    __TEST_CASE(	tcname, dest,										\
		set ## _RGHB	_RGHB ## _SRC1, src1;							\
		inst			_RGHA ## _DEST, _RGHB ## _SRC1, imm12;			\
		_RGHA ## 2rd	RD_RET1, _RGHA ## _DEST, 1;						\
    )

#define TEST_RRII_DD_12( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rd, 16, 17 )
#define TEST_RRII_DD_11( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rd, 16, 16 )
#define TEST_RRII_DD_10( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rd, 16,  0 )
#define TEST_RRII_DD_01( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rd,  0, 16 )

#define TEST_RRII_BB_12( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rb, rb, 16, 17 )
#define TEST_RRII_BB_11( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rb, rb, 16, 16 )

#define TEST_RRII_LD_D_12( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rb, 16, 17 )
#define TEST_RRII_LD_B_12( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rb, rb, 16, 17 )
#define TEST_RRII_LD_F_12( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rf, rb, 16, 17 )
#define TEST_RRII_LD_D_01( tcname, inst, dest, src1, imm12 )		_TEST_RRII( tcname, inst, dest, src1, imm12, rd, rb,  0, 17 )

#define TEST_RRII_LD_D_10( tcname, inst, type_dest, dest, imm12 )		\
	__TEST_CASE( tcname, dest,											\
		inst	RD_TMP, RB_NEXT, imm12;									\
		jump	1f;														\
		type_dest	dest;												\
		.balign	4;														\
1:		setrd	RD_RET1, RD_TMP;										\
		)

#define _TEST_RRII_ST( tcname, st_inst, ld_inst, dest, src1, src2, imm12, _RGHA, _RGHB, _SRC1, _SRC2 )		\
		__TEST_CASE(	tcname, dest,									\
		set ## _RGHA	_RGHA ## _SRC1, src1;							\
		set ## _RGHB	_RGHB ## _SRC2, src2;							\
		st_inst			_RGHA ## _SRC1, _RGHB ## _SRC2, imm12;			\
		ld_inst			RD_RET1, _RGHB ## _SRC2, imm12;					\
		)

#define TEST_RRII_ST_D_12( tcname, st_inst, ld_inst, dest, src1, src2, imm12 )		_TEST_RRII_ST( tcname, st_inst, ld_inst, dest, src1, src2, imm12, rd, rb, 16, 17 )
#define TEST_RRII_ST_B_12( tcname, st_inst, ld_inst, dest, src1, src2, imm12 )		_TEST_RRII_ST( tcname, st_inst, ld_inst, dest, src1, src2, imm12, rb, rb, 16, 17 )
#define TEST_RRII_ST_F_12( tcname, st_inst, ld_inst, dest, src1, src2, imm12 )		_TEST_RRII_ST( tcname, st_inst, ld_inst, dest, src1, src2, imm12, rf, rb, 16, 17 )

#define _TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else, _SRC1, _SRC2 )	\
	__TEST_CASE_HEAD__(tcname)											\
	setrd	rd ## _SRC1, src1;											\
	setrd	rd ## _SRC2, src2;											\
	inst	rd ## _SRC1, rd ## _SRC2, lbl_ ## tcname ## _taken;			\
	jump	lbl_ ## tcname ## lbl_else;									\
lbl_ ## tcname ## _taken:												\
	jump	lbl_ ## tcname ## lbl_then;									\
lbl_ ## tcname ## _fail:												\
	jump	___fail;													\
lbl_ ## tcname ## _pass:												\
	setrd	RD_FLAG, RD_ZERO;											\

#define TEST_RRII_BR_H_12( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else, 16, 17 )
#define TEST_RRII_BR_H_11( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else, 16, 16 )
#define TEST_RRII_BR_H_10( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else, 16,  0 )
#define TEST_RRII_BR_H_01( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else,  0, 16 )
#define TEST_RRII_BR_H_00( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_HIGH( tcname, inst, src1, src2, lbl_then, lbl_else,  0,  0 )

#define _TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else, _SRC1, _SRC2 )	\
	__TEST_CASE_HEAD__(tcname)											\
	jump	lbl_ ## tcname ## _test;									\
lbl_ ## tcname ## _taken:												\
	jump	lbl_ ## tcname ## lbl_then;									\
lbl_ ## tcname ## _test:												\
	setrd	rd ## _SRC1, src1;											\
	setrd	rd ## _SRC2, src2;											\
	inst	rd ## _SRC1, rd ## _SRC2, lbl_ ## tcname ## _taken;			\
	jump	lbl_ ## tcname ## lbl_else;									\
lbl_ ## tcname ## _fail:												\
	jump	___fail;													\
lbl_ ## tcname ## _pass:												\
	setrd	RD_FLAG, RD_ZERO;											\

#define TEST_RRII_BR_L_12( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else, 16, 17 )
#define TEST_RRII_BR_L_11( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else, 16, 16 )
#define TEST_RRII_BR_L_10( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else, 16,  0 )
#define TEST_RRII_BR_L_01( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else,  0, 16 )
#define TEST_RRII_BR_L_00( tcname, inst, src1, src2, lbl_then, lbl_else )		_TEST_RRII_BR_LOW( tcname, inst, src1, src2, lbl_then, lbl_else,  0,  0 )

#-----------------------------------------------------------------------
# DADAO MACROS for RRRI
#-----------------------------------------------------------------------

#define _TEST_RRRI_M1( tcname, inst, dest, src1, src2, imm6, _RGHA, _RGHB, _RGHC, _DEST, _SRC1, _SRC2 )		\
    __TEST_CASE(	tcname, dest,												\
		set ## _RGHB	_RGHB ## _SRC1, src1;									\
		set ## _RGHC	_RGHC ## _SRC2, src2;									\
		inst			_RGHA ## _DEST, _RGHB ## _SRC1, _RGHC ## _SRC2, imm6;	\
		setrd			RD_RET1, _RGHA ## _DEST;								\
    )

#define TEST_RRRI_LDMRD_123( tcname, inst, dest, src1, src2, imm6 )		_TEST_RRRI_M1( tcname, inst, dest, src1, src2, imm6, rd, rb, rd, 16, 17, 18 )
#define TEST_RRRI_LDMRB_123( tcname, inst, dest, src1, src2, imm6 )		_TEST_RRRI_M1( tcname, inst, dest, src1, src2, imm6, rb, rb, rd, 16, 17, 18 )
#define TEST_RRRI_LDMRA_123( tcname, inst, dest, src1, src2, imm6 )		_TEST_RRRI_M1( tcname, inst, dest, src1, src2, imm6, ra, rb, rd, 16, 17, 18 )
#define TEST_RRRI_LDMRF_123( tcname, inst, dest, src1, src2, imm6 )		_TEST_RRRI_M1( tcname, inst, dest, src1, src2, imm6, rf, rb, rd, 16, 17, 18 )

#define _TEST_RRRI_STM_M1( tcname, inst, ld_inst, dest, src1, src2, imm6, _RGHA, _RGHB, _RGHC, _DEST, _SRC1, _SRC2 )		\
	__TEST_CASE(	tcname, dest,												\
		setrd			RD_TMP, dest;											\
		rd2 ## _RGHA	_RGHA ## _DEST, RD_TMP, 1;								\
		set ## _RGHB	_RGHB ## _SRC1, src1;									\
		set ## _RGHC	_RGHC ## _SRC2, src2;									\
		inst			_RGHA ## _DEST, _RGHB ## _SRC1, _RGHC ## _SRC2, imm6;	\
		ld_inst			RD_RET1, _RGHB ## _SRC1, _RGHC ## _SRC2, imm6;			\
	)

#define TEST_RRRI_STMRD_M1( tcname, inst, ld_inst, dest, src1, src2, imm6 )		_TEST_RRRI_STM_M1( tcname, inst, ld_inst, dest, src1, src2, imm6, rd, rb, rd, 16, 17, 18 )
#define TEST_RRRI_STMRB_M1( tcname, inst, ld_inst, dest, src1, src2, imm6 )		_TEST_RRRI_STM_M1( tcname, inst, ld_inst, dest, src1, src2, imm6, rb, rb, rd, 16, 17, 18 )
#define TEST_RRRI_STMRA_M1( tcname, inst, ld_inst, dest, src1, src2, imm6 )		_TEST_RRRI_STM_M1( tcname, inst, ld_inst, dest, src1, src2, imm6, ra, rb, rd, 16, 17, 18 )
#define TEST_RRRI_STMRF_M1( tcname, inst, ld_inst, dest, src1, src2, imm6 )		_TEST_RRRI_STM_M1( tcname, inst, ld_inst, dest, src1, src2, imm6, rf, rb, rd, 16, 17, 18 )

#define _TEST_RRRI_LDM_M8( tcname, inst, _src1, _src2, _stride, _RGHA )		\
		__TEST_CASE_HEAD__(tcname)									\
	.irp	rn, 16,17,18,19,20,21,22,23;							\
		set ## _RGHA	_RGHA##\rn, \rn;							\
	.endr;															\
		setrb			RB_SRC, _src1;								\
		setrd			RD_TMP, _src2;								\
		inst			_RGHA ## 16, RB_SRC, RD_TMP, 8;				\
	.irp	rn, 16,17,18,19,20,21,22,23;							\
		inst			_RGHA##15, RB_SRC, RD_TMP, 1;				\
		_RGHA ## 2rd	RD_EXP1, _RGHA##15, 1;						\
		_RGHA ## 2rd	RD_RET1, _RGHA##\rn, 1;						\
		brne			RD_RET1, RD_EXP1, ___fail;					\
		addi			RD_TMP, RD_TMP, _stride;					\
	.endr;															\

#define TEST_RRRI_LDMRD_M8( tcname, inst, _src1, _src2, _stride )		_TEST_RRRI_LDM_M8( tcname, inst, _src1, _src2, _stride, rd )
#define TEST_RRRI_LDMRB_M8( tcname, inst, _src1, _src2, _stride )		_TEST_RRRI_LDM_M8( tcname, inst, _src1, _src2, _stride, rb )
#define TEST_RRRI_LDMRF_M8( tcname, inst, _src1, _src2, _stride )		_TEST_RRRI_LDM_M8( tcname, inst, _src1, _src2, _stride, rf )

#define _TEST_RRRI_STM_M8( tcname, inst, ld_inst, _src1, _src2, _stride, _RGHA )		\
		__TEST_CASE_HEAD__(tcname)									\
	.irp	rn, 16,17,18,19,20,21,22,23;							\
		set ## _RGHA	_RGHA##\rn, \rn;							\
	.endr;															\
		setrb			RB_SRC, _src1;								\
		setrd			RD_TMP, _src2;								\
		inst			_RGHA ## 16, RB_SRC, RD_TMP, 8;				\
	.irp	rn, 16,17,18,19,20,21,22,23;							\
		ld_inst			RD_EXP1, RB_SRC, RD_TMP, 1;					\
		setrd			RD_RET1, \rn;								\
		brne			RD_RET1, RD_EXP1, ___fail;					\
		addi			RD_TMP, RD_TMP, _stride;					\
	.endr;															\

#define TEST_RRRI_STMRD_M8( tcname, inst, ld_inst, _src1, _src2, _stride )		_TEST_RRRI_STM_M8( tcname, inst, ld_inst, _src1, _src2, _stride, rd )
#define TEST_RRRI_STMRB_M8( tcname, inst, ld_inst, _src1, _src2, _stride )		_TEST_RRRI_STM_M8( tcname, inst, ld_inst, _src1, _src2, _stride, rb )
#define TEST_RRRI_STMRF_M8( tcname, inst, ld_inst, _src1, _src2, _stride )		_TEST_RRRI_STM_M8( tcname, inst, ld_inst, _src1, _src2, _stride, rf )

#-----------------------------------------------------------------------
# DADAO MACROS for RRRR
#-----------------------------------------------------------------------

#define _TEST_RRRR_RRWR( tcname, inst, dest, cond1, cond2, orig, src, _COND1, _COND2, _DEST, _SRC )		\
	__TEST_CASE(	tcname, dest,											\
		setrd		rd ## _COND1, cond1;									\
		setrd		rd ## _COND2, cond2;									\
		setrd		rd ## _DEST, orig;										\
		setrd		rd ## _SRC, src;										\
		inst		rd ## _COND1, rd ## _COND2, rd ## _DEST, rd ## _SRC;	\
		rd2rd		RD_RET1, rd ## _DEST, 1;								\
	)

#define TEST_RRRR_RRWR_1234( tcname, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( tcname, inst, dest, cond1, cond2, orig, src, 16, 17, 18, 19 )
#define TEST_RRRR_RRWR_0123( tcname, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( tcname, inst, dest, cond1, cond2, orig, src,  0, 17, 18, 19 )
#define TEST_RRRR_RRWR_1023( tcname, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( tcname, inst, dest, cond1, cond2, orig, src, 16,  0, 18, 19 )
#define TEST_RRRR_RRWR_1230( tcname, inst, dest, cond1, cond2, orig, src )		_TEST_RRRR_RRWR( tcname, inst, dest, cond1, cond2, orig, src, 16, 17, 18,  0 )

#define _TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, _RGHBCD, _COND, _DEST, _SRC1, _SRC2 )	\
	__TEST_CASE(	tcname, dest,														\
		setrd		rd ## _COND, cond;													\
		setrd		_RGHBCD ## _SRC1, src1;												\
		setrd		_RGHBCD ## _SRC2, src2;												\
		inst		rd ## _COND, _RGHBCD ## _DEST, _RGHBCD ## _SRC1, _RGHBCD ## _SRC2;	\
		rd2rd		RD_RET1, _RGHBCD ## _DEST, 1;										\
	)

#define TEST_RRRR_RWRR_1234( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17, 18, 19 )
#define TEST_RRRR_RWRR_1123( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 16, 18, 19 )
#define TEST_RRRR_RWRR_1223( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 18, 18, 19 )
#define TEST_RRRR_RWRR_1232( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17, 18, 17 )
#define TEST_RRRR_RWRR_0123( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd,  0, 17, 18, 19 )
#define TEST_RRRR_RWRR_1203( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17,  0, 19 )
#define TEST_RRRR_RWRR_1230( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17, 18,  0 )

#define TEST_RRRR_RWFF_1234( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17, 18, 19 )
#define TEST_RRRR_RWFF_1223( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 18, 18, 19 )
#define TEST_RRRR_RWFF_1232( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd, 16, 17, 18, 17 )
#define TEST_RRRR_RWFF_0123( tcname, inst, dest, cond, src1, src2 )		_TEST_RRRR_RWRR( tcname, inst, dest, cond, src1, src2, rd,  0, 17, 18, 19 )

#define _TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, _DEST1, _DEST2, _SRC1, _SRC2 )			\
	__TEST_CASE_HEAD__(tcname)												\
	setrd	rd ## _SRC1, src1;												\
	setrd	rd ## _SRC2, src2;												\
	inst	rd ## _DEST1, rd ## _DEST2, rd ## _SRC1, rd ## _SRC2;			\
	rd2rd	RD_RET1, rd ## _DEST1, 1;										\
	rd2rd	RD_RET2, rd ## _DEST2, 1;										\
	setrd	RD_EXP1, dest1;													\
	setrd	RD_EXP2, dest2;													\
	brne	RD_RET1, RD_EXP1, ___fail;										\
	brne	RD_RET2, RD_EXP2, ___fail;										\

#define TEST_RRRR_WWRR_1234( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 18, 19 )
#define TEST_RRRR_WWRR_1213( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 16, 19 )
#define TEST_RRRR_WWRR_1223( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 17, 19 )
#define TEST_RRRR_WWRR_1231( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 18, 16 )
#define TEST_RRRR_WWRR_1232( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 18, 17 )
#define TEST_RRRR_WWRR_1211( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 16, 16 )
#define TEST_RRRR_WWRR_1222( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 17, 17 )
#define TEST_RRRR_WWRR_1203( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17,  0, 19 )
#define TEST_RRRR_WWRR_1230( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17, 18,  0 )
#define TEST_RRRR_WWRR_1200( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16, 17,  0,  0 )
#define TEST_RRRR_WWRR_0123( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2,  0, 17, 18, 19 )
#define TEST_RRRR_WWRR_1023( tcname, inst, dest1, dest2, src1, src2 )		_TEST_RRRR_WWRR( tcname, inst, dest1, dest2, src1, src2, 16,  0, 18, 19 )

#define _TEST_RRRR_WRRR( tcname, inst, flags, inst_ld, ftype, val_dst, val_src1, val_src2, val_src3, _DST, _SRC1, _SRC2, _SRC3 )	\
		__TEST_CASE_HEAD__(tcname)											\
		setrb			RB_SRC, test_ ## tcname ## _src1;					\
		inst_ld			rf ## _SRC1, RB_SRC, 0;								\
		setrb			RB_SRC, test_ ## tcname ## _src2;					\
		inst_ld			rf ## _SRC2, RB_SRC, 0;								\
		setrb			RB_SRC, test_ ## tcname ## _src3;					\
		inst_ld			rf ## _SRC3, RB_SRC, 0;								\
		inst			rf ## _DST, rf ## _SRC1, rf ## _SRC2, rf ## _SRC3;	\
		jump			lbl_ ## tcname ## _cmp;								\
		.balign			8;													\
test_ ## tcname ## _dst:													\
		ftype			val_dst;											\
		.balign			8;													\
test_ ## tcname ## _src1:													\
		ftype			val_src1;											\
		.balign			8;													\
test_ ## tcname ## _src2:													\
		ftype			val_src2;											\
		.balign			8;													\
test_ ## tcname ## _src3:													\
		ftype			val_src3;											\
		.balign			4;													\
lbl_ ## tcname ## _cmp:														\
		rf2rd			RD_RET1, rf ## _DST, 1;								\
		setrb			RB_DST, test_ ## tcname ## _dst;					\
		inst_ld			rf ## _DST, RB_DST, 0;								\
		rf2rd			RD_EXP1, rf ## _DST, 1;								\
		brne			RD_RET1, RD_EXP1, ___fail;							\
		__PASS_FAIL_FCSR__(flags)											\

#define TEST_RRRR_WRRR_FT_1234( tcname, inst, flags, val_dst, val_src1, val_src2, val_src3 )	\
			_TEST_RRRR_WRRR(    tcname, inst, flags, ldft, .single, val_dst, val_src1, val_src2, val_src3, 16, 17, 18, 19 )
#define TEST_RRRR_WRRR_FO_1234( tcname, inst, flags, val_dst, val_src1, val_src2, val_src3 )	\
			_TEST_RRRR_WRRR(    tcname, inst, flags, ldfo, .double, val_dst, val_src1, val_src2, val_src3, 16, 17, 18, 19 )

#-----------------------------------------------------------------------
# DADAO MACROS for RWII
#-----------------------------------------------------------------------

#define _TEST_RWII( tcname, inst, wyde, dest, orig, imm16, _RGHA, _DEST )	\
    __TEST_CASE(	tcname, dest,											\
		set ## _RGHA	_RGHA ## _DEST, orig;								\
		inst			_RGHA ## _DEST, wyde, imm16;						\
		_RGHA ## 2rd	RD_RET1, _RGHA ## _DEST, 1;							\
    )

#define TEST_RWII_D( tcname, inst, wyde, dest, orig, imm16 )		_TEST_RWII( tcname, inst, wyde, dest, orig, imm16, rd, 16 )
#define TEST_RWII_B( tcname, inst, wyde, dest, orig, imm16 )		_TEST_RWII( tcname, inst, wyde, dest, orig, imm16, rb, 16 )
#define TEST_RWII_F( tcname, inst, wyde, dest, orig, imm16 )		_TEST_RWII( tcname, inst, wyde, dest, orig, imm16, rf, 16 )

#endif /* __TEST_MACROS_SCALAR_H */
