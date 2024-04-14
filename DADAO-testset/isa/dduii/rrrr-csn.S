#*****************************************************************************
# csn.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	TEST_RRRR_RWRR_1234( 2, csn, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1234( 3, csn, 0xf00f0f00f0f00f00, 0x7fffffffffffffff, 0x0ff0f0ff0f0f0fff, 0xf00f0f00f0f00f00 )
	TEST_RRRR_RWRR_1234( 4, csn, 0x00ff0ff000ff0ff0, 0x0000000000000000, 0x0000f0ff0000f0ff, 0x00ff0ff000ff0ff0 )
	TEST_RRRR_RWRR_1234( 5, csn, 0xf0f0f0f0f0f0f0f0, 0x8000000f00000000, 0xf0f0f0f0f0f0f0f0, 0xf0f0ffff00f0f0ff )
	TEST_RRRR_RWRR_1234( 6, csn, 0x00f0f0ffffff0000, 0x0000000f00000000, 0xf0f0fffff0f0f0f0, 0x00f0f0ffffff0000 )

	TEST_RRRR_RWRR_1123( 7, csn, 0xff00ff0000ff00ff, 0x0000000000ff00ff, 0xffffffffffffffff, 0xff00ff0000ff00ff )
	TEST_RRRR_RWRR_1123( 8, csn, 0x0f00ff0ff0fff0ff, 0xf0ff0ff00fff00ff, 0x0f00ff0ff0fff0ff, 0xffffffffffffffff )

	TEST_RRRR_RWRR_1223( 9, csn, 0xfff000fffff000ff, 0xff00000000000000, 0xfff000fffff000ff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1223( 10, csn, 0x0000ffffffff0000, 0x0000000000000000, 0x0000000000000000, 0x0000ffffffff0000 )

	TEST_RRRR_RWRR_1232( 11, csn, 0x0f000fff0000ffff, 0xf000000000000000, 0x0f000fff0000ffff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1232( 12, csn, 0x00f0f0ff0f00f0ff, 0x0fffffffffffffff, 0x000fff0ff0f0f0ff, 0x00f0f0ff0f00f0ff )

	TEST_RRRR_RWRR_0123( 13, csn, 0xfff0f0f00f0f0f0f, 0, 0x00fff0ffff000fff, 0xfff0f0f00f0f0f0f)
	TEST_RRRR_RWRR_1203( 14, csn, 0x0000000000000000, 0xffffffffffffffff, 0, 0xfff0f0f0ffffffff)
	TEST_RRRR_RWRR_1230( 15, csn, 0x0000000000000000, 0x000000000000000f, 0xffffffffffffffff, 0)

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
