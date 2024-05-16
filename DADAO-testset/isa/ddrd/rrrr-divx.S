#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_WWRR_1234( 102, divu,  2,  3,  20,  6)

	TEST_RRRR_WWRR_1234( 103, divu, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff)
	TEST_RRRR_WWRR_1234( 104, divu, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff)

	TEST_RRRR_WWRR_1213( 105, divu, 0, 3, 24, 8 )
	TEST_RRRR_WWRR_1223( 106, divu, 0, 3, 27, 9 )
	TEST_RRRR_WWRR_1231( 107, divu, 0, 3, 30, 10 )
	TEST_RRRR_WWRR_1232( 108, divu, 0, 3, 33, 11 )

	TEST_RRRR_WWRR_1211( 109, divu, 0, 1, 25, 25 )
	TEST_RRRR_WWRR_1222( 110, divu, 0, 1, 36, 36 )

	TEST_RRRR_WWRR_1203( 114, divu, 0, 0, 0, 33 )
	TEST_RRRR_WWRR_0123( 115, divu, 0, 3, 20, 6 )
	TEST_RRRR_WWRR_1023( 116, divu, 2, 0, 26, 6 )

	TEST_RRRR_WWRR_1234( 202, divs,  2,  3,  20,  6)
	TEST_RRRR_WWRR_1234( 203, divs, -2, -3, -20,  6)
	TEST_RRRR_WWRR_1234( 204, divs,  2, -3,  20, -6)
	TEST_RRRR_WWRR_1234( 205, divs, -2,  3, -20, -6)

	TEST_RRRR_WWRR_1234( 206, divs, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff)
	TEST_RRRR_WWRR_1234( 207, divs, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff)

	TEST_RRRR_WWRR_1213( 208, divs, 0, 3, 24, 8 )
	TEST_RRRR_WWRR_1223( 209, divs, 0, 3, 27, 9 )
	TEST_RRRR_WWRR_1231( 210, divs, 0, 3, 30, 10 )
	TEST_RRRR_WWRR_1232( 211, divs, 0, 3, 33, 11 )

	TEST_RRRR_WWRR_1211( 212, divs, 0, 1, 25, 25 )
	TEST_RRRR_WWRR_1222( 213, divs, 0, 1, 36, 36 )

	TEST_RRRR_WWRR_1203( 217, divs, 0, 0, 0, 33 )
	TEST_RRRR_WWRR_0123( 218, divs, 0, 3, 20, 6 )
	TEST_RRRR_WWRR_1023( 219, divs, 2, 0, 26, 6 ) 

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

