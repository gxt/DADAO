#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rf2rf,  1,  1, rf, rf )
	TEST_ORRI_R1_12( 3, rf2rf, -1, -1, rf, rf )
	TEST_ORRI_R1_11( 4, rf2rf,  1,  1, rf, rf )
	TEST_ORRI_R1_11( 5, rf2rf, -1, -1, rf, rf )

	TEST_ORRI_R8_C1( 11, rf2rf, rf, rf )
	TEST_ORRI_R8_C2( 12, rf2rf, rf, rf )
#	TEST_ORRI_R8_C3( 13, rf2rf, rf, rf )
	TEST_ORRI_R8_C4( 14, rf2rf, rf, rf )
	TEST_ORRI_R8_C5( 15, rf2rf, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
