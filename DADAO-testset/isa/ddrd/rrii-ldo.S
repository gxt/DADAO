#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_LD_D( 2, ldo, 0x00ff00ff00ff00ff, tdat, 0 )
	TEST_RRII_LD_D( 3, ldo, 0xff00ff00ff00ff00, tdat, 8 )
	TEST_RRII_LD_D( 4, ldo, 0x0ff00ff00ff00ff0, tdat, 16 )
	TEST_RRII_LD_D( 5, ldo, 0xf00ff00ff00ff00f, tdat, 24 )

	TEST_RRII_LD_D( 6, ldo, 0x00ff00ff00ff00ff, tdat4, -24 )
	TEST_RRII_LD_D( 7, ldo, 0xff00ff00ff00ff00, tdat4, -16 )
	TEST_RRII_LD_D( 8, ldo, 0x0ff00ff00ff00ff0, tdat4, -8 )
	TEST_RRII_LD_D( 9, ldo, 0xf00ff00ff00ff00f, tdat4, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat:
tdat1:	.dd.o64	0x00ff00ff00ff00ff
tdat2:	.dd.o64	0xff00ff00ff00ff00
tdat3:	.dd.o64	0x0ff00ff00ff00ff0
tdat4:	.dd.o64	0xf00ff00ff00ff00f

DDTEST_DATA_END
