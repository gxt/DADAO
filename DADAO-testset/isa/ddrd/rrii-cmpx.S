#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_DD_12( 102, cmpu, 0x0000000000000000, 0x00000000, 0x000 )
	TEST_RRII_DD_12( 103, cmpu, 0x0000000000000001, 0x00000005, 0x000 )
	TEST_RRII_DD_12( 104, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 )

	TEST_RRII_DD_12( 105, cmpu, 0x0000000000000000, 0x00000fff, 0xfff )
	TEST_RRII_DD_12( 106, cmpu, 0x0000000000000001, 0xffffffffffffffff, 0xfff )
	TEST_RRII_DD_12( 107, cmpu, 0xffffffffffffffff, 0x00000000000000ff, 0xfff )

	TEST_RRII_DD_11( 108, cmpu, 1, 5, 4 )

	TEST_RRII_DD_10( 115, cmpu, -1, 0, 1 )
	TEST_RRII_DD_01( 116, cmpu, 0, 4, 5 )

	TEST_RRII_DD_12( 202, cmps, 0x0000000000000000, 0x00000000, 0x000 )
	TEST_RRII_DD_12( 203, cmps, 0x0000000000000001, 0x00000005, 0x000 )
	TEST_RRII_DD_12( 204, cmps, 0xffffffffffffffff, 0x000,0x000000005 )
	TEST_RRII_DD_12( 205, cmps, 0xffffffffffffffff, 0xffffffffffffffff, 0x000 )
	TEST_RRII_DD_12( 206, cmps, 0x0000000000000001, 0x000, 0xffffffffffffffff )

	TEST_RRII_DD_12( 207, cmps, 0x0000000000000000, 0x0000000000000005, 0x005 )
	TEST_RRII_DD_12( 208, cmps, 0x0000000000000000, 0xffffffffffffffff,    -1 )

	TEST_RRII_DD_12( 209, cmps, 0x0000000000000001, 0x00ffffffffffffff,  0x7ff )
	TEST_RRII_DD_12( 210, cmps, 0xffffffffffffffff, 0x000000000000000f,  0x7ff )

	TEST_RRII_DD_12( 211, cmps, 0x0000000000000001, 0xffffffffffffffff, -0x800 )
	TEST_RRII_DD_12( 212, cmps, 0xffffffffffffffff, 0xffffff0000000000,  0x00f )
	TEST_RRII_DD_12( 213, cmps, 0xffffffffffffffff, 0xfffffffffffffffe,     -1 )

	TEST_RRII_DD_11( 214, cmps, 1, 5, 4 )

	TEST_RRII_DD_10( 221, cmps, 1, 0, -1 )
	TEST_RRII_DD_01( 222, cmps, 0, 4, 5 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

