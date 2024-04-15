#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_LD_D_12( 12, ldo, 0x00ff00ff00ff00ff, tdat, 0 )
	TEST_RRII_LD_D_12( 13, ldo, 0xff00ff00ff00ff00, tdat, 8 )
	TEST_RRII_LD_D_12( 14, ldo, 0x0ff00ff00ff00ff0, tdat, 16 )
	TEST_RRII_LD_D_12( 15, ldo, 0xf00ff00ff00ff00f, tdat, 24 )

	TEST_RRII_LD_D_12( 16, ldo, 0x00ff00ff00ff00ff, tdat4, -24 )
	TEST_RRII_LD_D_12( 17, ldo, 0xff00ff00ff00ff00, tdat4, -16 )
	TEST_RRII_LD_D_12( 18, ldo, 0x0ff00ff00ff00ff0, tdat4, -8 )
	TEST_RRII_LD_D_12( 19, ldo, 0xf00ff00ff00ff00f, tdat4, 0 )

	TEST_RRII_LD_D_01( 21, ldo, 0, tdat, 8 )

	__TEST_CASE( 22, 0xBEE1BEE2BEE3BEE4,
		ldo	rd16, RB_IP, 8;							\
		jump	1f;									\
		.dd.o64	0xBEE1BEE2BEE3BEE4;					\
1:		rd2rd	RD_RET1, rd16, 1;					\
		)

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat:
tdat1:	.dd.o64	0x00ff00ff00ff00ff
tdat2:	.dd.o64	0xff00ff00ff00ff00
tdat3:	.dd.o64	0x0ff00ff00ff00ff0
tdat4:	.dd.o64	0xf00ff00ff00ff00f

DDTEST_DATA_END
