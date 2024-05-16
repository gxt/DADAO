#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_LD_B_12( 112, ldrb, 0x00ff00ff00ff00ff, tdat_b, 0 )
	TEST_RRII_LD_B_12( 113, ldrb, 0xff00ff00ff00ff00, tdat_b, 8 )
	TEST_RRII_LD_B_12( 114, ldrb, 0x0ff00ff00ff00ff0, tdat_b, 16 )
	TEST_RRII_LD_B_12( 115, ldrb, 0xf00ff00ff00ff00f, tdat_b, 24 )

	TEST_RRII_LD_B_12( 116, ldrb, 0x00ff00ff00ff00ff, tdat_e, -32 )
	TEST_RRII_LD_B_12( 117, ldrb, 0xff00ff00ff00ff00, tdat_e, -24 )
	TEST_RRII_LD_B_12( 118, ldrb, 0x0ff00ff00ff00ff0, tdat_e, -16 )
	TEST_RRII_LD_B_12( 119, ldrb, 0xf00ff00ff00ff00f, tdat_e, -8 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat_b:
	.dd.o64	0x00ff00ff00ff00ff
	.dd.o64	0xff00ff00ff00ff00
	.dd.o64	0x0ff00ff00ff00ff0
	.dd.o64	0xf00ff00ff00ff00f
tdat_e:

DDTEST_DATA_END
