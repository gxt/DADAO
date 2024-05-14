#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRI_M1_12( 2, rd2rd,  1,  1, rd, rd )
	TEST_ORRI_M1_12( 3, rd2rd, -1, -1, rd, rd )
	TEST_ORRI_M1_11( 4, rd2rd,  1,  1, rd, rd )
	TEST_ORRI_M1_11( 5, rd2rd, -1, -1, rd, rd )
	TEST_ORRI_M1_10( 6, rd2rd,  0,  0, rd, rd )

	TEST_ORRI_M8_C1( 11, rd2rd, rd, rd )
	TEST_ORRI_M8_C2( 12, rd2rd, rd, rd )
#	TEST_ORRI_M8_C3( 13, rd2rd, rd, rd )
	TEST_ORRI_M8_C4( 14, rd2rd, rd, rd )
	TEST_ORRI_M8_C5( 15, rd2rd, rd, rd )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
