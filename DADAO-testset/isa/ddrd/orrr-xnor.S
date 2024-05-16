#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 102, xnor, 0xfffffffff00ff00f, 0xff00ff00, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 103, xnor, 0xffffffffff00ff00, 0x0ff00ff0, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 104, xnor, 0xffffffff0ff00ff0, 0x00ff00ff, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 105, xnor, 0xffffffff00ff00ff, 0xf00ff00f, 0x0f0f0f0f )

	TEST_ORRR_DDD_112( 106, xnor, 0xfffffffff00ff00f, 0xff00ff00, 0xf0f0f0f0 )
	TEST_ORRR_DDD_121( 107, xnor, 0xfffffffff00ff00f, 0xff00ff00, 0xf0f0f0f0 )
	TEST_ORRR_DDD_111( 108, xnor, 0xffffffffffffffff, 0xff00ff00, 0xff00ff00 )

	TEST_ORRR_DDD_102( 124, xnor, 0xffffffffff00ff00,          0, 0x00ff00ff )
	TEST_ORRR_DDD_120( 125, xnor, 0xffffffffff00ff00, 0x00ff00ff,          0 )
	TEST_ORRR_DDD_100( 126, xnor, 0xffffffffffffffff,          0,          0 )
	TEST_ORRR_DDD_012( 127, xnor,                  0, 0x11111111, 0x22222222 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
