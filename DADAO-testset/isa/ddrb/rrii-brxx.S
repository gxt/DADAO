#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_BR_H_12( 111, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_H_12( 112, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_H_12( 113, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_H_12( 121, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_H_12( 122, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_H_12( 123, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_H_12( 124, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_H_12( 125, breq,  1, -1, _fail, _pass )
	TEST_RRII_BR_H_12( 126, breq, -1,  1, _fail, _pass )

	TEST_RRII_BR_H_11( 131, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_H_11( 132, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_H_10( 141, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_H_10( 142, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_H_10( 143, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_H_01( 144, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_H_01( 145, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_H_01( 146, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_H_00( 147, breq,  0,  0, _pass, _fail )

	TEST_RRII_BR_L_12( 151, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_L_12( 152, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_L_12( 153, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_L_12( 161, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_L_12( 162, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_L_12( 163, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_L_12( 164, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_L_12( 165, breq,  1, -1, _fail, _pass )
	TEST_RRII_BR_L_12( 166, breq, -1,  1, _fail, _pass )

	TEST_RRII_BR_L_11( 171, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_L_11( 172, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_L_10( 181, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_L_10( 182, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_L_10( 183, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_L_01( 184, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_L_01( 185, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_L_01( 186, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_L_00( 187, breq,  0,  0, _pass, _fail )

	TEST_RRII_BR_H_12( 211, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_12( 212, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_H_12( 213, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_H_12( 221, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_H_12( 222, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_H_12( 223, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_H_12( 224, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_H_12( 225, brne,  1, -1, _pass, _fail )
	TEST_RRII_BR_H_12( 226, brne, -1,  1, _pass, _fail )

	TEST_RRII_BR_H_11( 231, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_H_11( 232, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_H_10( 241, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_10( 242, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_H_10( 243, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_H_01( 244, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_01( 245, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_H_01( 246, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_H_00( 247, brne,  0,  0, _fail, _pass )

	TEST_RRII_BR_L_12( 251, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_L_12( 252, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_L_12( 253, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_L_12( 261, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_L_12( 262, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_L_12( 263, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_L_12( 264, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_L_12( 265, brne,  1, -1, _pass, _fail )
	TEST_RRII_BR_L_12( 266, brne, -1,  1, _pass, _fail )

	TEST_RRII_BR_L_11( 271, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_L_11( 272, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_L_10( 281, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_L_10( 282, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_L_10( 283, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_L_01( 284, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_L_01( 285, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_L_01( 286, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_L_00( 287, brne,  0,  0, _fail, _pass )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

