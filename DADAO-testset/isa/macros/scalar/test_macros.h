#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define __TEST_CASE( testnum, expect1, code... )		\
test_ ## testnum:										\
	setrd	RD_NUMR, testnum;							\
	code;												\
	setrd	RD_EXP1, expect1;							\
	cmpu	RD_FLAG, RD_RET1, RD_EXP1;					\
	brnz	RD_FLAG, ___fail;							\
	addi	RD_PASS, RD_PASS, 1;

#define TEST_CASE( testnum, testreg, correctval, code... )	\
test_ ## testnum:						\
	setrd	TESTNUM, testnum;				\
	code;							\
	setrd	rd15, correctval;				\
	cmps	rd15, testreg, rd15;				\
	brnz	rd15, fail;

#define TEST_CASE_DOUBLE_TESTREG( testnum, testreg_1, testreg_2, correctval_1, correctval_2, code... )	\
test_ ## testnum:											\
	setrd	TESTNUM, testnum;									\
	code;												\
	setrd	rd14, correctval_1;									\
	setrd	rd15, correctval_2;									\
	cmps	rd14, testreg_1, rd14;									\
	brnz	rd14, fail;										\
	cmps	rd15, testreg_2, rd15;									\
	brnz    rd15, fail;

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

#define TEST_ORRI_OP( testnum, inst, result, val1, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, val1;						\
	inst	rd31, rd16, imm6;					\
    )

#define TEST_ORRI_SRC_OP( testnum, inst, result, val1, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
        setrd    rd31, val1;						\
	inst	rd31, rd31, imm6;					\
    )

#define TEST_ORRI_ZERO_SRC( testnum, inst, result, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
        inst    rd31, rd0, imm6;					\
    )

#define TEST_RRRR_RET3_OP( testnum, inst, result, val1, val2, val3)     \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val1;                                             \
        setrd	rd17, val2;                                             \
        setrd	rd18, val3;                                             \
        inst    rd16, rd17, rd31, rd18;                                 \
    )

#define TEST_RRRR_RET3_ZEROSRC1( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd0, rd16, rd31, rd17                                   \
    )

#define TEST_RRRR_RET3_ZEROSRC2( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd16, rd0, rd31, rd17                                   \
    )

#define TEST_RRRR_RET3_ZEROSRC3( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd16, rd17, rd31, rd0                                   \
    )

#define TEST_RRRR_RET1_OP( testnum, inst, result, val1, val2, val3)     \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val1;                                             \
        setrd	rd17, val2;                                             \
        setrd	rd18, val3;                                             \
        inst    rd16, rd31, rd17, rd18;                                 \
    )

#define TEST_RRRR_RET1_SRC1_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd31, val1;                                             \
        setrd	rd16, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd31, rd31, rd16, rd17;                                 \
    )

#define TEST_RRRR_RET1_SRC2_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val1;                                             \
        setrd	rd31, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd16, rd31, rd31, rd17;                                 \
    )

#define TEST_RRRR_RET1_SRC3_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        setrd	rd16, val1;                                             \
        setrd	rd17, val2;                                             \
        setrd	rd31, val3;                                             \
        inst    rd16, rd31, rd17, rd31;                                 \
    )
#define TEST_RRRR_RET1_ZEROSRC1( testnum, inst, result, val2, val3 )	\
    TEST_CASE( testnum, rd31, result,					\
        setrd	rd16, val2;                                             \
        setrd	rd17, val3;                                             \
        inst    rd0, rd31, rd16, rd17                                   \
    )
#define TEST_RRRR_RET1_ZEROSRC2( testnum, inst, result, val1, val3 )	\
    TEST_CASE( testnum, rd31, result,					\
        setrd	rd16, val1;                                             \
        setrd	rd17, val3;                                             \
        inst    rd16, rd31, rd0, rd17                                   \
    )
#define TEST_RRRR_RET1_ZEROSRC3( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
        setrd	rd16, val1;                                             \
        setrd	rd17, val2;                                             \
        inst    rd16, rd31, rd17, rd0                                   \
    )

#define TEST_ORRR_OP( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, val1;						\
	setrd	rd17, val2;						\
	inst	rd31, rd16, rd17;					\
    )

