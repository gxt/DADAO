#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 2, rd2fo, 0, ldfo, .double,  1.0, ldo, .dd.o64,  1, rf, rd )
	TEST_FRRI_R1_12( 3, rd2fo, 0, ldfo, .double, -1.0, ldo, .dd.o64, -1, rf, rd )
	TEST_FRRI_R1_10( 4, rd2fo, 0, ldfo, .double,  0.0, ldo, .dd.o64,  0, rf, rd )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
