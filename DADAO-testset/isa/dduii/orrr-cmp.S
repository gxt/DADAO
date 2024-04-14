#*****************************************************************************
# cmp.S
#-----------------------------------------------------------------------------
#
# Test cmp instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRR_DBB_123( 2, cmp, 0x00000000, 0x00000000, 0x000 )
	TEST_ORRR_DBB_123( 3, cmp, 0x00000001, 0x00000005, 0x000 )
	TEST_ORRR_DBB_123( 4, cmp, -1, 0x00000000, 8)

	TEST_ORRR_DBB_123( 5, cmp, 0x0000000000000000, 0x000000000000, 0x000000000000 )
	TEST_ORRR_DBB_123( 6, cmp, 0x0000000000000001, 0xffffffffffff, 0x000000000000 )
	TEST_ORRR_DBB_123( 7, cmp, 0xffffffffffffffff, 0x000000000000, 0xffffffffffff )
	TEST_ORRR_DBB_123( 8, cmp, 0x0000000000000001, 0xffffffffffff, 0xfffffffffffc )

	TEST_ORRR_DBB_122(  9, cmp, 0x0, 0x12345678, 0x12345678 )
	TEST_ORRR_DBB_012( 10, cmp, 0x0, 0x12345678, 0x43215678 )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

