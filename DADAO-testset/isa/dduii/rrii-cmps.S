#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_DD_12( 2, cmps, 0x0000000000000000, 0x00000000, 0x000 )
	TEST_RRII_DD_12( 3, cmps, 0x0000000000000001, 0x00000005, 0x000 )
	TEST_RRII_DD_12( 4, cmps, 0xffffffffffffffff, 0x000,0x000000005 )
	TEST_RRII_DD_12( 5, cmps, 0xffffffffffffffff, 0xffffffffffffffff, 0x000 )
	TEST_RRII_DD_12( 6, cmps, 0x0000000000000001, 0x000, 0xffffffffffffffff )

	TEST_RRII_DD_12( 7, cmps, 0x0000000000000000, 0x0000000000000005, 0x005 )
	TEST_RRII_DD_12( 8, cmps, 0x0000000000000000, 0xffffffffffffffff,    -1 )

	TEST_RRII_DD_12( 9, cmps, 0x0000000000000001, 0x00ffffffffffffff,  0x7ff )
	TEST_RRII_DD_12(10, cmps, 0xffffffffffffffff, 0x000000000000000f,  0x7ff )

	TEST_RRII_DD_12(11, cmps, 0x0000000000000001, 0xffffffffffffffff, -0x800 )
	TEST_RRII_DD_12(12, cmps, 0xffffffffffffffff, 0xffffff0000000000,  0x00f )
	TEST_RRII_DD_12(13, cmps, 0xffffffffffffffff, 0xfffffffffffffffe,     -1 )

	TEST_RRII_DD_11( 14, cmps, 1, 5, 4 )

	TEST_RRII_DD_10( 21, cmps, 1, 0, -1 )
	TEST_RRII_DD_01( 22, cmps, 0, 4, 5 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
