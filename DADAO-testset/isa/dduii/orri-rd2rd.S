#*****************************************************************************
# rd2rd.S
#-----------------------------------------------------------------------------

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_R1_12( 2, rd2rd,  1,  1, rd, rd )
	TEST_ORRI_R1_12( 3, rd2rd, -1, -1, rd, rd )
	TEST_ORRI_R1_11( 4, rd2rd,  1,  1, rd, rd )
	TEST_ORRI_R1_11( 5, rd2rd, -1, -1, rd, rd )
	TEST_ORRI_R1_10( 6, rd2rd,  0,  0, rd, rd )

	TEST_ORRI_R8_C1( 11, rd2rd, rd, rd )
	TEST_ORRI_R8_C2( 12, rd2rd, rd, rd )
#	TEST_ORRI_R8_C3( 13, rd2rd, rd, rd )
	TEST_ORRI_R8_C4( 14, rd2rd, rd, rd )
	TEST_ORRI_R8_C5( 15, rd2rd, rd, rd )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
