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

	TEST_ORRR_DDD_123( 17, cmpu, 0x0, 0x0, 0x0 )
	TEST_ORRR_DDD_123( 18, cmpu, 0x1, 0x5, 0x0 )
	TEST_ORRR_DDD_123( 19, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 )

	TEST_ORRR_DDD_123( 20, cmpu, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 21, cmpu, 0x0000000000000001, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f )
	TEST_ORRR_DDD_123( 22, cmpu, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )

	TEST_ORRR_DDD_112( 23, cmpu,  1, 5, 4 )
	TEST_ORRR_DDD_121( 24, cmpu, -1, 4, 5 )
	TEST_ORRR_DDD_111( 25, cmpu,  0, 6, 6 )

	TEST_ORRR_DDD_102( 26, cmpu, -1, 0, 1 )
	TEST_ORRR_DDD_120( 27, cmpu,  1, 5, 0 )
	TEST_ORRR_DDD_100( 28, cmpu,  0, 0, 0 ) 
	TEST_ORRR_DDD_012( 29, cmpu,  0, 5, 6 )
 
DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

