#*****************************************************************************
# cmpu.S
#-----------------------------------------------------------------------------
#
# Test cmpu instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_RRII_DD_12( 2, cmpu, 0x0000000000000000, 0x00000000, 0x000 )
	TEST_RRII_DD_12( 3, cmpu, 0x0000000000000001, 0x00000005, 0x000 )
	TEST_RRII_DD_12( 4, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 )

	TEST_RRII_DD_12( 5, cmpu, 0x0000000000000000, 0x00000fff, 0xfff )
	TEST_RRII_DD_12( 6, cmpu, 0x0000000000000001, 0xffffffffffffffff, 0xfff )
	TEST_RRII_DD_12( 7, cmpu, 0xffffffffffffffff, 0x00000000000000ff, 0xfff )

	TEST_RRII_DD_11( 8, cmpu, 1, 5, 4 )

	TEST_RRII_DD_10( 15, cmpu, -1, 0, 1 )
	TEST_RRII_DD_01( 16, cmpu, 0, 4, 5 )

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

