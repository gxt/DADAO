#*****************************************************************************
# sub.S
#-----------------------------------------------------------------------------
#
# Test sub instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_ORRR_BBD_123( 30, sub, 0x0, 0x0, 0x0 )
	TEST_ORRR_BBD_123( 31, sub, 0x1, 0x3, 0x2 )

	TEST_ORRR_BBD_123( 32, sub, 0xfffffffe00000001, 0xffffffff00000000, 0x00000000ffffffff )
	TEST_ORRR_BBD_123( 33, sub, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000 )
	TEST_ORRR_BBD_123( 34, sub, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )

	TEST_ORRR_BBD_112( 35, sub, 13, 24, 11 )
	TEST_ORRR_BBD_112( 36, sub, 0xfffffffe00000001, 0xffffffff00000000, 0x00000000ffffffff )
	TEST_ORRR_BBD_120( 37, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0,                  0 )

DDTEST_CODE_END

.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

 

