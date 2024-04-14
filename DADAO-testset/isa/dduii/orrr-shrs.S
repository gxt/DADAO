#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 2, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 3, shrs, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 4, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 5, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 6, shrs, 0xfffff0f0f0fff0f0, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 7, shrs, 0xffffffffff87f87f, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 8, shrs, 0x0787878787878787, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 9, shrs, 0xffffffffffffffff, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 10, shrs, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 11, shrs, 0xffffffffffffe001, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 12, shrs, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 13, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
