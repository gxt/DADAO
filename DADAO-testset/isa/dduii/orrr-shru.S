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

	TEST_ORRR_DDD_123( 2, shru, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 3, shru, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 4, shru, 0x0000000000000001, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 5, shru, 0x7fffffffffffffff, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 6, shru, 0x0000f0f0f0fff0f0, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 7, shru, 0x000000007f87f87f, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 8, shru, 0x0787878787878787, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 9, shru, 0x00000000ffffffff, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 10, shru, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 11, shru, 0x000000000001e001, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 12, shru, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 13, shru, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
