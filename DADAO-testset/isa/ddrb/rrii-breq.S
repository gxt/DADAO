#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRII_BR_12( 11, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_12( 12, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_12( 13, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_12( 21, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_12( 22, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_12( 23, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_12( 24, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_12( 25, breq,  1, -1, _fail, _pass )
	TEST_RRII_BR_12( 26, breq, -1,  1, _fail, _pass )

	TEST_RRII_BR_11( 31, breq,  1,  1, _pass, _fail )
	TEST_RRII_BR_11( 32, breq, -1, -1, _pass, _fail )

	TEST_RRII_BR_10( 41, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_10( 42, breq,  1,  0, _fail, _pass )
	TEST_RRII_BR_10( 43, breq, -1,  0, _fail, _pass )
	TEST_RRII_BR_01( 44, breq,  0,  0, _pass, _fail )
	TEST_RRII_BR_01( 45, breq,  0,  1, _fail, _pass )
	TEST_RRII_BR_01( 46, breq,  0, -1, _fail, _pass )
	TEST_RRII_BR_00( 47, breq,  0,  0, _pass, _fail )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

