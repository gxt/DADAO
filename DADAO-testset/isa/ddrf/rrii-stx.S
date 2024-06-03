#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_ST_F_12( 101, sto, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_b,  0 )
	TEST_RRII_ST_F_12( 102, sto, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_b,  8 )
	TEST_RRII_ST_F_12( 103, sto, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_b, 16 )
	TEST_RRII_ST_F_12( 104, sto, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_b, 24 )

	TEST_RRII_ST_F_12( 105, sto, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_e, -32 )
	TEST_RRII_ST_F_12( 106, sto, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_e, -24 )
	TEST_RRII_ST_F_12( 107, sto, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_e, -16 )
	TEST_RRII_ST_F_12( 108, sto, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_e,  -8 )

	TEST_RRII_ST_F_12( 401, stt, ldtu, 0x00ff00ff, 0x00ff00ff, tstt_b4,  0 )
	TEST_RRII_ST_F_12( 402, stt, ldtu, 0xff00ff00, 0xff00ff00, tstt_b4,  4 )
	TEST_RRII_ST_F_12( 403, stt, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstt_b4,  8 )
	TEST_RRII_ST_F_12( 404, stt, ldtu, 0xf00ff00f, 0xf00ff00f, tstt_b4, 12 )

	TEST_RRII_ST_F_12( 405, stt, ldtu, 0x00ff00ff, 0x00ff00ff, tstt_e4, -16 )
	TEST_RRII_ST_F_12( 406, stt, ldtu, 0xff00ff00, 0xff00ff00, tstt_e4, -12 )
	TEST_RRII_ST_F_12( 407, stt, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstt_e4,  -8 )
	TEST_RRII_ST_F_12( 408, stt, ldtu, 0xf00ff00f, 0xf00ff00f, tstt_e4,  -4 )

		setrb	RB_DST, tstt_b4
		ldo		RD_RET2, RB_DST, 0
		setrd	RD_EXP2, 0xff00ff0000ff00ff
		brne	RD_EXP2, RD_RET2, ___fail

	TEST_RRII_ST_F_12( 411, stt, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstt_b8,  0 )
	TEST_RRII_ST_F_12( 412, stt, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstt_b8,  8 )
	TEST_RRII_ST_F_12( 413, stt, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstt_b8, 16 )
	TEST_RRII_ST_F_12( 414, stt, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstt_b8, 24 )

	TEST_RRII_ST_F_12( 415, stt, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstt_e8, -32 )
	TEST_RRII_ST_F_12( 416, stt, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstt_e8, -24 )
	TEST_RRII_ST_F_12( 417, stt, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstt_e8, -16 )
	TEST_RRII_ST_F_12( 418, stt, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstt_e8,  -8 )

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
