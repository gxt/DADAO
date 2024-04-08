#*****************************************************************************
# csne.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_RRRR_RRWR_1234( 2, csne, 0x00f0f0f0fff0000f, 0xffffffffffffffff, 0x0000000000000000, 2, 0x00f0f0f0fff0000f )
	TEST_RRRR_RRWR_1234( 3, csne, 0xf00f00f00f00f00f, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 3, 0xf00f00f00f00f00f )
	TEST_RRRR_RRWR_1234( 4, csne, 0x00f000f000f00fff, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 4, 0x00f000f000f00fff )
	TEST_RRRR_RRWR_1234( 5, csne, 0xf00f0f00f0ff00f0, 0xfffffffffffffff0, 0x000000000000000f, 5, 0xf00f0f00f0ff00f0 )
	TEST_RRRR_RRWR_1234( 6, csne, 0x0ff0ff0fff0fffff, 0x00000000000000ff, 0x000000000000000f, 6, 0x0ff0ff0fff0fffff )

	TEST_RRRR_RRWR_1234(  7, csne, 2, 0xffffffffffffffff, 0xffffffffffffffff, 2, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RRWR_1234(  8, csne, 3, 0x0000000000000000, 0x0000000000000000, 3, 0x0f0f0f0f0f0f0f0f )
	TEST_RRRR_RRWR_1234(  9, csne, 4, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 4, 0x0000000000000000 )
	TEST_RRRR_RRWR_1234( 10, csne, 5, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 5, 0xffffffffffffffff )
	TEST_RRRR_RRWR_1234( 11, csne, 6, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 6, 0xf00f0f000f00ffff )

	TEST_RRRR_RRWR_0123( 12, csne, 0xff00ff00ff00ff00, 0, 0xffffffffffffffff, 2, 0xff00ff00ff00ff00 )
	TEST_RRRR_RRWR_0123( 13, csne, 3, 0, 0x0000000000000000, 3, 0xfff0f0f0f0f0f0f0 )

	TEST_RRRR_RRWR_1023( 14, csne, 0x00ff00ff00ff00ff, 0xffffffffffffffff, 0, 4, 0x00ff00ff00ff00ff )
	TEST_RRRR_RRWR_1023( 15, csne, 5, 0x0000000000000000, 0, 5, 0xffff000f00ffffff )

	TEST_RRRR_RRWR_1230( 16, csne, 6, 0x00f0ff0f0f0f0fff, 0x00f0ff0f0f0f0fff, 6, 0 )
	TEST_RRRR_RRWR_1230( 17, csne, 0, 0x000f0f0ffff00000, 0x000f0f0ffff000f0, 7, 0 )

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
