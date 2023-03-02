#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define TEST_CASE( testnum, testreg, correctval, code... )	\
test_ ## testnum:						\
	move	TESTNUM, testnum;				\
	code;							\
	move	rd15, correctval;				\
	cmps	rd15, testreg, rd15;				\
	brnz	rd15, fail;

#define TEST_CASE_DOUBLE_TESTREG( testnum, testreg_1, testreg_2, correctval_1, correctval_2, code... )	\
test_ ## testnum:											\
	move	TESTNUM, testnum;									\
	code;												\
	move	rd14, correctval_1;									\
	move	rd15, correctval_2;									\
	cmps	rd14, testreg_1, rd14;									\
	brnz	rd14, fail;										\
	cmps	rd15, testreg_2, rd15;									\
	brnz    rd15, fail;

#define TEST_CASE_MULTI_REG( testnum, inst, reg1, reg2, imm6, reggroup, correctval, cmpinst, registernum... )	\
test_ ## testnum:												\
        inst    reg1, reg2, imm6;										\
        move    TESTNUM, testnum;										\
        move    reggroup##62, correctval;									\
        .irp    param,registernum;										\
        cmpinst	rd63, reggroup\param, reggroup##62;								\
        brnz    rd63, fail;											\
        addi    reggroup##62, reggroup##62, 1;									\
        .endr;

#-----------------------------------------------------------------------
# DADAO MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------

#define TEST_ORRI_INIT_REG( REG_GROUP )				\
    move REG_GROUP##0, 0x0;					\
    move REG_GROUP##1, 0x1;                                     \
    move REG_GROUP##2, 0x2;                                     \
    move REG_GROUP##3, 0x3;                                     \
    move REG_GROUP##4, 0x4;                                     \
    move REG_GROUP##5, 0x5;                                     \
    move REG_GROUP##6, 0x6;                                     \
    move REG_GROUP##7, 0x7;                                     \
    move REG_GROUP##8, 0x8;                                     \
    move REG_GROUP##9, 0x9;                                     \
    move REG_GROUP##10, 0xa;                                    \
    move REG_GROUP##11, 0xb;                                    \
    move REG_GROUP##12, 0xc;                                    \
    move REG_GROUP##13, 0xd;                                    \
    move REG_GROUP##14, 0xe;                                    \
    move REG_GROUP##15, 0xf;                                    \
    move REG_GROUP##16, 0x10;                                   \
    move REG_GROUP##17, 0x11;                                   \
    move REG_GROUP##18, 0x12;                                   \
    move REG_GROUP##19, 0x13;                                   \
    move REG_GROUP##20, 0x14;                                   \
    move REG_GROUP##21, 0x15;                                   \
    move REG_GROUP##22, 0x16;                                   \
    move REG_GROUP##23, 0x17;                                   \
    move REG_GROUP##24, 0x18;                                   \
    move REG_GROUP##25, 0x19;                                   \
    move REG_GROUP##26, 0x1a;                                   \
    move REG_GROUP##27, 0x1b;                                   \
    move REG_GROUP##28, 0x1c;                                   \
    move REG_GROUP##29, 0x1d;                                   \
    move REG_GROUP##30, 0x1e;                                   \
    move REG_GROUP##31, 0x1f;                                   \
    move REG_GROUP##32, 0x20;                                   \
    move REG_GROUP##33, 0x21;                                   \
    move REG_GROUP##34, 0x22;                                   \
    move REG_GROUP##35, 0x23;                                   \
    move REG_GROUP##36, 0x24;                                   \
    move REG_GROUP##37, 0x25;                                   \
    move REG_GROUP##38, 0x26;                                   \
    move REG_GROUP##39, 0x27;                                   \
    move REG_GROUP##40, 0x28;                                   \
    move REG_GROUP##41, 0x29;                                   \
    move REG_GROUP##42, 0x2a;                                   \
    move REG_GROUP##43, 0x2b;                                   \
    move REG_GROUP##44, 0x2c;                                   \
    move REG_GROUP##45, 0x2d;                                   \
    move REG_GROUP##46, 0x2e;                                   \
    move REG_GROUP##47, 0x2f;                                   \
    move REG_GROUP##48, 0x30;                                   \
    move REG_GROUP##49, 0x31;                                   \
    move REG_GROUP##50, 0x32;                                   \
    move REG_GROUP##51, 0x33;                                   \
    move REG_GROUP##52, 0x34;                                   \
    move REG_GROUP##53, 0x35;                                   \
    move REG_GROUP##54, 0x36;                                   \
    move REG_GROUP##55, 0x37;                                   \
    move REG_GROUP##56, 0x38;                                   \
    move REG_GROUP##57, 0x39;                                   \
    move REG_GROUP##58, 0x3a;                                   \
    move REG_GROUP##59, 0x3b;                                   \
    move REG_GROUP##60, 0x3c;                                   \
    move REG_GROUP##61, 0x3d;                                   \
    move REG_GROUP##62, 0x3e;                                   \
    move REG_GROUP##63, 0x3f;                                    

