#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_BR_H_12( 11, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_12( 12, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_H_12( 13, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_H_12( 21, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_H_12( 22, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_H_12( 23, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_H_12( 24, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_H_12( 25, brne,  1, -1, _pass, _fail )
	TEST_RRII_BR_H_12( 26, brne, -1,  1, _pass, _fail )

	TEST_RRII_BR_H_11( 31, brne,  1,  1, _fail, _pass )
	TEST_RRII_BR_H_11( 32, brne, -1, -1, _fail, _pass )

	TEST_RRII_BR_H_10( 41, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_10( 42, brne,  1,  0, _pass, _fail )
	TEST_RRII_BR_H_10( 43, brne, -1,  0, _pass, _fail )
	TEST_RRII_BR_H_01( 44, brne,  0,  0, _fail, _pass )
	TEST_RRII_BR_H_01( 45, brne,  0,  1, _pass, _fail )
	TEST_RRII_BR_H_01( 46, brne,  0, -1, _pass, _fail )
	TEST_RRII_BR_H_00( 47, brne,  0,  0, _fail, _pass )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

