#*****************************************************************************
# addi.S
#-----------------------------------------------------------------------------
#
# Test addi instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_RRII_BB_12( 17, addi, 0x00000000, 0x00000000, 0x000 )
	TEST_RRII_BB_12( 18, addi, 0x00000002, 0x00000001, 0x001 )
	TEST_RRII_BB_12( 19, addi, 0x0000000a, 0x00000003, 0x007 )

	TEST_RRII_BB_12( 20, addi, 0xffffffffffffffff, 0x0000000000000000, -1 )
	TEST_RRII_BB_12( 21, addi, 0xffffffff80000000, 0xffffffff80000000, 0x000 )
	TEST_RRII_BB_12( 22, addi, 0xffffffff7ffff800, 0xffffffff80000000, -0x800 )

	TEST_RRII_BB_12( 23, addi, 0x00000000000007ff, 0x00000000, 0x7ff )
	TEST_RRII_BB_12( 24, addi, 0x000000007fffffff, 0x7fffffff, 0x000 )
	TEST_RRII_BB_12( 25, addi, 0x00000000800007fe, 0x7fffffff, 0x7ff )

	TEST_RRII_BB_12( 26, addi, 0xffffffff800007ff, 0xffffffff80000000, 0x7ff )
	TEST_RRII_BB_12( 27, addi, 0x000000007ffff7ff, 0x000000007fffffff, -0x800 )

	TEST_RRII_BB_12( 28, addi, 0xffffffffffffffff, 0x0000000000000000, -1 )
	TEST_RRII_BB_12( 29, addi, 0x0000000000000000, 0xffffffffffffffff, 0x001 )
	TEST_RRII_BB_12( 30, addi, 0xfffffffffffffffe, 0xffffffffffffffff, -1 )

	TEST_RRII_BB_12( 31, addi, 0x0000000080000000, 0x7fffffff, 0x001 )

	TEST_RRII_BB_11( 33, addi, 10, 5, 5)

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

