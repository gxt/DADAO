#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_STMRF_M1( 112, stmo, ldmo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_STMRF_M1( 113, stmo, ldmo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_STMRF_M1( 114, stmo, ldmo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_STMRF_M1( 115, stmo, ldmo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_STMRF_M1( 116, stmo, ldmo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_STMRF_M1( 117, stmo, ldmo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_STMRF_M1( 118, stmo, ldmo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_STMRF_M1( 119, stmo, ldmo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_STMRF_M8( 121, stmo, ldmo, tdat_b, 0, 8 )

	TEST_RRRI_STMRF_M1( 612, stmt, ldmtu,        0x00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_STMRF_M1( 613, stmt, ldmtu,        0xff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_STMRF_M1( 614, stmt, ldmtu,        0x0ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_STMRF_M1( 615, stmt, ldmtu,        0xf00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_STMRF_M1( 616, stmt, ldmtu,        0x00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_STMRF_M1( 617, stmt, ldmtu,        0xff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_STMRF_M1( 618, stmt, ldmtu,        0x0ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_STMRF_M1( 619, stmt, ldmtu,        0xf00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_STMRF_M8( 621, stmt, ldmtu, tdat_b, 0, 4 )

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