#define TEST_ORRI_OP( testnum, inst, result, val1, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val1;						\
	inst	rd31, rd16, imm6;					\
    )

#define TEST_ORRI_SRC_OP( testnum, inst, result, val1, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
        move    rd31, val1;						\
	inst	rd31, rd31, imm6;					\
    )

#define TEST_ORRI_ZERO_SRC( testnum, inst, result, imm6 )		\
    TEST_CASE( testnum, rd31, result,					\
        inst    rd31, rd0, imm6;					\
    )

#define TEST_RRRR_RET3_OP( testnum, inst, result, val1, val2, val3)     \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val1;                                             \
        move    rd17, val2;                                             \
        move    rd18, val3;                                             \
        inst    rd16, rd17, rd31, rd18;                                 \
    )

#define TEST_RRRR_RET3_ZEROSRC1( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd0, rd16, rd31, rd17                                   \
    )

#define TEST_RRRR_RET3_ZEROSRC2( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd16, rd0, rd31, rd17                                   \
    )

#define TEST_RRRR_RET3_ZEROSRC3( testnum, inst, result, val2, val3 )    \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd16, rd17, rd31, rd0                                   \
    )

#define TEST_RRRR_RET1_OP( testnum, inst, result, val1, val2, val3)     \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val1;                                             \
        move    rd17, val2;                                             \
        move    rd18, val3;                                             \
        inst    rd16, rd31, rd17, rd18;                                 \
    )

#define TEST_RRRR_RET1_SRC1_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd31, val1;                                             \
        move    rd16, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd31, rd31, rd16, rd17;                                 \
    )

#define TEST_RRRR_RET1_SRC2_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val1;                                             \
        move    rd31, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd16, rd31, rd31, rd17;                                 \
    )

#define TEST_RRRR_RET1_SRC3_EQ_DEST( testnum, inst, result, val1, val2, val3)   \
    TEST_CASE( testnum, rd31, result,                                   \
        move    rd16, val1;                                             \
        move    rd17, val2;                                             \
        move    rd31, val3;                                             \
        inst    rd16, rd31, rd17, rd31;                                 \
    )
#define TEST_RRRR_RET1_ZEROSRC1( testnum, inst, result, val2, val3 )	\
    TEST_CASE( testnum, rd31, result,					\
        move    rd16, val2;                                             \
        move    rd17, val3;                                             \
        inst    rd0, rd31, rd16, rd17                                   \
    )
#define TEST_RRRR_RET1_ZEROSRC2( testnum, inst, result, val1, val3 )	\
    TEST_CASE( testnum, rd31, result,					\
        move    rd16, val1;                                             \
        move    rd17, val3;                                             \
        inst    rd16, rd31, rd0, rd17                                   \
    )
#define TEST_RRRR_RET1_ZEROSRC3( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
        move    rd16, val1;                                             \
        move    rd17, val2;                                             \
        inst    rd16, rd31, rd17, rd0                                   \
    )

#define TEST_ORRR_OP( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val1;						\
	move	rd17, val2;						\
	inst	rd31, rd16, rd17;					\
    )

#define TEST_ORRR_OP_RB( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb16, val1;						\
	move	rb17, val2;						\
	inst	rd31, rb16, rb17;					\
    )

#define TEST_ORRR_OP_BBD( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb16, val1;						\
	move	rd31, val2;						\
	inst	rb31, rb16, rd31;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_ORRR_SRC1_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	move	rd16, val2;						\
	inst	rd31, rd31, rd16;					\
    )

#define TEST_ORRR_SRC1_EQ_DEST_RB( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rb31, val1;						\
	move	rb17, val2;						\
	inst	rb31, rb31, rb17;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_ORRR_SRC1_EQ_DEST_BBD( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rb31, val1;						\
	move	rd17, val2;						\
	inst	rb31, rb31, rd17;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_ORRR_SRC2_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val1;						\
	move	rd31, val2;						\
	inst	rd31, rd16, rd31;					\
    )

