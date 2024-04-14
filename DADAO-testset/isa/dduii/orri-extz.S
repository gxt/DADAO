#*****************************************************************************
# extz.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_12( 15, extz, 0xfff0f0f0f0f0000f, 0xfff0f0f0f0f0000f, 0 )
	TEST_ORRI_12( 16, extz, 0x0000000000000001, 0xfff0f0f0f0f0000f, 63 )
	TEST_ORRI_12( 17, extz, 0x000000000f0000ff, 0xff0f0f0f0f0000ff, 32 )
	TEST_ORRI_12( 18, extz, 0x0000000000000000, 0x0000000000000000, 63 )
	TEST_ORRI_12( 19, extz, 0x000000000001ff00, 0xfff0f0f0f0ffff00, 47 )
	TEST_ORRI_12( 20, extz, 0x000000000000000f, 0xfff0000f0f0f0f0f, 60 )

	TEST_ORRI_11( 21, extz, 0x1ff0f000f0f0ffff, 0xfff0f000f0f0ffff, 3 )
	TEST_ORRI_11( 22, extz, 0x00000000000000f0, 0x0000f0f0f00000f0, 48 )

	TEST_ORRI_10( 23, extz, 0x0000000000000000, 0, 31 )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
