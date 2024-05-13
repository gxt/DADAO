#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RIII_BR_H( 111, brn,  -1, _pass, _fail )
	TEST_RIII_BR_H( 112, brn,   0, _fail, _pass )
	TEST_RIII_BR_H( 113, brn,   1, _fail, _pass )

	TEST_RIII_BR_L( 121, brn,  -1, _pass, _fail )
	TEST_RIII_BR_L( 122, brn,   0, _fail, _pass )
	TEST_RIII_BR_L( 123, brn,   1, _fail, _pass )

	TEST_RIII_BR_H( 211, brnn, -1, _fail, _pass )
	TEST_RIII_BR_H( 212, brnn,  0, _pass, _fail )
	TEST_RIII_BR_H( 213, brnn,  1, _pass, _fail )

	TEST_RIII_BR_L( 221, brnn, -1, _fail, _pass )
	TEST_RIII_BR_L( 222, brnn,  0, _pass, _fail )
	TEST_RIII_BR_L( 223, brnn,  1, _pass, _fail )

	TEST_RIII_BR_H( 311, brz,  -1, _fail, _pass )
	TEST_RIII_BR_H( 312, brz,   0, _pass, _fail )
	TEST_RIII_BR_H( 313, brz,   1, _fail, _pass )

	TEST_RIII_BR_L( 321, brz,  -1, _fail, _pass )
	TEST_RIII_BR_L( 322, brz,   0, _pass, _fail )
	TEST_RIII_BR_L( 323, brz,   1, _fail, _pass )

	TEST_RIII_BR_H( 411, brnz, -1, _pass, _fail )
	TEST_RIII_BR_H( 412, brnz,  0, _fail, _pass )
	TEST_RIII_BR_H( 413, brnz,  1, _pass, _fail )

	TEST_RIII_BR_L( 421, brnz, -1, _pass, _fail )
	TEST_RIII_BR_L( 422, brnz,  0, _fail, _pass )
	TEST_RIII_BR_L( 423, brnz,  1, _pass, _fail )

	TEST_RIII_BR_H( 511, brp,  -1, _fail, _pass )
	TEST_RIII_BR_H( 512, brp,   0, _fail, _pass )
	TEST_RIII_BR_H( 513, brp,   1, _pass, _fail )

	TEST_RIII_BR_L( 521, brp,  -1, _fail, _pass )
	TEST_RIII_BR_L( 522, brp,   0, _fail, _pass )
	TEST_RIII_BR_L( 523, brp,   1, _pass, _fail )

	TEST_RIII_BR_H( 611, brnp, -1, _pass, _fail )
	TEST_RIII_BR_H( 612, brnp,  0, _pass, _fail )
	TEST_RIII_BR_H( 613, brnp,  1, _fail, _pass )

	TEST_RIII_BR_L( 621, brnp, -1, _pass, _fail )
	TEST_RIII_BR_L( 622, brnp,  0, _pass, _fail )
	TEST_RIII_BR_L( 623, brnp,  1, _fail, _pass )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END