#define TEST_ORRR_SRC2_EQ_DEST_RB( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rb16, val1;						\
	move	rb31, val2;						\
	inst	rd31, rb16, rb31;					\
    )

#define TEST_ORRR_SRC12_EQ_DEST( testnum, inst, result, val1 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	inst	rd31, rd31, rd31;					\
    )

#define TEST_ORRR_SRC1_EQ_SRC2_RB( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb31, val;						\
	inst	rd31, rb31, rb31;					\
    )

#define TEST_ORRR_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, 0;						\
1:	move	rd17, val1;						\
	move	rd18, val2;						\
	inst	rd19, rd17, rd18;					\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	rd2rd	rd31, rd19, 0;						\
	addi	rd16, rd16, 1;						\
	cmps	rd20, rd16, 2;						\
	brnz	rd20, 1b						\
    )

#define TEST_ORRR_SRC12_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, 0;						\
1:	move	rd17, val1;						\
	.rept	src1_swyms						\
		swym	0;						\
	.endr;								\
	move	rd18, val2;						\
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
	move	rd16, 0;						\
1:	move	rd18, val1;						\
	.rept	src1_swyms						\
		swym	0;						\
	.endr;								\
	move	rd17, val2;						\
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
	move	rd16, val;						\
	inst	rd31, rd0, rd16;					\
    )

#define TEST_ORRR_ZEROSRC2( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val;						\
	inst	rd31, rd16, rd0;					\
    )

#define TEST_ORRR_ZEROSRC2_BBD( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb16, val;						\
	inst	rb31, rb16, rd0;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_ORRR_ZEROSRC12( testnum, inst, result )			\
    TEST_CASE( testnum, rd31, result,					\
	inst	rd31, rd0, rd0;						\
    )

#define TEST_ORRR_ZERODEST( testnum, inst, val1, val2 )			\
    TEST_CASE( testnum, rd0, 0,						\
	move	rd16, val1;						\
	move	rd17, val2;						\
	inst	rd0, rd16, rd17;					\
    )

#define TEST_ORRR_ZERODEST_RB( testnum, inst, val1, val2 )		\
    TEST_CASE( testnum, rd0, 0,						\
	move	rb16, val1;						\
	move	rb17, val2;						\
	inst	rd0, rb16, rb17;					\
    )

#-----------------------------------------------------------------------
# Tests for an instruction with register-ww-immu16 operands
#-----------------------------------------------------------------------

#define TEST_RWII_RD( testnum, inst, result, val1, val2, wyde )		\
    TEST_CASE( testnum, rd31, result, 					\
	move	rd31, val1;						\
	inst	rd31, wyde, val2;					\
    )

#define TEST_RWII_RB( testnum, inst, result, val1, val2, wyde )		\
    TEST_CASE( testnum, rd31, result, 					\
	move	rb31, val1;						\
	inst	rb31, wyde, val2;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_RWII_RF( testnum, inst, result, val1, val2 , wyde)		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	rd2rf	rf31, rd31, 0;						\
	inst	rf31, wyde, val2;					\
	rf2rd	rd31, rf31, 0;						\
    )

#-----------------------------------------------------------------------
# Tests for instructions with register-register-immediate operand
#-----------------------------------------------------------------------

#define TEST_RRII_OP( testnum, inst, result, val1, imm )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val1;						\
	inst	rd31, rd16, imm;					\
    )

#define TEST_RRII_OP_RB( testnum, inst, result, val1, imm )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb16, val1;						\
	inst	rb31, rb16, imm;					\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_RRII_SRC1_EQ_DEST( testnum, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	inst	rd31, rd31, imm;					\
    )

#define TEST_RRII_SRC1_EQ_DEST_RB( testnum, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rb31, val1;						\
	inst	rb31, rb31, imm;					\
	rb2rd	rd31, rb31, imm;					\
    )

#define TEST_RRII_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd18, 0;						\
1:	move	rd16, val1;						\
	inst	rd19, rd16, imm;					\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	rd2rd	rd31, rd19, 0;						\
	addi	rd18, rd18, 1;						\
	cmps	rd20, rd18, 2;						\
	brnz	rd20, 1b;						\
    )

#define TEST_RRII_SRC1_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd18, 0;						\
1:	move	rd16, val1;						\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	inst	rd31, rd16, imm;					\
	addi	rd18, rd18, 1;						\
	cmps	rd19, rd18, 2;						\
	brnz	rd19, 1b;						\
    )

#define TEST_RRII_ZEROSRC1( testnum, inst, result, imm )		\
    TEST_CASE( testnum, rd31, result,					\
	inst	rd31, rd0, imm;						\
    )

