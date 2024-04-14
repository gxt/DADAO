#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rb2rd,  1,  1, rd, rb )
	TEST_ORRI_R1_12( 3, rb2rd, -1, -1, rd, rb )
	TEST_ORRI_R1_11( 4, rb2rd,  1,  1, rd, rb )
	TEST_ORRI_R1_11( 5, rb2rd, -1, -1, rd, rb )

	TEST_ORRI_R8_C1( 11, rb2rd, rd, rb )
	TEST_ORRI_R8_C2( 12, rb2rd, rd, rb )
	TEST_ORRI_R8_C3( 13, rb2rd, rd, rb )
	TEST_ORRI_R8_C4( 14, rb2rd, rd, rb )
	TEST_ORRI_R8_C5( 15, rb2rd, rd, rb )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
