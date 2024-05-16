#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_ST_B_12( 101, strb, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_b,  0 )
	TEST_RRII_ST_B_12( 102, strb, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_b,  8 )
	TEST_RRII_ST_B_12( 103, strb, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_b, 16 )
	TEST_RRII_ST_B_12( 104, strb, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_b, 24 )

	TEST_RRII_ST_B_12( 105, strb, ldo, 0x00ff00ff00ff00ff, 0x00ff00ff00ff00ff, tsto_e, -32 )
	TEST_RRII_ST_B_12( 106, strb, ldo, 0xff00ff00ff00ff00, 0xff00ff00ff00ff00, tsto_e, -24 )
	TEST_RRII_ST_B_12( 107, strb, ldo, 0x0ff00ff00ff00ff0, 0x0ff00ff00ff00ff0, tsto_e, -16 )
	TEST_RRII_ST_B_12( 108, strb, ldo, 0xf00ff00ff00ff00f, 0xf00ff00ff00ff00f, tsto_e,  -8 )

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

DDTEST_DATA_END
