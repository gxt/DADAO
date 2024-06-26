#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_DD_12( 2, andi, 0x0000000000000f00, 0xffffffffff00ff00, 0xf0f )
	TEST_RRII_DD_12( 3, andi, 0x00000000000000f0, 0x000000000ff00ff0, 0x0f0 )
	TEST_RRII_DD_12( 4, andi, 0x000000000000000f, 0x0000000000ff00ff, 0x70f )
	TEST_RRII_DD_12( 5, andi, 0x0000000000000000, 0xfffffffff00ff00f, 0x0f0 )

	TEST_RRII_DD_11( 6, andi, 0x0000000000000000, 0xffffffffff00ff00, 0x0f0 )

	TEST_RRII_DD_10( 13, andi, 0, 0, 0xf)
	TEST_RRII_DD_01( 14, andi, 0, 0xf, 0xf)

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
