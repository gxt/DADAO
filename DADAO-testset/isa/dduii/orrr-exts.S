#*****************************************************************************
# exts.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 2, exts, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 3, exts, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 4, exts, 0xfffff0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 5, exts, 0x000000000ff00000, 0xf00f0f0f0ff00000, 0x0000000000000020 )
	TEST_ORRR_DDD_123( 6, exts, 0x00000f0f00f000f0, 0xf0000f0f00f000f0, 0x0000000000000012 )
	TEST_ORRR_DDD_123( 7, exts, 0xffffffffffffffff, 0x0000000000000001, 0x000000000000003f )
	TEST_ORRR_DDD_123( 8, exts, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )

	TEST_ORRR_DDD_112( 9, exts, 0xfff0f0f0f0f0f0ff, 0xfff0f0f0f0f0f0ff, 0x0000000000000003 )
	TEST_ORRR_DDD_112( 10, exts, 0x0000f0f0f000ffff, 0x00f0f0f0f000ffff, 0x000000000000000f )

	TEST_ORRR_DDD_121( 11, exts, 0x0000000000000000, 0xf0f0f0f0000f0000, 0x000000000000003f )
	TEST_ORRR_DDD_121( 12, exts, 0xffffffffffffffff, 0x00f0f0f0f000ffff, 0x0000000000000030 )

	TEST_ORRR_DDD_102( 13, exts, 0x0000000000000000, 0, 0x0000000000000025 )
	TEST_ORRR_DDD_120( 14, exts, 0x0f0f0f0f0000f0f0, 0x0f0f0f0f0000f0f0, 0 )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
