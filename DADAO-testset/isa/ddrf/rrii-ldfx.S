#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_LD_F_12( 111, ldft, 0x00ff00ff, tdat, 0 )
	TEST_RRII_LD_F_12( 112, ldft, 0xff00ff00, tdat, 8 )
	TEST_RRII_LD_F_12( 113, ldft, 0x0ff00ff0, tdat, 16 )
	TEST_RRII_LD_F_12( 114, ldft, 0xf00ff00f, tdat, 24 )

	TEST_RRII_LD_F_12( 121, ldft, 0x00ff00ff, tdat4, -24 )
	TEST_RRII_LD_F_12( 122, ldft, 0xff00ff00, tdat4, -16 )
	TEST_RRII_LD_F_12( 123, ldft, 0x0ff00ff0, tdat4, -8 )
	TEST_RRII_LD_F_12( 124, ldft, 0xf00ff00f, tdat4, 0 )

	TEST_RRII_LD_F_12( 211, ldfo, 0x00ff00ff00ff00ff, tdat, 0 )
	TEST_RRII_LD_F_12( 212, ldfo, 0xff00ff00ff00ff00, tdat, 8 )
	TEST_RRII_LD_F_12( 213, ldfo, 0x0ff00ff00ff00ff0, tdat, 16 )
	TEST_RRII_LD_F_12( 214, ldfo, 0xf00ff00ff00ff00f, tdat, 24 )

	TEST_RRII_LD_F_12( 221, ldfo, 0x00ff00ff00ff00ff, tdat4, -24 )
	TEST_RRII_LD_F_12( 222, ldfo, 0xff00ff00ff00ff00, tdat4, -16 )
	TEST_RRII_LD_F_12( 223, ldfo, 0x0ff00ff00ff00ff0, tdat4, -8 )
	TEST_RRII_LD_F_12( 224, ldfo, 0xf00ff00ff00ff00f, tdat4, 0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN

tdat:
tdat1:	.dd.o64	0x00ff00ff00ff00ff
tdat2:	.dd.o64	0xff00ff00ff00ff00
tdat3:	.dd.o64	0x0ff00ff00ff00ff0
tdat4:	.dd.o64	0xf00ff00ff00ff00f

DDTEST_DATA_END
