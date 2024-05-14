#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRI_M1_12( 102, rd2rf,  1,  1, rf, rd )
	TEST_ORRI_M1_12( 103, rd2rf, -1, -1, rf, rd )
	TEST_ORRI_M1_11( 104, rd2rf,  1,  1, rf, rd )
	TEST_ORRI_M1_11( 105, rd2rf, -1, -1, rf, rd )
	TEST_ORRI_M1_10( 106, rd2rf,  0,  0, rf, rd )

	TEST_ORRI_M8_C1( 111, rd2rf, rf, rd )
	TEST_ORRI_M8_C2( 112, rd2rf, rf, rd )
	TEST_ORRI_M8_C3( 113, rd2rf, rf, rd )
	TEST_ORRI_M8_C4( 114, rd2rf, rf, rd )
	TEST_ORRI_M8_C5( 115, rd2rf, rf, rd )

	TEST_ORRI_M1_12( 202, rf2rd,  1,  1, rd, rf )
	TEST_ORRI_M1_12( 203, rf2rd, -1, -1, rd, rf )
	TEST_ORRI_M1_11( 204, rf2rd,  1,  1, rd, rf )
	TEST_ORRI_M1_11( 205, rf2rd, -1, -1, rd, rf )

	TEST_ORRI_M8_C1( 211, rf2rd, rd, rf )
	TEST_ORRI_M8_C2( 212, rf2rd, rd, rf )
	TEST_ORRI_M8_C3( 213, rf2rd, rd, rf )
	TEST_ORRI_M8_C4( 214, rf2rd, rd, rf )
	TEST_ORRI_M8_C5( 215, rf2rd, rd, rf )

	TEST_ORRI_M1_12( 302, rf2rf,  1,  1, rf, rf )
	TEST_ORRI_M1_12( 303, rf2rf, -1, -1, rf, rf )
	TEST_ORRI_M1_11( 304, rf2rf,  1,  1, rf, rf )
	TEST_ORRI_M1_11( 305, rf2rf, -1, -1, rf, rf )

	TEST_ORRI_M8_C1( 311, rf2rf, rf, rf )
	TEST_ORRI_M8_C2( 312, rf2rf, rf, rf )
#	TEST_ORRI_M8_C3( 313, rf2rf, rf, rf )
	TEST_ORRI_M8_C4( 314, rf2rf, rf, rf )
	TEST_ORRI_M8_C5( 315, rf2rf, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