#define TEST_RRII_ZERODEST( testnum, inst, val1, imm )			\
    TEST_CASE( testnum, rd0, 0,						\
	move	rd31, val1;						\
	inst	rd0, rd31, imm;						\
    )

#-----------------------------------------------------------------------
# Tests for instructions with register-immediate operand
#-----------------------------------------------------------------------
#define TEST_RIII_OP( testnum, inst, result, val1, imm )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rb31, val1;						\
	inst	rb31, imm;						\
	rb2rd	rd31, rb31, 0;						\
    )

#define TEST_RIII_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd18, 0;						\
1:	move	rd16, val1;						\
	inst	rd16, imm;						\
	.rept	swym_cycles						\
		swym	0;						\
	.endr;								\
	rd2rd	rd31, rd16, 0;						\
	addi	rd18, rd18, 1;						\
	cmps	rd19, rd18, 2;						\
	brnz	rd19, 1b;						\
    )

#define TEST_RIII_SRC1_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd18, 0;						\
1:	move	rd31, val1;						\
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
	move	rd16, val1;							\
	move	rd17, val2;							\
	inst	rd30, rd31, rd16, rd17;						\
    )

#define TEST_RRRR_SRC1_EQ_DEST1( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd16, rd31, result_1, result_2,			\
	move	rd16, val1;								\
	move	rd17, val2;								\
	inst	rd16, rd31, rd16, rd17;							\
    ) 

#define TEST_RRRR_SRC1_EQ_DEST2( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd17, result_1, result_2,			\
	move	rd16, val1;								\
	move	rd17, val2;								\
	inst	rd31, rd17, rd16, rd17;							\
    )

#define TEST_RRRR_SRC2_EQ_DEST1( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd17, rd31, result_1, result_2,			\
	move	rd16, val1;								\
	move	rd17, val2;								\
	inst	rd17, rd31, rd16, rd17;							\
    )

#define TEST_RRRR_SRC2_EQ_DEST2( testnum, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd17, result_1, result_2,			\
	move	rd1, val1;								\
	move	rd2, val2;								\
	inst	rd31, rd17, rd1, rd2;							\
    )
	
#define TEST_RRRR_SRC12_EQ_DEST1( testnum, inst, result_1, result_2, val1  )		\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd16, rd31, result_1, result_2,			\
	move	rd16, val1;								\
	inst	rd16, rd31, rd16, rd16;							\
    )

#define TEST_RRRR_SRC12_EQ_DEST2( testnum, inst, result_1, result_2, val1 )		\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd16, result_1, result_2,			\
	move	rd16, val1;								\
	inst	rd31, rd16, rd16, rd16;							\
    )

#define TEST_RRRR_DEST_BYPASS( testnum, swym_cycles, inst, result_1, result_2, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,				\
	move	rd18, 0;									\
1:	move	rd16, val1;									\
	move	rd17, val2;									\
	inst	rd20, rd21, rd16, rd17;								\
	.rept	swym_cycles									\
		swym	0;									\
	.endr;											\
        rd2rd   rd30, rd20, 0;									\
	rd2rd	rd31, rd21, 0;									\
	addi	rd18, rd18, 1;									\
	cmps	rd19, rd18, 2;									\
	brnz	rd19, 1b;									\
    )

#define TEST_RRRR_ZEROSR1( testnum, inst, result_1, result_2, val )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,	\
	move	rd16, val;						\
	inst	rd30, rd31, rd0, rd16;					\
    )

#define TEST_RRRR_ZEROSR2( testnum, inst, result_1, result_2, val )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30, rd31, result_1, result_2,	\
	move	rd16, val;						\
	inst	rd30, rd31, rd16, rd0;					\
    )

#define TEST_RRRR_ZEROSR12( testnum, inst, result_1, result_2	 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd30 ,rd31, result_1, result_2,	\
	inst	rd30, rd31, rd0, rd0;					\
    )

#define TEST_RRRR_ZERODEST1( testnum, inst, result, val1, val2 )	\
    TEST_CASE_DOUBLE_TESTREG( testnum, rd0, rd31, 0, result,		\
	move	rd16, val1;						\
	move	rd17, val2;						\
	inst	rd0, rd31, rd16, rd17;					\
    )

#define TEST_RRRR_ZERODEST2( testnum, inst, result, val1, val2 )        \
    TEST_CASE_DOUBLE_TESTREG( testnum, rd31, rd0, result, 0,		\
	move	rd16, val1;						\
	move	rd17, val2;						\
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