#define TEST_ORRR_SRC1_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd31, val1;						\
	setrd	rd16, val2;						\
	inst	rd31, rd31, rd16;					\
    )

#define TEST_ORRR_SRC2_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, val1;						\
	setrd	rd31, val2;						\
	inst	rd31, rd16, rd31;					\
    )

#define TEST_ORRR_SRC12_EQ_DEST( testnum, inst, result, val1 )		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd31, val1;						\
	inst	rd31, rd31, rd31;					\
    )

#define TEST_ORRR_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, 0;						\
1:	setrd	rd17, val1;						\
	setrd	rd18, val2;						\
	inst	rd19, rd17, rd18;					\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	rd2rd	rd31, rd19, 1;						\
	addi	rd16, rd16, 1;						\
	cmps	rd20, rd16, 2;						\
	brnz	rd20, 1b						\
    )

#define TEST_ORRR_SRC12_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, 0;						\
1:	setrd	rd17, val1;						\
	.rept	src1_swyms						\
		swym	0;						\
	.endr;								\
	setrd	rd18, val2;						\
	.rept	src2_swyms						\
		swym	0;						\
	.endr;								\
	inst	rd31, rd17, rd18;					\
	addi	rd16, rd16, 1;						\
	cmps	rd19, rd16, 2;						\
	brnz	rd19, 1b						\
    )

#define TEST_ORRR_SRC21_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, 0;						\
1:	setrd	rd18, val1;						\
	.rept	src1_swyms						\
		swym	0;						\
	.endr;								\
	setrd	rd17, val2;						\
	.rept	src2_swyms						\
		swym	0;						\
	.endr;								\
	inst	rd31, rd17, rd18;					\
	addi	rd16, rd16, 1;						\
	cmps	rd19, rd16, 2;						\
	brnz	rd19, 1b						\
    )

#define TEST_ORRR_ZEROSRC1( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, val;						\
	inst	rd31, rd0, rd16;					\
    )

#define TEST_ORRR_ZEROSRC2( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd16, val;						\
	inst	rd31, rd16, rd0;					\
    )

#define TEST_ORRR_ZEROSRC12( testnum, inst, result )			\
    TEST_CASE( testnum, rd31, result,					\
	inst	rd31, rd0, rd0;						\
    )

#define TEST_ORRR_ZERODEST( testnum, inst, val1, val2 )			\
    TEST_CASE( testnum, rd0, 0,						\
	setrd	rd16, val1;						\
	setrd	rd17, val2;						\
	inst	rd0, rd16, rd17;					\
    )

#-----------------------------------------------------------------------
# Tests for an instruction with register-ww-immu16 operands
#-----------------------------------------------------------------------

#define TEST_RWII_RD( testnum, inst, result, val1, val2, wyde )		\
    TEST_CASE( testnum, rd31, result, 					\
	setrd	rd31, val1;						\
	inst	rd31, wyde, val2;					\
    )

#define TEST_RWII_RB( testnum, inst, result, val1, val2, wyde )		\
    TEST_CASE( testnum, rd31, result, 					\
	setrb	rb31, val1;						\
	inst	rb31, wyde, val2;					\
	rb2rd	rd31, rb31, 1;						\
    )

#define TEST_RWII_RF( testnum, inst, result, val1, val2 , wyde)		\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd31, val1;						\
	rd2rf	rf31, rd31, 1;						\
	inst	rf31, wyde, val2;					\
	rf2rd	rd31, rf31, 1;						\
    )

#-----------------------------------------------------------------------
# Tests for instructions with register-immediate operand
#-----------------------------------------------------------------------
#define TEST_RIII_OP( testnum, inst, result, val1, imm )		\
    TEST_CASE( testnum, rd31, result,					\
	setrb	rb31, val1;						\
	inst	rb31, imm;						\
	rb2rd	rd31, rb31, 1;						\
    )

#define TEST_RIII_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd18, 0;						\
1:	setrd	rd16, val1;						\
	inst	rd16, imm;						\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	rd2rd	rd31, rd16, 1;						\
	addi	rd18, rd18, 1;						\
	cmps	rd19, rd18, 2;						\
	brnz	rd19, 1b;						\
    )

