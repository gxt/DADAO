#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_ST_F_12( 101, stfo, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tstfo_b,  0 )
	TEST_RRII_ST_F_12( 102, stfo, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tstfo_b,  8 )
	TEST_RRII_ST_F_12( 103, stfo, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tstfo_b, 16 )
	TEST_RRII_ST_F_12( 104, stfo, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tstfo_b, 24 )

	TEST_RRII_ST_F_12( 105, stfo, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tstfo_e, -32 )
	TEST_RRII_ST_F_12( 106, stfo, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tstfo_e, -24 )
	TEST_RRII_ST_F_12( 107, stfo, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tstfo_e, -16 )
	TEST_RRII_ST_F_12( 108, stfo, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tstfo_e,  -8 )

	TEST_RRII_ST_F_12( 401, stft, ldtu, 0x00ff00ff, 0x00ff00ff, tstft_b4,  0 )
	TEST_RRII_ST_F_12( 402, stft, ldtu, 0xff00ff00, 0xff00ff00, tstft_b4,  4 )
	TEST_RRII_ST_F_12( 403, stft, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstft_b4,  8 )
	TEST_RRII_ST_F_12( 404, stft, ldtu, 0xf00ff00f, 0xf00ff00f, tstft_b4, 12 )

	TEST_RRII_ST_F_12( 405, stft, ldtu, 0x00ff00ff, 0x00ff00ff, tstft_e4, -16 )
	TEST_RRII_ST_F_12( 406, stft, ldtu, 0xff00ff00, 0xff00ff00, tstft_e4, -12 )
	TEST_RRII_ST_F_12( 407, stft, ldtu, 0x0ff00ff0, 0x0ff00ff0, tstft_e4,  -8 )
	TEST_RRII_ST_F_12( 408, stft, ldtu, 0xf00ff00f, 0xf00ff00f, tstft_e4,  -4 )

		setrb	RB_DST, tstft_b4
		ldo		RD_RET2, RB_DST, 0
		setrd	RD_EXP2, 0xff00ff0000ff00ff
		brne	RD_EXP2, RD_RET2, ___fail

	TEST_RRII_ST_F_12( 411, stft, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstft_b8,  0 )
	TEST_RRII_ST_F_12( 412, stft, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstft_b8,  8 )
	TEST_RRII_ST_F_12( 413, stft, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstft_b8, 16 )
	TEST_RRII_ST_F_12( 414, stft, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstft_b8, 24 )

	TEST_RRII_ST_F_12( 415, stft, ldo, 0xdeadbeef00ff00ff, 0x00ff00ff00ff00ff, tstft_e8, -32 )
	TEST_RRII_ST_F_12( 416, stft, ldo, 0xdeadbeefff00ff00, 0xff00ff00ff00ff00, tstft_e8, -24 )
	TEST_RRII_ST_F_12( 417, stft, ldo, 0xdeadbeef0ff00ff0, 0x0ff00ff00ff00ff0, tstft_e8, -16 )
	TEST_RRII_ST_F_12( 418, stft, ldo, 0xdeadbeeff00ff00f, 0xf00ff00ff00ff00f, tstft_e8,  -8 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tstfo_b:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstfo_e:

tstft_b4:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstft_e4:

tstft_b8:
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
	.dd.o64	0xdeadbeefdeadbeef
tstft_e8:

DDTEST_DATA_END
