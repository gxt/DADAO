#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_WWRR_1234( 2, divu,  2,  3,  20,  6)

	TEST_RRRR_WWRR_1234( 3, divu, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff)
	TEST_RRRR_WWRR_1234( 4, divu, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff)

	TEST_RRRR_WWRR_1213( 5, divu, 0, 3, 24, 8 )
	TEST_RRRR_WWRR_1223( 6, divu, 0, 3, 27, 9 )
	TEST_RRRR_WWRR_1231( 7, divu, 0, 3, 30, 10 )
	TEST_RRRR_WWRR_1232( 8, divu, 0, 3, 33, 11 )

	TEST_RRRR_WWRR_1211(  9, divu, 0, 1, 25, 25 )
	TEST_RRRR_WWRR_1222( 10, divu, 0, 1, 36, 36 )

	TEST_RRRR_WWRR_1203( 14, divu, 0, 0, 0, 33 )
	TEST_RRRR_WWRR_0123( 15, divu, 0, 3, 20, 6 )
	TEST_RRRR_WWRR_1023( 16, divu, 2, 0, 26, 6 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