#define TEST_RIII_SRC1_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	setrd	rd18, 0;						\
1:	setrd	rd31, val1;						\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	inst	rd31, imm;						\
	addi	rd18, rd18, 1;						\
	cmps	rd19, rd18, 2;						\
	brnz	rd18, 1b;						\
    )

#define TEST_RIII_ZEROSRC1( testnum, inst, result, imm )		\
    TEST_CASE( testnum, rd0, result,					\
	inst	rd0, imm;						\
    )

#-----------------------------------------------------------------------
# Tests for an instruction with register-register-register-register operands
#-----------------------------------------------------------------------

#define TEST_RRRR_OP( testnum, inst, result_1, result_2, val1, val2 )		\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,		\
	setrd	rd16, val1;							\
	setrd	rd17, val2;							\
	inst	rd30, rd31, rd16, rd17;						\
    )

#define TEST_RRRR_SRC1_EQ_DEST1( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd16, rd31, result_1, result_2,			\
	setrd	rd16, val1;								\
	setrd	rd17, val2;								\
	inst	rd16, rd31, rd16, rd17;							\
    ) 

#define TEST_RRRR_SRC1_EQ_DEST2( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd17, result_1, result_2,			\
	setrd	rd16, val1;								\
	setrd	rd17, val2;								\
	inst	rd31, rd17, rd16, rd17;							\
    )

#define TEST_RRRR_SRC2_EQ_DEST1( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd17, rd31, result_1, result_2,			\
	setrd	rd16, val1;								\
	setrd	rd17, val2;								\
	inst	rd17, rd31, rd16, rd17;							\
    )

#define TEST_RRRR_SRC2_EQ_DEST2( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd17, result_1, result_2,			\
	setrd	rd16, val1;								\
	setrd	rd17, val2;								\
	inst	rd31, rd17, rd16, rd17;							\
    )
	
#define TEST_RRRR_SRC12_EQ_DEST1( testnum, inst, result_1, result_2, val1  )		\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd16, rd31, result_1, result_2,			\
	setrd	rd16, val1;								\
	inst	rd16, rd31, rd16, rd16;							\
    )

#define TEST_RRRR_SRC12_EQ_DEST2( testnum, inst, result_1, result_2, val1 )		\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd16, result_1, result_2,			\
	setrd	rd16, val1;								\
	inst	rd31, rd16, rd16, rd16;							\
    )

#define TEST_RRRR_DEST_BYPASS( testnum, swym_cycles, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,				\
	setrd	rd18, 0;									\
1:	setrd	rd16, val1;									\
	setrd	rd17, val2;									\
	inst	rd20, rd21, rd16, rd17;								\
	.rept	swym_cycles									\
		swym	0;									\
	.endr;											\
        rd2rd   rd30, rd20, 1;									\
	rd2rd	rd31, rd21, 1;									\
	addi	rd18, rd18, 1;									\
	cmps	rd19, rd18, 2;									\
	brnz	rd19, 1b;									\
    )

#define TEST_RRRR_ZEROSR1( testnum, inst, result_1, result_2, val )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,	\
	setrd	rd16, val;						\
	inst	rd30, rd31, rd0, rd16;					\
    )

#define TEST_RRRR_ZEROSR2( testnum, inst, result_1, result_2, val )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,	\
	setrd	rd16, val;						\
	inst	rd30, rd31, rd16, rd0;					\
    )

#define TEST_RRRR_ZEROSR12( testnum, inst, result_1, result_2	 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30 ,rd31, result_1, result_2,	\
	inst	rd30, rd31, rd0, rd0;					\
    )

#define TEST_RRRR_ZERODEST1( testnum, inst, result, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd0, rd31, 0, result,		\
	setrd	rd16, val1;						\
	setrd	rd17, val2;						\
	inst	rd0, rd31, rd16, rd17;					\
    )

#define TEST_RRRR_ZERODEST2( testnum, inst, result, val1, val2 )        \
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd0, result, 0,		\
	setrd	rd16, val1;						\
	setrd	rd17, val2;						\
	inst	rd31, rd0, rd16, rd17;					\
    )

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
