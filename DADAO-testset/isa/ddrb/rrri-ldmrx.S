#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_LDMRB_123( 112,  ldmrb, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRB_123( 113,  ldmrb, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRB_123( 114,  ldmrb, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRB_123( 115,  ldmrb, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRB_123( 116,  ldmrb, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRB_123( 117,  ldmrb, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRB_123( 118,  ldmrb, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRB_123( 119,  ldmrb, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRB_M8( 121,  ldmrb, tdat_b, 0, 8 )

	TEST_RRRI_LDMRA_123( 212,  ldmra, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRA_123( 213,  ldmra, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRA_123( 214,  ldmra, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRA_123( 215,  ldmra, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRA_123( 216,  ldmra, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRA_123( 217,  ldmra, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRA_123( 218,  ldmra, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRA_123( 219,  ldmra, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

//	TEST_RRRI_LDMRA_M8( 221,  ldmra, tdat_b, 0, 8 )

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
