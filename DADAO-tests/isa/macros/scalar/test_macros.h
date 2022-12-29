#ifndef __TEST_MACROS_SCALAR_H
#define __TEST_MACROS_SCALAR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

#define TEST_CASE( testnum, testreg, correctval, code... )	\
test_ ## testnum:						\
	move	TESTNUM, testnum;				\
	code;							\
	move	rd7, correctval;				\
	cmps	rd7, testreg, rd7;				\
	brnz	rd7, fail;

#-----------------------------------------------------------------------
# DADAO MACROS
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Tests for an instruction with register-register operands
#-----------------------------------------------------------------------

#define TEST_ORRR_OP( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd14, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd14, rd1, rd2;						\
    )

#define TEST_ORRR_SRC1_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd1, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd1, rd1, rd2;						\
    )

#define TEST_ORRR_SRC2_EQ_DEST( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd2, rd1, rd2;						\
    )

#define TEST_ORRR_SRC12_EQ_DEST( testnum, inst, result, val1 )		\
    TEST_CASE( testnum, rd1, result,					\
	move	rd1, val1;						\
	inst	rd1, rd1, rd1;						\
    )

#define TEST_ORRR_DEST_BYPASS( testnum, swym_cycles, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd6, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd14, rd1, rd2;						\
	.rept	swym_cycles						\
		swym;							\
	.endr;								\
	rd2rd	rd6, rd14, 0;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_SRC12_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd14, result,					\
	move	rd4, 0;							\
1:	move	rd1, val1;						\
	.rept	src1_swyms						\
		swym;							\
	.endr;								\
	move	rd2, val2;						\
	.rept	src2_swyms						\
		swym;							\
	.endr;								\
	inst	rd14, rd1, rd2;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_SRC21_BYPASS( testnum, src1_swyms, src2_swyms, inst, result, val1, val2 ) \
    TEST_CASE( testnum, rd14, result,					\
	move	rd4, 0;							\
1:	move	rd2, val1;						\
	.rept	src1_swyms						\
		swym;							\
	.endr;								\
	move	rd1, val2;						\
	.rept	src2_swyms						\
		swym;							\
	.endr;								\
	inst	rd14, rd1, rd2;						\
	addi	rd4, 1;							\
	cmps	rd5, rd4, 2;						\
	brnz	rd5, 1b							\
    )

#define TEST_ORRR_ZEROSRC1( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val;						\
	inst	rd2, rd0, rd1;						\
    )

#define TEST_ORRR_ZEROSRC2( testnum, inst, result, val )		\
    TEST_CASE( testnum, rd2, result,					\
	move	rd1, val;						\
	inst	rd2, rd1, rd0;						\
    )

#define TEST_ORRR_ZEROSRC12( testnum, inst, result )			\
    TEST_CASE( testnum, rd1, result,					\
	inst	rd1, rd0, rd0;						\
    )

#define TEST_ORRR_ZERODEST( testnum, inst, val1, val2 )			\
    TEST_CASE( testnum, rd0, 0,						\
	move	rd1, val1;						\
	move	rd2, val2;						\
	inst	rd0, rd1, rd2;						\
    )

#-----------------------------------------------------------------------
# Tests for an instruction with register-ww-immu16 operands
#-----------------------------------------------------------------------

#define TEST_RWII_RD_W0( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        inst	rd5, w0, val2;						\
    )

#define TEST_RWII_RD_W1( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        inst	rd5, w1, val2;						\
    )

#define TEST_RWII_RD_W2( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        inst	rd5, w2, val2;						\
    )

#define TEST_RWII_RD_W3( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        inst	rd5, w3, val2;						\
    )

#define TEST_RWII_RB_W0( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rb5, val1;						\
        inst	rb5, w0, val2;						\
        rb2rd	rd5, rb5, 0;						\
    )

#define TEST_RWII_RB_W1( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rb5, val1;						\
        inst	rb5, w1, val2;						\
        rb2rd	rd5, rb5, 0;						\
    )

#define TEST_RWII_RB_W2( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
	move	rb5, val1;						\
        inst	rb2, w2, val2;						\
        rb2rd	rd2, rb2, 0;						\
    )

#define TEST_RWII_RB_W3( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rb5, val1;						\
        inst	rb5, w3, val2;						\
        rb2rd	rd5, rb5, 0;						\
    )

#define TEST_RWII_RF_W0( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        rd2rf	rf5, rd5, 0;						\
        inst	rf5, w0, val2;						\
        rf2rd	rd5, rf5, 0;						\
    )

#define TEST_RWII_RF_W1( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        rd2rf	rf5, rd5, 0;						\
        inst	rf5, w1, val2;						\
        rf2rd	rd5, rf5, 0;						\
    )

#define TEST_RWII_RF_W2( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        rd2rf	rf5, rd5, 0;						\
        inst	rf5, w2, val2;						\
        rf2rd	rd5, rf5, 0;						\
    )

#define TEST_RWII_RF_W3( testnum, inst, result, val1, val2 )		\
    TEST_CASE( testnum, rd5, result,					\
        move	rd5, val1;						\
        rd2rf	rf5, rd5, 0;						\
        inst	rf5, w3, val2;						\
        rf2rd	rd5, rf5, 0;						\
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
