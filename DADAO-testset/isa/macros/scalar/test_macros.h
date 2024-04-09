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

#define TEST_CASE( testnum, testreg, correctval, code... )	\
test_ ## testnum:						\
	setrd	TESTNUM, testnum;				\
	code;							\
	setrd	rd15, correctval;				\
	cmps	rd15, testreg, rd15;				\
	brnz	rd15, fail;

#define TEST_CASE_MULTI_REG( testnum, inst, reg1, reg2, imm6, reggroup, correctval, cmpinst, registernum... )	\
test_ ## testnum:												\
        inst	reg1, reg2, imm6;										\
        setrd	TESTNUM, testnum;										\
        set##reggroup	reggroup##62, correctval;								\
        .irp    param,registernum;										\
        cmpinst	rd63, reggroup\param, reggroup##62;								\
        brnz    rd63, fail;											\
        addi    reggroup##62, reggroup##62, 1;									\
        .endr;

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
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------

#define TEST_ORRI_INIT_REG( REG_GROUP )				\
    set##REG_GROUP REG_GROUP##0, 0x0;					\
    set##REG_GROUP REG_GROUP##1, 0x1;                                     \
    set##REG_GROUP REG_GROUP##2, 0x2;                                     \
    set##REG_GROUP REG_GROUP##3, 0x3;                                     \
    set##REG_GROUP REG_GROUP##4, 0x4;                                     \
    set##REG_GROUP REG_GROUP##5, 0x5;                                     \
    set##REG_GROUP REG_GROUP##6, 0x6;                                     \
    set##REG_GROUP REG_GROUP##7, 0x7;                                     \
    set##REG_GROUP REG_GROUP##8, 0x8;                                     \
    set##REG_GROUP REG_GROUP##9, 0x9;                                     \
    set##REG_GROUP REG_GROUP##10, 0xa;                                    \
    set##REG_GROUP REG_GROUP##11, 0xb;                                    \
    set##REG_GROUP REG_GROUP##12, 0xc;                                    \
    set##REG_GROUP REG_GROUP##13, 0xd;                                    \
    set##REG_GROUP REG_GROUP##14, 0xe;                                    \
    set##REG_GROUP REG_GROUP##15, 0xf;                                    \
    set##REG_GROUP REG_GROUP##16, 0x10;                                   \
    set##REG_GROUP REG_GROUP##17, 0x11;                                   \
    set##REG_GROUP REG_GROUP##18, 0x12;                                   \
    set##REG_GROUP REG_GROUP##19, 0x13;                                   \
    set##REG_GROUP REG_GROUP##20, 0x14;                                   \
    set##REG_GROUP REG_GROUP##21, 0x15;                                   \
    set##REG_GROUP REG_GROUP##22, 0x16;                                   \
    set##REG_GROUP REG_GROUP##23, 0x17;                                   \
    set##REG_GROUP REG_GROUP##24, 0x18;                                   \
    set##REG_GROUP REG_GROUP##25, 0x19;                                   \
    set##REG_GROUP REG_GROUP##26, 0x1a;                                   \
    set##REG_GROUP REG_GROUP##27, 0x1b;                                   \
    set##REG_GROUP REG_GROUP##28, 0x1c;                                   \
    set##REG_GROUP REG_GROUP##29, 0x1d;                                   \
    set##REG_GROUP REG_GROUP##30, 0x1e;                                   \
    set##REG_GROUP REG_GROUP##31, 0x1f;                                   \
    set##REG_GROUP REG_GROUP##32, 0x20;                                   \
    set##REG_GROUP REG_GROUP##33, 0x21;                                   \
    set##REG_GROUP REG_GROUP##34, 0x22;                                   \
    set##REG_GROUP REG_GROUP##35, 0x23;                                   \
    set##REG_GROUP REG_GROUP##36, 0x24;                                   \
    set##REG_GROUP REG_GROUP##37, 0x25;                                   \
    set##REG_GROUP REG_GROUP##38, 0x26;                                   \
    set##REG_GROUP REG_GROUP##39, 0x27;                                   \
    set##REG_GROUP REG_GROUP##40, 0x28;                                   \
    set##REG_GROUP REG_GROUP##41, 0x29;                                   \
    set##REG_GROUP REG_GROUP##42, 0x2a;                                   \
    set##REG_GROUP REG_GROUP##43, 0x2b;                                   \
    set##REG_GROUP REG_GROUP##44, 0x2c;                                   \
    set##REG_GROUP REG_GROUP##45, 0x2d;                                   \
    set##REG_GROUP REG_GROUP##46, 0x2e;                                   \
    set##REG_GROUP REG_GROUP##47, 0x2f;                                   \
    set##REG_GROUP REG_GROUP##48, 0x30;                                   \
    set##REG_GROUP REG_GROUP##49, 0x31;                                   \
    set##REG_GROUP REG_GROUP##50, 0x32;                                   \
    set##REG_GROUP REG_GROUP##51, 0x33;                                   \
    set##REG_GROUP REG_GROUP##52, 0x34;                                   \
    set##REG_GROUP REG_GROUP##53, 0x35;                                   \
    set##REG_GROUP REG_GROUP##54, 0x36;                                   \
    set##REG_GROUP REG_GROUP##55, 0x37;                                   \
    set##REG_GROUP REG_GROUP##56, 0x38;                                   \
    set##REG_GROUP REG_GROUP##57, 0x39;                                   \
    set##REG_GROUP REG_GROUP##58, 0x3a;                                   \
    set##REG_GROUP REG_GROUP##59, 0x3b;                                   \
    set##REG_GROUP REG_GROUP##60, 0x3c;                                   \
    set##REG_GROUP REG_GROUP##61, 0x3d;                                   \
    set##REG_GROUP REG_GROUP##62, 0x3e;                                   \
    set##REG_GROUP REG_GROUP##63, 0x3f;                                    

#-----------------------------------------------------------------------
# Pass and fail code (assumes test num is in TESTNUM)
#-----------------------------------------------------------------------

#define TEST_PASSFAIL						\
	brnz  TESTNUM, pass;					\
fail:								\
	DDTEST_FAIL;						\
pass:								\
	DDTEST_PASS;						\

#-----------------------------------------------------------------------
# Test data section
#-----------------------------------------------------------------------

#define TEST_DATA

#endif /* __TEST_MACROS_SCALAR_H */
