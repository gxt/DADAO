#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_RRWR_1234(  2, cseq, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0xffffffffffffffff, 3, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RRWR_1234(  3, cseq, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000, 0x0000000000000000, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_RRRR_RRWR_1234(  4, cseq, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 3, 0x0000000000000000 )
	TEST_RRRR_RRWR_1234(  5, cseq, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 3, 0xffffffffffffffff )
	TEST_RRRR_RRWR_1234(  6, cseq, 0xf00f0f000f00ffff, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 3, 0xf00f0f000f00ffff )

	TEST_RRRR_RRWR_1234(  7, cseq, 7, 0xffffffffffffffff, 0x0000000000000000, 7, 0x00f0f0f0fff0000f )
	TEST_RRRR_RRWR_1234(  8, cseq, 8, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 8, 0xf00f00f00f00f00f )
	TEST_RRRR_RRWR_1234(  9, cseq, 9, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 9, 0x00f000f000f00fff )
	TEST_RRRR_RRWR_1234( 10, cseq, 3, 0xfffffffffffffff0, 0x000000000000000f, 3, 0xf00f0f00f0ff00f0 )

	TEST_RRRR_RRWR_0123( 11, cseq, 7, 0, 0xffffffffffffffff, 7, 0x0000000000000000 )
	TEST_RRRR_RRWR_0123( 12, cseq, 0xfff0f0f0f0f0f0f0, 0, 0x0000000000000000, 3, 0xfff0f0f0f0f0f0f0 )

	TEST_RRRR_RRWR_1023( 13, cseq, 7, 0xffffffffffffffff, 0, 7, 0x0000000000000000 )
	TEST_RRRR_RRWR_1023( 14, cseq, 0xffff000f00ffffff, 0x0000000000000000, 0, 3, 0xffff000f00ffffff )

	TEST_RRRR_RRWR_1230( 15, cseq, 7, 0x00f0ff0f0f0f0fff, 0xf00f000f00ffffff, 7, 0 )
	TEST_RRRR_RRWR_1230( 16, cseq, 0x0000000000000000, 0x000f0f0ffff00000, 0x000f0f0ffff00000, 3, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
