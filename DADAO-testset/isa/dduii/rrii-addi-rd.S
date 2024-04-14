#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_DD_12( 2,  addi, 0x00000000, 0x00000000, 0x000 )
	TEST_RRII_DD_12( 3,  addi, 0x00000002, 0x00000001, 0x001 )
	TEST_RRII_DD_12( 4,  addi, 0x0000000a, 0x00000003, 0x007 )

	TEST_RRII_DD_12( 5,  addi, 0xffffffffffffffff, 0x0000000000000000, -1 )
	TEST_RRII_DD_12( 6,  addi, 0xffffffff80000000, 0xffffffff80000000, 0x000 )
	TEST_RRII_DD_12( 7,  addi, 0xffffffff7ffff800, 0xffffffff80000000, -0x800 )

	TEST_RRII_DD_12( 8,  addi, 0x00000000000007ff, 0x00000000, 0x7ff )
	TEST_RRII_DD_12( 9,  addi, 0x000000007fffffff, 0x7fffffff, 0x000 )
	TEST_RRII_DD_12( 10, addi, 0x00000000800007fe, 0x7fffffff, 0x7ff )

	TEST_RRII_DD_12( 11, addi, 0xffffffff800007ff, 0xffffffff80000000, 0x7ff )
	TEST_RRII_DD_12( 12, addi, 0x000000007ffff7ff, 0x000000007fffffff, -0x800 )

	TEST_RRII_DD_12( 13, addi, 0xffffffffffffffff, 0x0000000000000000, -1 )
	TEST_RRII_DD_12( 14, addi, 0x0000000000000000, 0xffffffffffffffff, 0x001 )
	TEST_RRII_DD_12( 15, addi, 0xfffffffffffffffe, 0xffffffffffffffff, -1 )

	TEST_RRII_DD_12( 16, addi, 0x0000000080000000, 0x7fffffff, 0x001 )

	TEST_RRII_DD_11( 32, addi, 9, 5, 4)

	TEST_RRII_DD_10( 40, addi, 32, 0, 32 )
	TEST_RRII_DD_01( 41, addi, 0, 33, 33 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

