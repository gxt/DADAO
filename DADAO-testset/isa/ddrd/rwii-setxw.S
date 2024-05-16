#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RWII_D( 102, setzw, w0, 0x000000000000ffff, 0xffffffffffff0000, 0xffff )
	TEST_RWII_D( 103, setzw, w1, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff )
	TEST_RWII_D( 104, setzw, w2, 0x0000ffff00000000, 0xffff0000ffffffff, 0xffff )
	TEST_RWII_D( 105, setzw, w3, 0xffff000000000000, 0x0000ffffffffffff, 0xffff )

	TEST_RWII_D( 202, setow, w0, 0xffffffffffff0000, 0x000000000000ffff, 0x0 )
	TEST_RWII_D( 203, setow, w1, 0xffffffff0000ffff, 0x00000000ffff0000, 0x0 )
	TEST_RWII_D( 204, setow, w2, 0xffff0000ffffffff, 0x0000ffff00000000, 0x0 )
	TEST_RWII_D( 205, setow, w3, 0x0000ffffffffffff, 0xffff000000000000, 0x0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
