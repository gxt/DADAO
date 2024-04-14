#*****************************************************************************
# shlu.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRI_12( 14, shlu, 0x000000000000001e, 0x000000000000000f, 1 )
	TEST_ORRI_12( 15, shlu, 0xff0f0f0f0f0f0fff, 0xff0f0f0f0f0f0fff, 0 )
	TEST_ORRI_12( 16, shlu, 0x0f000f0000000000, 0xf000f000f000f000, 28 )
	TEST_ORRI_12( 17, shlu, 0x0000000000000000, 0xf0f0f0fff0f0f0f0, 63 )
	TEST_ORRI_12( 18, shlu, 0xf000000000000000, 0xf0f0f0f0fff0000f, 60 )
	TEST_ORRI_12( 19, shlu, 0xf0ffff0000000000, 0xf0f0f0f0f0ffff00, 32 )
	TEST_ORRI_12( 20, shlu, 0xffe0000000000000, 0xf0f0f0f0f0ffff00, 45 )

	TEST_ORRI_11( 21, shlu, 0xf0f0f0f0f0f00000, 0xf00f0f0f0f0f0f0f, 20 )
	TEST_ORRI_11( 22, shlu, 0xc000000000000000, 0xf0f0f0f0fffff0ff, 62 )

	TEST_ORRI_10( 23, shlu, 0x0000000000000000, 0, 3 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
