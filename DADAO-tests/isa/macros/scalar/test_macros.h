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

#define TEST_ORRR_OP( testnum, inst, result, val1, val2 )	\
    TEST_CASE( testnum, rd14, result,				\
	move  rd1, val1;					\
	move  rd2, val2;					\
	inst  rd14, rd1, rd2;					\
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
