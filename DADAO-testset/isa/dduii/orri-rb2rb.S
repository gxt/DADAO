#*****************************************************************************
# rb2rb.S
#-----------------------------------------------------------------------------

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rb2rb,  1,  1, rb, rb )
	TEST_ORRI_R1_12( 3, rb2rb, -1, -1, rb, rb )
	TEST_ORRI_R1_11( 4, rb2rb,  1,  1, rb, rb )
	TEST_ORRI_R1_11( 5, rb2rb, -1, -1, rb, rb )

	TEST_ORRI_R8_C1( 11, rb2rb, rb, rb )
	TEST_ORRI_R8_C2( 12, rb2rb, rb, rb )
#	TEST_ORRI_R8_C3( 13, rb2rb, rb, rb )
	TEST_ORRI_R8_C4( 14, rb2rb, rb, rb )
	TEST_ORRI_R8_C5( 15, rb2rb, rb, rb )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
