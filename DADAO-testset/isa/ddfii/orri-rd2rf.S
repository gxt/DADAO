#*****************************************************************************
# rd2rf.S
#-----------------------------------------------------------------------------

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rd2rf,  1,  1, rf, rd )
	TEST_ORRI_R1_12( 3, rd2rf, -1, -1, rf, rd )
	TEST_ORRI_R1_11( 4, rd2rf,  1,  1, rf, rd )
	TEST_ORRI_R1_11( 5, rd2rf, -1, -1, rf, rd )
	TEST_ORRI_R1_10( 6, rd2rf,  0,  0, rf, rd )

	TEST_ORRI_R8_C1( 11, rd2rf, rf, rd )
	TEST_ORRI_R8_C2( 12, rd2rf, rf, rd )
	TEST_ORRI_R8_C3( 13, rd2rf, rf, rd )
	TEST_ORRI_R8_C4( 14, rd2rf, rf, rd )
	TEST_ORRI_R8_C5( 15, rd2rf, rf, rd )

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
