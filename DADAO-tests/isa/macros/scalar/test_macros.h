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

#-----------------------------------------------------------------------
# DADAO MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------

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
		swym;							\
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
		swym;							\
	.endr;								\
	move	rd18, val2;						\
	.rept	src2_swyms						\
		swym;							\
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
		swym;							\
	.endr;								\
	move	rd17, val2;						\
	.rept	src2_swyms						\
		swym;							\
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
		swym;							\
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
		swym;							\
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
		swym;							\
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
		swym;							\
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
		swym;										\
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
