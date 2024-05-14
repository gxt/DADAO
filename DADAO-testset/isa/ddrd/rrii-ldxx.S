#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_LD_D_12( 112, ldo, 0x00ff00ff00ff00ff, tdat, 0 )
	TEST_RRII_LD_D_12( 113, ldo, 0xff00ff00ff00ff00, tdat, 8 )
	TEST_RRII_LD_D_12( 114, ldo, 0x0ff00ff00ff00ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 115, ldo, 0xf00ff00ff00ff00f, tdat, 24 )

	TEST_RRII_LD_D_12( 116, ldo, 0x00ff00ff00ff00ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 117, ldo, 0xff00ff00ff00ff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 118, ldo, 0x0ff00ff00ff00ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 119, ldo, 0xf00ff00ff00ff00f, tdat4, 0 )

	TEST_RRII_LD_D_01( 121, ldo, 0, tdat, 8 )

	TEST_RRII_LD_D_10( 122, ldo, .dd.o64, 0xBEE1BEE2BEE3BEE4, 8 )

	TEST_RRII_LD_D_12( 212, ldbu, 0xff, tdat, 0 )
	TEST_RRII_LD_D_12( 213, ldbu, 0x00, tdat, 8 )
	TEST_RRII_LD_D_12( 214, ldbu, 0xf0, tdat, 16 )
	TEST_RRII_LD_D_12( 215, ldbu, 0x0f, tdat, 24 )

	TEST_RRII_LD_D_12( 216, ldbu, 0xff, tdat4, -24 )
	TEST_RRII_LD_D_12( 217, ldbu, 0x00, tdat4, -16 )
	TEST_RRII_LD_D_12( 218, ldbu, 0xf0, tdat4, -8 )
	TEST_RRII_LD_D_12( 219, ldbu, 0x0f, tdat4, 0 )

	TEST_RRII_LD_D_10( 221, ldbu, .dd.b08, 0xE1, 8 )
	TEST_RRII_LD_D_10( 222, ldbu, .dd.b08, 0x35, 8 )

	TEST_RRII_LD_D_12( 312, ldbs, 0xffffffffffffffff, tdat, 0 )
	TEST_RRII_LD_D_12( 313, ldbs, 0x0000000000000000, tdat, 8 )
	TEST_RRII_LD_D_12( 314, ldbs, 0xfffffffffffffff0, tdat, 16 )
	TEST_RRII_LD_D_12( 315, ldbs, 0x000000000000000f, tdat, 24 )

	TEST_RRII_LD_D_12( 316, ldbs, 0xffffffffffffffff, tdat4, -24 )
	TEST_RRII_LD_D_12( 317, ldbs, 0x0000000000000000, tdat4, -16 )
	TEST_RRII_LD_D_12( 318, ldbs, 0xfffffffffffffff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 319, ldbs, 0x000000000000000f, tdat4, 0 )

	TEST_RRII_LD_D_10( 321, ldbs, .dd.b08, 0xffffffffffffffE1, 8 )
	TEST_RRII_LD_D_10( 322, ldbs, .dd.b08, 0x0000000000000035, 8 )

	TEST_RRII_LD_D_12( 412, ldwu, 0x00ff, tdat, 0 )
	TEST_RRII_LD_D_12( 413, ldwu, 0xff00, tdat, 8 )
	TEST_RRII_LD_D_12( 414, ldwu, 0x0ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 415, ldwu, 0xf00f, tdat, 24 )

	TEST_RRII_LD_D_12( 416, ldwu, 0x00ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 417, ldwu, 0xff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 418, ldwu, 0x0ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 419, ldwu, 0xf00f, tdat4, 0 )

	TEST_RRII_LD_D_10( 421, ldwu, .dd.w16, 0xE1E2, 8 )
	TEST_RRII_LD_D_10( 422, ldwu, .dd.w16, 0x3536, 8 )

	TEST_RRII_LD_D_12( 512, ldws, 0x00000000000000ff, tdat, 0 )
	TEST_RRII_LD_D_12( 513, ldws, 0xffffffffffffff00, tdat, 8 )
	TEST_RRII_LD_D_12( 514, ldws, 0x0000000000000ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 515, ldws, 0xfffffffffffff00f, tdat, 24 )

	TEST_RRII_LD_D_12( 516, ldws, 0x00000000000000ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 517, ldws, 0xffffffffffffff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 518, ldws, 0x0000000000000ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 519, ldws, 0xfffffffffffff00f, tdat4, 0 )

	TEST_RRII_LD_D_10( 521, ldws, .dd.w16, 0xffffffffffffE1E2, 8 )
	TEST_RRII_LD_D_10( 522, ldws, .dd.w16, 0x0000000000003536, 8 )

	TEST_RRII_LD_D_12( 612, ldtu, 0x00ff00ff, tdat, 0 )
	TEST_RRII_LD_D_12( 613, ldtu, 0xff00ff00, tdat, 8 )
	TEST_RRII_LD_D_12( 614, ldtu, 0x0ff00ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 615, ldtu, 0xf00ff00f, tdat, 24 )

	TEST_RRII_LD_D_12( 616, ldtu, 0x00ff00ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 617, ldtu, 0xff00ff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 618, ldtu, 0x0ff00ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 619, ldtu, 0xf00ff00f, tdat4, 0 )

	TEST_RRII_LD_D_10( 621, ldtu, .dd.t32, 0xE1E2E3E4, 8 )
	TEST_RRII_LD_D_10( 622, ldtu, .dd.t32, 0x35363738, 8 )

	TEST_RRII_LD_D_12( 712, ldts, 0x0000000000ff00ff, tdat, 0 )
	TEST_RRII_LD_D_12( 713, ldts, 0xffffffffff00ff00, tdat, 8 )
	TEST_RRII_LD_D_12( 714, ldts, 0x000000000ff00ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 715, ldts, 0xfffffffff00ff00f, tdat, 24 )

	TEST_RRII_LD_D_12( 716, ldts, 0x0000000000ff00ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 717, ldts, 0xffffffffff00ff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 718, ldts, 0x000000000ff00ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 719, ldts, 0xfffffffff00ff00f, tdat4, 0 )

	TEST_RRII_LD_D_10( 721, ldts, .dd.t32, 0xffffffffE1E2E3E4, 8 )
	TEST_RRII_LD_D_10( 722, ldts, .dd.t32, 0x0000000035363738, 8 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat:
tdat1:	.dd.o64	0x00ff00ff00ff00ff
tdat2:	.dd.o64	0xff00ff00ff00ff00
tdat3:	.dd.o64	0x0ff00ff00ff00ff0
tdat4:	.dd.o64	0xf00ff00ff00ff00f

DDTEST_DATA_END
