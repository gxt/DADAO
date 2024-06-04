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

	TEST_RRRI_STMO_RA_M1( 212, stmo, ldmo, 0x00ff00ff00ff00ff, tstmo_ra_b,   0, 1 )
	TEST_RRRI_STMO_RA_M1( 213, stmo, ldmo, 0xff00ff00ff00ff00, tstmo_ra_b,   8, 1 )
	TEST_RRRI_STMO_RA_M1( 214, stmo, ldmo, 0x0ff00ff00ff00ff0, tstmo_ra_b,  16, 1 )
	TEST_RRRI_STMO_RA_M1( 215, stmo, ldmo, 0xf00ff00ff00ff00f, tstmo_ra_b,  24, 1 )

	TEST_RRRI_STMO_RA_M1( 216, stmo, ldmo, 0x00ff00ff00ff00ff, tstmo_ra_e, -32, 1 )
	TEST_RRRI_STMO_RA_M1( 217, stmo, ldmo, 0xff00ff00ff00ff00, tstmo_ra_e, -24, 1 )
	TEST_RRRI_STMO_RA_M1( 218, stmo, ldmo, 0x0ff00ff00ff00ff0, tstmo_ra_e, -16, 1 )
	TEST_RRRI_STMO_RA_M1( 219, stmo, ldmo, 0xf00ff00ff00ff00f, tstmo_ra_e,  -8, 1 )

//	TEST_RRRI_STMO_RA_M8( 221, stmo, ldmo, tstmo_ra_b, 0, 8 )

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

tstmo_ra_b:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstmo_ra_e:

DDTEST_DATA_END
