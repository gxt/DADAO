#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_ST_D_12( 101, sto, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_b,  0 )
	TEST_RRII_ST_D_12( 102, sto, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_b,  8 )
	TEST_RRII_ST_D_12( 103, sto, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_b, 16 )
	TEST_RRII_ST_D_12( 104, sto, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_b, 24 )

	TEST_RRII_ST_D_12( 105, sto, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_e, -32 )
	TEST_RRII_ST_D_12( 106, sto, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_e, -24 )
	TEST_RRII_ST_D_12( 107, sto, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_e, -16 )
	TEST_RRII_ST_D_12( 108, sto, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_e,  -8 )

	TEST_RRII_ST_D_12( 201, stb, ldbu, 0xff, 0xff, tstb_b1,  0 )
	TEST_RRII_ST_D_12( 202, stb, ldbu, 0x00, 0x00, tstb_b1,  1 )
	TEST_RRII_ST_D_12( 203, stb, ldbu, 0xf0, 0xf0, tstb_b1,  2 )
	TEST_RRII_ST_D_12( 204, stb, ldbu, 0x0f, 0x0f, tstb_b1,  3 )

	TEST_RRII_ST_D_12( 205, stb, ldbu, 0xff, 0xff, tstb_e1, -4 )
	TEST_RRII_ST_D_12( 206, stb, ldbu, 0x00, 0x00, tstb_e1, -3 )
	TEST_RRII_ST_D_12( 207, stb, ldbu, 0xf0, 0xf0, tstb_e1, -2 )
	TEST_RRII_ST_D_12( 208, stb, ldbu, 0x0f, 0x0f, tstb_e1, -1 )

		setrb	RB_DST, tstb_b1
		ldo		RD_RET2, RB_DST, 0
		setrd	RD_EXP2, 0x0ff000ff0ff000ff
		brne	RD_EXP2, RD_RET2, ___fail

	TEST_RRII_ST_D_12( 211, stb, ldo, 0xdeadbeefdeadbeff, 0x00ff00ff00ff00ff, tstb_b8,  0 )
	TEST_RRII_ST_D_12( 212, stb, ldo, 0xdeadbeefdeadbe00, 0xff00ff00ff00ff00, tstb_b8,  8 )
	TEST_RRII_ST_D_12( 213, stb, ldo, 0xdeadbeefdeadbef0, 0x0ff00ff00ff00ff0, tstb_b8, 16 )
	TEST_RRII_ST_D_12( 214, stb, ldo, 0xdeadbeefdeadbe0f, 0xf00ff00ff00ff00f, tstb_b8, 24 )

	TEST_RRII_ST_D_12( 215, stb, ldo, 0xdeadbeefdeadbeff, 0x00ff00ff00ff00ff, tstb_e8, -32 )
	TEST_RRII_ST_D_12( 216, stb, ldo, 0xdeadbeefdeadbe00, 0xff00ff00ff00ff00, tstb_e8, -24 )
	TEST_RRII_ST_D_12( 217, stb, ldo, 0xdeadbeefdeadbef0, 0x0ff00ff00ff00ff0, tstb_e8, -16 )
	TEST_RRII_ST_D_12( 218, stb, ldo, 0xdeadbeefdeadbe0f, 0xf00ff00ff00ff00f, tstb_e8,  -8 )

	TEST_RRII_ST_D_12( 301, stw, ldwu, 0x00ff, 0x00ff, tstw_b2,  0 )
	TEST_RRII_ST_D_12( 302, stw, ldwu, 0xff00, 0xff00, tstw_b2,  2 )
	TEST_RRII_ST_D_12( 303, stw, ldwu, 0x0ff0, 0x0ff0, tstw_b2,  4 )
	TEST_RRII_ST_D_12( 304, stw, ldwu, 0xf00f, 0xf00f, tstw_b2,  6 )

	TEST_RRII_ST_D_12( 305, stw, ldwu, 0x00ff, 0x00ff, tstw_e2, -8 )
	TEST_RRII_ST_D_12( 306, stw, ldwu, 0xff00, 0xff00, tstw_e2, -6 )
	TEST_RRII_ST_D_12( 307, stw, ldwu, 0x0ff0, 0x0ff0, tstw_e2, -4 )
	TEST_RRII_ST_D_12( 308, stw, ldwu, 0xf00f, 0xf00f, tstw_e2, -2 )

		setrb	RB_DST, tstw_b2
		ldo		RD_RET2, RB_DST, 0
		setrd	RD_EXP2, 0xf00f0ff0ff0000ff
		brne	RD_EXP2, RD_RET2, ___fail

	TEST_RRII_ST_D_12( 311, stw, ldo, 0xdeadbeefdead00ff, 0x00ff00ff00ff00ff, tstw_b8,  0 )
	TEST_RRII_ST_D_12( 312, stw, ldo, 0xdeadbeefdeadff00, 0xff00ff00ff00ff00, tstw_b8,  8 )
	TEST_RRII_ST_D_12( 313, stw, ldo, 0xdeadbeefdead0ff0, 0x0ff00ff00ff00ff0, tstw_b8, 16 )
	TEST_RRII_ST_D_12( 314, stw, ldo, 0xdeadbeefdeadf00f, 0xf00ff00ff00ff00f, tstw_b8, 24 )

	TEST_RRII_ST_D_12( 315, stw, ldo, 0xdeadbeefdead00ff, 0x00ff00ff00ff00ff, tstw_e8, -32 )
	TEST_RRII_ST_D_12( 316, stw, ldo, 0xdeadbeefdeadff00, 0xff00ff00ff00ff00, tstw_e8, -24 )
	TEST_RRII_ST_D_12( 317, stw, ldo, 0xdeadbeefdead0ff0, 0x0ff00ff00ff00ff0, tstw_e8, -16 )
	TEST_RRII_ST_D_12( 318, stw, ldo, 0xdeadbeefdeadf00f, 0xf00ff00ff00ff00f, tstw_e8,  -8 )

	TEST_RRII_ST_D_12( 401, stt, ldtu, 0x00ff00ff, 0x00ff00ff, tstt_b4,  0 )
	TEST_RRII_ST_D_12( 402, stt, ldtu, 0xff00ff00, 0xff00ff00, tstt_b4,  4 )
	TEST_RRII_ST_D_12( 403, stt, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstt_b4,  8 )
	TEST_RRII_ST_D_12( 404, stt, ldtu, 0xf00ff00f, 0xf00ff00f, tstt_b4, 12 )

	TEST_RRII_ST_D_12( 405, stt, ldtu, 0x00ff00ff, 0x00ff00ff, tstt_e4, -16 )
	TEST_RRII_ST_D_12( 406, stt, ldtu, 0xff00ff00, 0xff00ff00, tstt_e4, -12 )
	TEST_RRII_ST_D_12( 407, stt, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstt_e4,  -8 )
	TEST_RRII_ST_D_12( 408, stt, ldtu, 0xf00ff00f, 0xf00ff00f, tstt_e4,  -4 )

		setrb	RB_DST, tstt_b4
		ldo		RD_RET2, RB_DST, 0
		setrd	RD_EXP2, 0xff00ff0000ff00ff
		brne	RD_EXP2, RD_RET2, ___fail

	TEST_RRII_ST_D_12( 411, stt, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstt_b8,  0 )
	TEST_RRII_ST_D_12( 412, stt, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstt_b8,  8 )
	TEST_RRII_ST_D_12( 413, stt, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstt_b8, 16 )
	TEST_RRII_ST_D_12( 414, stt, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstt_b8, 24 )

	TEST_RRII_ST_D_12( 415, stt, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstt_e8, -32 )
	TEST_RRII_ST_D_12( 416, stt, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstt_e8, -24 )
	TEST_RRII_ST_D_12( 417, stt, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstt_e8, -16 )
	TEST_RRII_ST_D_12( 418, stt, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstt_e8,  -8 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tsto_b:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tsto_e:

tstb_b1:
	.dd.o64	0xdeadbeefdeadbeef
tstb_e1:

tstb_b8:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstb_e8:

tstw_b2:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstw_e2:

tstw_b8:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstw_e8:

tstt_b4:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstt_e4:

tstt_b8:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstt_e8:

DDTEST_DATA_END
