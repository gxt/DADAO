#*****************************************************************************
# rf2rd.S
#-----------------------------------------------------------------------------

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rf2rd,  1,  1, rd, rf )
	TEST_ORRI_R1_12( 3, rf2rd, -1, -1, rd, rf )
	TEST_ORRI_R1_11( 4, rf2rd,  1,  1, rd, rf )
	TEST_ORRI_R1_11( 5, rf2rd, -1, -1, rd, rf )

	TEST_ORRI_R8_C1( 11, rf2rd, rd, rf )
	TEST_ORRI_R8_C2( 12, rf2rd, rd, rf )
	TEST_ORRI_R8_C3( 13, rf2rd, rd, rf )
	TEST_ORRI_R8_C4( 14, rf2rd, rd, rf )
	TEST_ORRI_R8_C5( 15, rf2rd, rd, rf )

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
