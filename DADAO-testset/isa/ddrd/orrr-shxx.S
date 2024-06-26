#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 102, shlu, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 103, shlu, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 104, shlu, 0x8000000000000000, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 105, shlu, 0xfffffffffffffffe, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 106, shlu, 0xf0fff0f0f0f00000, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 107, shlu, 0x1fe1fe1e00000000, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 108, shlu, 0x1e1e1e1e1e1e1e1e, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 109, shlu, 0x0000000000000000, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 110, shlu, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 111, shlu, 0x787f800000000000, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 112, shlu, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 113, shlu, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

	TEST_ORRR_DDD_123( 202, shru, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 203, shru, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 204, shru, 0x0000000000000001, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 205, shru, 0x7fffffffffffffff, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 206, shru, 0x0000f0f0f0fff0f0, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 207, shru, 0x000000007f87f87f, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 208, shru, 0x0787878787878787, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 209, shru, 0x00000000ffffffff, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 210, shru, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 211, shru, 0x000000000001e001, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 212, shru, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 213, shru, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

	TEST_ORRR_DDD_123( 302, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 303, shrs, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
	TEST_ORRR_DDD_123( 304, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x000000000000003f )
	TEST_ORRR_DDD_123( 305, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_ORRR_DDD_123( 306, shrs, 0xfffff0f0f0fff0f0, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
	TEST_ORRR_DDD_123( 307, shrs, 0xffffffffff87f87f, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

	TEST_ORRR_DDD_112( 308, shrs, 0x0787878787878787, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
	TEST_ORRR_DDD_112( 309, shrs, 0xffffffffffffffff, 0xffffffff00000000, 0x0000000000000020 )

	TEST_ORRR_DDD_121( 310, shrs, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
	TEST_ORRR_DDD_121( 311, shrs, 0xffffffffffffe001, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

	TEST_ORRR_DDD_102( 312, shrs, 0x0000000000000000, 0, 0x0000000000000031 )
	TEST_ORRR_DDD_120( 313, shrs, 0xffffffffffffffff, 0xffffffffffffffff, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
