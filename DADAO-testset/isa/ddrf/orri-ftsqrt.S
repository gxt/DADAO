#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 11, ftsqrt, fflag_NX, ldft, .single, 1.7724538498928541, ldft, .single,  3.14159265, rf, rf )
	TEST_FRRI_R1_12( 12, ftsqrt, 0, ldft, .single, 100, ldft, .single, 10000, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
