#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_RWRR_1234( 112, csn, 0x0f0f0f0f0f0f0f0f, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1234( 113, csn, 0xf00f0f00f0f00f00, 0x7fffffffffffffff, 0x0ff0f0ff0f0f0fff, 0xf00f0f00f0f00f00 )
	TEST_RRRR_RWRR_1234( 114, csn, 0x00ff0ff000ff0ff0, 0x0000000000000000, 0x0000f0ff0000f0ff, 0x00ff0ff000ff0ff0 )
	TEST_RRRR_RWRR_1234( 115, csn, 0xf0f0f0f0f0f0f0f0, 0x8000000f00000000, 0xf0f0f0f0f0f0f0f0, 0xf0f0ffff00f0f0ff )
	TEST_RRRR_RWRR_1234( 116, csn, 0x00f0f0ffffff0000, 0x0000000f00000000, 0xf0f0fffff0f0f0f0, 0x00f0f0ffffff0000 )

	TEST_RRRR_RWRR_1123( 117, csn, 0xff00ff0000ff00ff, 0x0000000000ff00ff, 0xffffffffffffffff, 0xff00ff0000ff00ff )
	TEST_RRRR_RWRR_1123( 118, csn, 0x0f00ff0ff0fff0ff, 0xf0ff0ff00fff00ff, 0x0f00ff0ff0fff0ff, 0xffffffffffffffff )

	TEST_RRRR_RWRR_1223( 119, csn, 0xfff000fffff000ff, 0xff00000000000000, 0xfff000fffff000ff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1223( 120, csn, 0x0000ffffffff0000, 0x0000000000000000, 0x0000000000000000, 0x0000ffffffff0000 )

	TEST_RRRR_RWRR_1232( 121, csn, 0x0f000fff0000ffff, 0xf000000000000000, 0x0f000fff0000ffff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1232( 122, csn, 0x00f0f0ff0f00f0ff, 0x0fffffffffffffff, 0x000fff0ff0f0f0ff, 0x00f0f0ff0f00f0ff )

	TEST_RRRR_RWRR_0123( 123, csn, 0xfff0f0f00f0f0f0f, 0, 0x00fff0ffff000fff, 0xfff0f0f00f0f0f0f)
	TEST_RRRR_RWRR_1203( 124, csn, 0x0000000000000000, 0xffffffffffffffff, 0, 0xfff0f0f0ffffffff)
	TEST_RRRR_RWRR_1230( 125, csn, 0x0000000000000000, 0x000000000000000f, 0xffffffffffffffff, 0)

	TEST_RRRR_RWRR_1234( 212, csz, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1234( 213, csz, 0xf00f0f00f0f00f00, 0x7fffffffffffffff, 0x0ff0f0ff0f0f0fff, 0xf00f0f00f0f00f00 )
	TEST_RRRR_RWRR_1234( 214, csz, 0x0000f0ff0000f0ff, 0x0000000000000000, 0x0000f0ff0000f0ff, 0x00ff0ff000ff0ff0 )
	TEST_RRRR_RWRR_1234( 215, csz, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0xf0f0ffff00f0f0ff )
	TEST_RRRR_RWRR_1234( 216, csz, 0x00f0f0ffffff0000, 0x0000000f00000000, 0xf0f0fffff0f0f0f0, 0x00f0f0ffffff0000 )

	TEST_RRRR_RWRR_1123( 217, csz, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 0xff00ff0000ff00ff )
	TEST_RRRR_RWRR_1123( 218, csz, 0xffffffffffffff00, 0xf0ff0ff00fff00ff, 0x0f00ff0ff0fff0ff, 0xffffffffffffff00 )

	TEST_RRRR_RWRR_1223( 219, csz, 0xf0f0f0f0f0f0f0f0, 0xff00000000000000, 0xfff000fffff000ff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1223( 220, csz, 0x00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000ffffffff0000 )

	TEST_RRRR_RWRR_1232( 221, csz, 0x0f000fff0000ffff, 0x0000000000000000, 0x0f000fff0000ffff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1232( 222, csz, 0x00f0f0ff0f00f0ff, 0x0fffffffffffffff, 0x000fff0ff0f0f0ff, 0x00f0f0ff0f00f0ff )

	TEST_RRRR_RWRR_0123( 223, csz, 0x0000000000000000, 0, 0x0000000000000000, 0xfff0f0f00f0f0f0f)
	TEST_RRRR_RWRR_1203( 224, csz, 0x0000000000000000, 0x0000000000000000, 0, 0xfff0f0f0ffffffff)
	TEST_RRRR_RWRR_1230( 225, csz, 0x0000000000000000, 0x000000000000000f, 0xffffffffffffffff, 0)

	TEST_RRRR_RWRR_1234( 312, csp, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1234( 313, csp, 0x0ff0f0ff0f0f0fff, 0x7fffffffffffffff, 0x0ff0f0ff0f0f0fff, 0xf00f0f00f0f00f00 )
	TEST_RRRR_RWRR_1234( 314, csp, 0x00ff0ff000ff0ff0, 0x0000000000000000, 0x0000f0ff0000f0ff, 0x00ff0ff000ff0ff0 )
	TEST_RRRR_RWRR_1234( 315, csp, 0xf0f0ffff00f0f0ff, 0x8000000f00000000, 0xf0f0f0f0f0f0f0f0, 0xf0f0ffff00f0f0ff )
	TEST_RRRR_RWRR_1234( 316, csp, 0xf0f0fffff0f0f0f0, 0x0000000f00000000, 0xf0f0fffff0f0f0f0, 0x00f0f0ffffff0000 )

	TEST_RRRR_RWRR_1123( 317, csp, 0xffffffffffffffff, 0x0000000000ff00ff, 0xffffffffffffffff, 0xff00ff0000ff00ff )
	TEST_RRRR_RWRR_1123( 318, csp, 0xfffffffffffff000, 0xf0ff0ff00fff00ff, 0x0f00ff0ff0fff0ff, 0xfffffffffffff000 )

	TEST_RRRR_RWRR_1223( 319, csp, 0xf0f0f0f0f0f0f0f0, 0xff00000000000000, 0xfff000fffff000ff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1223( 320, csp, 0x0000ffffffff0000, 0x0000000000000000, 0x0000000000000000, 0x0000ffffffff0000 )

	TEST_RRRR_RWRR_1232( 321, csp, 0xf0f0f0f0f0f0f0f0, 0xf000000000000000, 0x0f000fff0000ffff, 0xf0f0f0f0f0f0f0f0 )
	TEST_RRRR_RWRR_1232( 322, csp, 0x000fff0ff0f0f0ff, 0x0fffffffffffffff, 0x000fff0ff0f0f0ff, 0x00f0f0ff0f00f0ff )

	TEST_RRRR_RWRR_0123( 323, csp, 0xfff0f0f00f0f0f0f, 0, 0x00fff0ffff000fff, 0xfff0f0f00f0f0f0f)
	TEST_RRRR_RWRR_1203( 324, csp, 0x0000000000000000, 0x00ffffffffffffff, 0, 0xfff0f0f0ffffffff)
	TEST_RRRR_RWRR_1230( 325, csp, 0x0000000000000000, 0xf00000000000000f, 0xffffffffffffffff, 0)

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
