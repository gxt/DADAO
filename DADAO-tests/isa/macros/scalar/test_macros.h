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

#define TEST_ORRR_SRC1_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	move	rd16, val2;						\
	inst	rd31, rd31, rd16;					\
    )

#define TEST_ORRR_SRC2_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd31, result,					\
	move	rd16, val1;						\
	move	rd31, val2;						\
	inst	rd31, rd16, rd31;					\
    )

#define TEST_ORRR_SRC12_EQ_DEST( testnum, inst, result, val1 )		\
    TEST_CASE( testnum, rd31, result,					\
	move	rd31, val1;						\
	inst	rd31, rd31, rd31;					\
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
	addi	rd16, 1;						\
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
	addi	rd16, 1;						\
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
	addi	rd16, 1;						\
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
    TEST_CASE( testnum, rd14, result,					\
	move	rd1, val1;						\
	inst	rd14, rd1, imm;						\
    )

#define TEST_RRII_SRC1_EQ_DEST( testnum, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd1, result,					\
	move	rd1, val1;						\
	inst	rd1, rd1, imm;						\
    )

#define TEST_RRII_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd6, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	inst	rd14, rd1, imm;						\
	.rept	swym_cycles						\
		swym;							\
	.endr;								\
	rd2rd	rd6, rd14, 0;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b;						\
    )

#define TEST_RRII_SRC1_BYPASS( testnum, swym_cycles, inst, result, val1, imm )	\
    TEST_CASE( testnum, rd14, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	.rept	swym_cycles						\
		swym;							\
	.endr;								\
	inst	rd14, rd1, imm;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b;						\
    )

#define TEST_RRII_ZEROSRC1( testnum, inst, result, imm )		\
    TEST_CASE( testnum, rd1, result,					\
	inst	rd1, rd0, imm;						\
    )

#define TEST_RRII_ZERODEST( testnum, inst, val1, imm )			\
    TEST_CASE( testnum, rd0, 0,						\
	move	rd1, val1;						\
	inst	rd0, rd1, imm;						\
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
