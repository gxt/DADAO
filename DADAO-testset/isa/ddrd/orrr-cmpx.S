#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRR_DDD_123( 117, cmpu, 0x0, 0x0, 0x0 )
	TEST_ORRR_DDD_123( 118, cmpu, 0x1, 0x5, 0x0 )
	TEST_ORRR_DDD_123( 119, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 )

	TEST_ORRR_DDD_123( 120, cmpu, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 121, cmpu, 0x0000000000000001, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f )
	TEST_ORRR_DDD_123( 122, cmpu, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )

	TEST_ORRR_DDD_112( 123, cmpu,  1, 5, 4 )
	TEST_ORRR_DDD_121( 124, cmpu, -1, 4, 5 )
	TEST_ORRR_DDD_111( 125, cmpu,  0, 6, 6 )

	TEST_ORRR_DDD_102( 126, cmpu, -1, 0, 1 )
	TEST_ORRR_DDD_120( 127, cmpu,  1, 5, 0 )
	TEST_ORRR_DDD_100( 128, cmpu,  0, 0, 0 ) 
	TEST_ORRR_DDD_012( 129, cmpu,  0, 5, 6 )

	TEST_ORRR_DDD_123( 223, cmps, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 )
	TEST_ORRR_DDD_123( 224, cmps, 0x0000000000000000, 0x0fffffffffffffff, 0x0fffffffffffffff )
	TEST_ORRR_DDD_123( 225, cmps, 0xffffffffffffffff, 0x0000000000000003, 0x0000000000000005 )
	TEST_ORRR_DDD_123( 226, cmps, 0x0000000000000001, 0x0000000000000005, 0x0000000000000002 )
	TEST_ORRR_DDD_123( 227, cmps, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 228, cmps, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff )
	TEST_ORRR_DDD_123( 229, cmps, 0x0000000000000001, 0xffffffffffffffff, 0xfffffffffffffffe )
	TEST_ORRR_DDD_123( 230, cmps, 0x0000000000000001, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_ORRR_DDD_123( 231, cmps, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f )

	TEST_ORRR_DDD_112( 232, cmps, 1, 6, 4 )
	TEST_ORRR_DDD_121( 233, cmps, 1, 7, 5 )
	TEST_ORRR_DDD_111( 234, cmps, 0, 8, 8 )

	TEST_ORRR_DDD_102( 235, cmps, 1, 0, -1)
	TEST_ORRR_DDD_102( 236, cmps, 0, 0, 0)
	TEST_ORRR_DDD_120( 237, cmps, 1, 1, 0)
	TEST_ORRR_DDD_120( 238, cmps, 0, 0, 0)
	TEST_ORRR_DDD_100( 239, cmps, 0, 0, 0)
	TEST_ORRR_DDD_012( 240, cmps, 0, 5, 6 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
