#*****************************************************************************
# add.S
#-----------------------------------------------------------------------------
#
# Test add instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRR_BBD_123( 31, add, 0x0, 0x0, 0x0 )
	TEST_ORRR_BBD_123( 32, add, 0x3, 0x1, 0x2 )

	TEST_ORRR_BBD_123( 33, add, 0xffffffffffffffff, 0xffffffff00000000, 0x00000000ffffffff )
	TEST_ORRR_BBD_123( 34, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000 )
	TEST_ORRR_BBD_123( 35, add, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f )

	TEST_ORRR_BBD_112( 36, add, 24, 13, 11 )
	TEST_ORRR_BBD_112( 37, add, 0xffffffffffffffff, 0xffffffff00000000, 0x00000000ffffffff )

	TEST_ORRR_BBD_120( 38, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0,                  0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

