#*****************************************************************************
# cmps.S
#-----------------------------------------------------------------------------
#
# Test cmps instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 23, cmps, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 24, cmps, 0x0000000000000000, 0x0fffffffffffffff, 0x0fffffffffffffff )
	TEST_ORRR_DDD_123( 25, cmps, 0xffffffffffffffff, 0x0000000000000003, 0x0000000000000005 )
	TEST_ORRR_DDD_123( 26, cmps, 0x0000000000000001, 0x0000000000000005, 0x0000000000000002 )
	TEST_ORRR_DDD_123( 27, cmps, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 28, cmps, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 29, cmps, 0x0000000000000001, 0xffffffffffffffff, 0xfffffffffffffffe )
	TEST_ORRR_DDD_123( 30, cmps, 0x0000000000000001, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_ORRR_DDD_123( 31, cmps, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f )

	TEST_ORRR_DDD_112( 32, cmps, 1, 6, 4 )
	TEST_ORRR_DDD_121( 33, cmps, 1, 7, 5 )
	TEST_ORRR_DDD_111( 34, cmps, 0, 8, 8 )

	TEST_ORRR_DDD_102( 35, cmps, 1, 0, -1)
	TEST_ORRR_DDD_102( 36, cmps, 0, 0, 0)
	TEST_ORRR_DDD_120( 37, cmps, 1, 1, 0)
	TEST_ORRR_DDD_120( 38, cmps, 0, 0, 0)
	TEST_ORRR_DDD_100( 39, cmps, 0, 0, 0)
	TEST_ORRR_DDD_012( 40, cmps, 0, 5, 6 )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
