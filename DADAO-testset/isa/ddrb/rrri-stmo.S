#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRI_STMO_RB_M1( 112, stmo, ldmo, 0x00ff00ff00ff00ff, tstmo_rb_b,   0, 1 )
	TEST_RRRI_STMO_RB_M1( 113, stmo, ldmo, 0xff00ff00ff00ff00, tstmo_rb_b,   8, 1 )
	TEST_RRRI_STMO_RB_M1( 114, stmo, ldmo, 0x0ff00ff00ff00ff0, tstmo_rb_b,  16, 1 )
	TEST_RRRI_STMO_RB_M1( 115, stmo, ldmo, 0xf00ff00ff00ff00f, tstmo_rb_b,  24, 1 )

	TEST_RRRI_STMO_RB_M1( 116, stmo, ldmo, 0x00ff00ff00ff00ff, tstmo_rb_e, -32, 1 )
	TEST_RRRI_STMO_RB_M1( 117, stmo, ldmo, 0xff00ff00ff00ff00, tstmo_rb_e, -24, 1 )
	TEST_RRRI_STMO_RB_M1( 118, stmo, ldmo, 0x0ff00ff00ff00ff0, tstmo_rb_e, -16, 1 )
	TEST_RRRI_STMO_RB_M1( 119, stmo, ldmo, 0xf00ff00ff00ff00f, tstmo_rb_e,  -8, 1 )

	TEST_RRRI_STMO_RB_M8( 121, stmo, ldmo, tstmo_rb_b, 0, 8 )

	TEST_RRRI_STMRA_M1( 212, stmra, ldmo, 0x00ff00ff00ff00ff, tstmra_b,   0, 1 )
	TEST_RRRI_STMRA_M1( 213, stmra, ldmo, 0xff00ff00ff00ff00, tstmra_b,   8, 1 )
	TEST_RRRI_STMRA_M1( 214, stmra, ldmo, 0x0ff00ff00ff00ff0, tstmra_b,  16, 1 )
	TEST_RRRI_STMRA_M1( 215, stmra, ldmo, 0xf00ff00ff00ff00f, tstmra_b,  24, 1 )

	TEST_RRRI_STMRA_M1( 216, stmra, ldmo, 0x00ff00ff00ff00ff, tstmra_e, -32, 1 )
	TEST_RRRI_STMRA_M1( 217, stmra, ldmo, 0xff00ff00ff00ff00, tstmra_e, -24, 1 )
	TEST_RRRI_STMRA_M1( 218, stmra, ldmo, 0x0ff00ff00ff00ff0, tstmra_e, -16, 1 )
	TEST_RRRI_STMRA_M1( 219, stmra, ldmo, 0xf00ff00ff00ff00f, tstmra_e,  -8, 1 )

//	TEST_RRRI_STMRA_M8( 221, stmra, ldmo, tstmra_b, 0, 8 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tstmo_rb_b:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstmo_rb_e:

tstmra_b:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstmra_e:

DDTEST_DATA_END
