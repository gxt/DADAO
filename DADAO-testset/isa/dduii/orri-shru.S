#*****************************************************************************
# shru.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_12( 14, shru, 0x0000000000000007, 0x000000000000000f, 1 )
	TEST_ORRI_12( 15, shru, 0xff0f0f0f0f0f0fff, 0xff0f0f0f0f0f0fff, 0 )
	TEST_ORRI_12( 16, shru, 0x0000000f000f000f, 0xf000f000f000f000, 28 )
	TEST_ORRI_12( 17, shru, 0x0000000000000001, 0xf0f0f0fff0f0f0f0, 63 )
	TEST_ORRI_12( 18, shru, 0x000000000000000f, 0xf0f0f0f0fff0000f, 60 )
	TEST_ORRI_12( 19, shru, 0x00000000f0f0f0f0, 0xf0f0f0f0f0ffff00, 32 )
	TEST_ORRI_12( 20, shru, 0x0000000000078787, 0xf0f0f0f0f0ffff00, 45 )

	TEST_ORRI_11( 21, shru, 0x00000f00f0f0f0f0, 0xf00f0f0f0f0f0f0f, 20 )
	TEST_ORRI_11( 22, shru, 0x0000000000000003, 0xf0f0f0f0fffff0ff, 62 )

	TEST_ORRI_10( 23, shru, 0x0000000000000000, 0, 3 )

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
