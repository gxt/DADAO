#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_RRWR_1234( 112, cseq, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0xffffffffffffffff, 3, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RRWR_1234( 113, cseq, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000, 0x0000000000000000, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_RRRR_RRWR_1234( 114, cseq, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 3, 0x0000000000000000 )
	TEST_RRRR_RRWR_1234( 115, cseq, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 3, 0xffffffffffffffff )
	TEST_RRRR_RRWR_1234( 116, cseq, 0xf00f0f000f00ffff, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 3, 0xf00f0f000f00ffff )

	TEST_RRRR_RRWR_1234( 117, cseq, 7, 0xffffffffffffffff, 0x0000000000000000, 7, 0x00f0f0f0fff0000f )
	TEST_RRRR_RRWR_1234( 118, cseq, 8, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 8, 0xf00f00f00f00f00f )
	TEST_RRRR_RRWR_1234( 119, cseq, 9, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 9, 0x00f000f000f00fff )
	TEST_RRRR_RRWR_1234( 120, cseq, 3, 0xfffffffffffffff0, 0x000000000000000f, 3, 0xf00f0f00f0ff00f0 )

	TEST_RRRR_RRWR_0123( 121, cseq, 7, 0, 0xffffffffffffffff, 7, 0x0000000000000000 )
	TEST_RRRR_RRWR_0123( 122, cseq, 0xfff0f0f0f0f0f0f0, 0, 0x0000000000000000, 3, 0xfff0f0f0f0f0f0f0 )

	TEST_RRRR_RRWR_1023( 123, cseq, 7, 0xffffffffffffffff, 0, 7, 0x0000000000000000 )
	TEST_RRRR_RRWR_1023( 124, cseq, 0xffff000f00ffffff, 0x0000000000000000, 0, 3, 0xffff000f00ffffff )

	TEST_RRRR_RRWR_1230( 125, cseq, 7, 0x00f0ff0f0f0f0fff, 0xf00f000f00ffffff, 7, 0 )
	TEST_RRRR_RRWR_1230( 126, cseq, 0x0000000000000000, 0x000f0f0ffff00000, 0x000f0f0ffff00000, 3, 0 )

	TEST_RRRR_RRWR_1234( 212, csne, 0x00f0f0f0fff0000f, 0xffffffffffffffff, 0x0000000000000000, 2, 0x00f0f0f0fff0000f )
	TEST_RRRR_RRWR_1234( 213, csne, 0xf00f00f00f00f00f, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 3, 0xf00f00f00f00f00f )
	TEST_RRRR_RRWR_1234( 214, csne, 0x00f000f000f00fff, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 4, 0x00f000f000f00fff )
	TEST_RRRR_RRWR_1234( 215, csne, 0xf00f0f00f0ff00f0, 0xfffffffffffffff0, 0x000000000000000f, 5, 0xf00f0f00f0ff00f0 )
	TEST_RRRR_RRWR_1234( 216, csne, 0x0ff0ff0fff0fffff, 0x00000000000000ff, 0x000000000000000f, 6, 0x0ff0ff0fff0fffff )

	TEST_RRRR_RRWR_1234( 217, csne, 2, 0xffffffffffffffff, 0xffffffffffffffff, 2, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RRWR_1234( 218, csne, 3, 0x0000000000000000, 0x0000000000000000, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_RRRR_RRWR_1234( 219, csne, 4, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 4, 0x0000000000000000 )
	TEST_RRRR_RRWR_1234( 220, csne, 5, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 5, 0xffffffffffffffff )
	TEST_RRRR_RRWR_1234( 221, csne, 6, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 6, 0xf00f0f000f00ffff )

	TEST_RRRR_RRWR_0123( 222, csne, 0xff00ff00ff00ff00, 0, 0xffffffffffffffff, 2, 0xff00ff00ff00ff00 )
	TEST_RRRR_RRWR_0123( 223, csne, 3, 0, 0x0000000000000000, 3, 0xfff0f0f0f0f0f0f0 )

	TEST_RRRR_RRWR_1023( 224, csne, 0x00ff00ff00ff00ff, 0xffffffffffffffff, 0, 4, 0x00ff00ff00ff00ff )
	TEST_RRRR_RRWR_1023( 225, csne, 5, 0x0000000000000000, 0, 5, 0xffff000f00ffffff )

	TEST_RRRR_RRWR_1230( 226, csne, 6, 0x00f0ff0f0f0f0fff, 0x00f0ff0f0f0f0fff, 6, 0 )
	TEST_RRRR_RRWR_1230( 227, csne, 0, 0x000f0f0ffff00000, 0x000f0f0ffff000f0, 7, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
