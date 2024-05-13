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

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

