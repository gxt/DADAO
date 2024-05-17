#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_LDMRF_123( 112, ldmfo, 0x00ff00ff00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRF_123( 113, ldmfo, 0xff00ff00ff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRF_123( 114, ldmfo, 0x0ff00ff00ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRF_123( 115, ldmfo, 0xf00ff00ff00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRF_123( 116, ldmfo, 0x00ff00ff00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRF_123( 117, ldmfo, 0xff00ff00ff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRF_123( 118, ldmfo, 0x0ff00ff00ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRF_123( 119, ldmfo, 0xf00ff00ff00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRF_M8 ( 121, ldmfo, tdat_b, 0, 8 )

	TEST_RRRI_LDMRF_123( 612, ldmft,         0x00ff00ff, tdat_b,   0, 1 )
	TEST_RRRI_LDMRF_123( 613, ldmft,         0xff00ff00, tdat_b,   8, 1 )
	TEST_RRRI_LDMRF_123( 614, ldmft,         0x0ff00ff0, tdat_b,  16, 1 )
	TEST_RRRI_LDMRF_123( 615, ldmft,         0xf00ff00f, tdat_b,  24, 1 )

	TEST_RRRI_LDMRF_123( 616, ldmft,         0x00ff00ff, tdat_e, -32, 1 )
	TEST_RRRI_LDMRF_123( 617, ldmft,         0xff00ff00, tdat_e, -24, 1 )
	TEST_RRRI_LDMRF_123( 618, ldmft,         0x0ff00ff0, tdat_e, -16, 1 )
	TEST_RRRI_LDMRF_123( 619, ldmft,         0xf00ff00f, tdat_e,  -8, 1 )

	TEST_RRRI_LDMRF_M8 ( 621, ldmft, tdat_b, 0, 4 )

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
