#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_STMRF_M1( 112, stmfo, ldmo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_STMRF_M1( 113, stmfo, ldmo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_STMRF_M1( 114, stmfo, ldmo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_STMRF_M1( 115, stmfo, ldmo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_STMRF_M1( 116, stmfo, ldmo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_STMRF_M1( 117, stmfo, ldmo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_STMRF_M1( 118, stmfo, ldmo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_STMRF_M1( 119, stmfo, ldmo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_STMRF_M8( 121, stmfo, ldmo, tdat_b, 0, 8 )

	TEST_RRRI_STMRF_M1( 612, stmft, ldmtu,        0x00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_STMRF_M1( 613, stmft, ldmtu,        0xff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_STMRF_M1( 614, stmft, ldmtu,        0x0ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_STMRF_M1( 615, stmft, ldmtu,        0xf00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_STMRF_M1( 616, stmft, ldmtu,        0x00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_STMRF_M1( 617, stmft, ldmtu,        0xff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_STMRF_M1( 618, stmft, ldmtu,        0x0ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_STMRF_M1( 619, stmft, ldmtu,        0xf00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_STMRF_M8( 621, stmft, ldmtu, tdat_b, 0, 4 )

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
