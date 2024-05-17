#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_RWR_123( 112, csp1, 0xf0f0f0f0f0f0f0f0, 1, 3, 0xf0f0f0f0f0f0f0f0 )
	TEST_FRRR_RWR_123( 113, csp1, 0x0f0f0f0f0f0f0f0f, 1, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_FRRR_RWR_123( 114, csp1, 0x0000000000000000, 1, 3, 0x0000000000000000 )
	TEST_FRRR_RWR_123( 115, csp1, 0xffffffffffffffff, 1, 3, 0xffffffffffffffff )

	TEST_FRRR_RWR_123( 117, csp1, 7,  0, 7, 0x00f0f0f0fff0000f )
	TEST_FRRR_RWR_123( 118, csp1, 8, -1, 8, 0xf00f00f00f00f00f )
	TEST_FRRR_RWR_123( 119, csp1, 9, 11, 9, 0x00f000f000f00fff )
	TEST_FRRR_RWR_123( 120, csp1, 3, -5, 3, 0xf00f0f00f0ff00f0 )

	TEST_FRRR_RWR_012( 121, csp1, 7, 0, 7, 0x0000000000000000 )

	TEST_FRRR_RWR_123( 212, csnp1, 0x00f0f0f0fff0000f,  0, 2, 0x00f0f0f0fff0000f )
	TEST_FRRR_RWR_123( 213, csnp1, 0xf00f00f00f00f00f, -1, 3, 0xf00f00f00f00f00f )
	TEST_FRRR_RWR_123( 214, csnp1, 0x00f000f000f00fff, 11, 4, 0x00f000f000f00fff )
	TEST_FRRR_RWR_123( 215, csnp1, 0xf00f0f00f0ff00f0, -5, 5, 0xf00f0f00f0ff00f0 )

	TEST_FRRR_RWR_123( 217, csnp1, 2, 1, 2, 0xf0f0f0f0f0f0f0f0 )
	TEST_FRRR_RWR_123( 218, csnp1, 3, 1, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_FRRR_RWR_123( 219, csnp1, 4, 1, 4, 0x0000000000000000 )
	TEST_FRRR_RWR_123( 220, csnp1, 5, 1, 5, 0xffffffffffffffff )
	TEST_FRRR_RWR_123( 221, csnp1, 6, 1, 6, 0xf00f0f000f00ffff )

	TEST_FRRR_RWR_012( 222, csnp1, 0xff00ff00ff00ff00, 0, 2, 0xff00ff00ff00ff00 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
