#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_LDMO_RB_123( 112,  ldmo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMO_RB_123( 113,  ldmo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMO_RB_123( 114,  ldmo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMO_RB_123( 115,  ldmo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMO_RB_123( 116,  ldmo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMO_RB_123( 117,  ldmo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMO_RB_123( 118,  ldmo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMO_RB_123( 119,  ldmo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMO_RB_M8( 121,  ldmo, tdat_b, 0, 8 )

	TEST_RRRI_LDMO_RA_123( 212,  ldmo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMO_RA_123( 213,  ldmo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMO_RA_123( 214,  ldmo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMO_RA_123( 215,  ldmo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMO_RA_123( 216,  ldmo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMO_RA_123( 217,  ldmo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMO_RA_123( 218,  ldmo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMO_RA_123( 219,  ldmo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

//	TEST_RRRI_LDMO_RA_M8( 221,  ldmo, tdat_b, 0, 8 )

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
