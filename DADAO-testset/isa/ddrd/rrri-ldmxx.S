#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_LDMRD_123( 112,  ldmo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 113,  ldmo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 114,  ldmo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 115,  ldmo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 116,  ldmo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 117,  ldmo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 118,  ldmo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 119,  ldmo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 121,  ldmo, tdat_b, 0, 8 )

	TEST_RRRI_LDMRD_123( 212, ldmbu,               0xff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 213, ldmbu,               0x00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 214, ldmbu,               0xf0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 215, ldmbu,               0x0f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 216, ldmbu,               0xff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 217, ldmbu,               0x00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 218, ldmbu,               0xf0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 219, ldmbu,               0x0f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 221, ldmbu, tdat_b, 0, 1 )

	TEST_RRRI_LDMRD_123( 312, ldmbs, 0xffffffffffffffff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 313, ldmbs, 0x0000000000000000, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 314, ldmbs, 0xfffffffffffffff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 315, ldmbs, 0x000000000000000f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 316, ldmbs, 0xffffffffffffffff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 317, ldmbs, 0x0000000000000000, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 318, ldmbs, 0xfffffffffffffff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 319, ldmbs, 0x000000000000000f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 321, ldmbs, tdat_b, 0, 1 )

	TEST_RRRI_LDMRD_123( 412, ldmwu,             0x00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 413, ldmwu,             0xff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 414, ldmwu,             0x0ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 415, ldmwu,             0xf00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 416, ldmwu,             0x00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 417, ldmwu,             0xff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 418, ldmwu,             0x0ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 419, ldmwu,             0xf00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 421, ldmwu, tdat_b, 0, 2 )

	TEST_RRRI_LDMRD_123( 512, ldmws, 0x00000000000000ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 513, ldmws, 0xffffffffffffff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 514, ldmws, 0x0000000000000ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 515, ldmws, 0xfffffffffffff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 516, ldmws, 0x00000000000000ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 517, ldmws, 0xffffffffffffff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 518, ldmws, 0x0000000000000ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 519, ldmws, 0xfffffffffffff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 521, ldmws, tdat_b, 0, 2 )

	TEST_RRRI_LDMRD_123( 612, ldmtu,         0x00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 613, ldmtu,         0xff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 614, ldmtu,         0x0ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 615, ldmtu,         0xf00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 616, ldmtu,         0x00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 617, ldmtu,         0xff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 618, ldmtu,         0x0ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 619, ldmtu,         0xf00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 621, ldmtu, tdat_b, 0, 4 )

	TEST_RRRI_LDMRD_123( 712, ldmts, 0x0000000000ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRD_123( 713, ldmts, 0xffffffffff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRD_123( 714, ldmts, 0x000000000ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRD_123( 715, ldmts, 0xfffffffff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRD_123( 716, ldmts, 0x0000000000ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRD_123( 717, ldmts, 0xffffffffff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRD_123( 718, ldmts, 0x000000000ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRD_123( 719, ldmts, 0xfffffffff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRD_M8 ( 721, ldmts, tdat_b, 0, 4 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat_b:
	.dd.o64	0x00ff00ff00ff00ff
	.dd.o64	0xff00ff00ff00ff00
	.dd.o64	0x0ff00ff00ff00ff0
	.dd.o64	0xf00ff00ff00ff00f
	.dd.o64	0x00ff00ff00ff00ff
	.dd.o64	0xff00ff00ff00ff00
	.dd.o64	0x0ff00ff00ff00ff0
	.dd.o64	0xf00ff00ff00ff00f
tdat_e:

DDTEST_DATA_END
