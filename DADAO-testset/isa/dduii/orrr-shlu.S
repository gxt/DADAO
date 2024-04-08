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

	TEST_ORRR_DDD_123( 2, shlu, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 3, shlu, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 4, shlu, 0x8000000000000000, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 5, shlu, 0xfffffffffffffffe, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 6, shlu, 0xf0fff0f0f0f00000, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 7, shlu, 0x1fe1fe1e00000000, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 8, shlu, 0x1e1e1e1e1e1e1e1e, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 9, shlu, 0x0000000000000000, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 10, shlu, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 11, shlu, 0x787f800000000000, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 12, shlu, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 13, shlu, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
