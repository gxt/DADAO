#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RWII_D( 2, setzw, w0, 0x000000000000ffff, 0xffffffffffff0000, 0xffff )
	TEST_RWII_D( 3, setzw, w1, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff )
	TEST_RWII_D( 4, setzw, w2, 0x0000ffff00000000, 0xffff0000ffffffff, 0xffff )
	TEST_RWII_D( 5, setzw, w3, 0xffff000000000000, 0x0000ffffffffffff, 0xffff )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
