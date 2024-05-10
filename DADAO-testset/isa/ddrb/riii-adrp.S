#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RIII( 2, adrp, 0)
	TEST_RIII( 3, adrp, 5)
	TEST_RIII( 4, adrp, 0x10000)
	TEST_RIII( 5, adrp, -5)
	TEST_RIII( 6, adrp, 0x1ffff)
	TEST_RIII( 7, adrp, -0x20000)

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
 
