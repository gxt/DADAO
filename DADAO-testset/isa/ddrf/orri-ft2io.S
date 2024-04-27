#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 2, ft2io, 0, ldo, .dd.o64,  1, ldft, .single,  1.0, rd, rf )
	TEST_FRRI_R1_12( 3, ft2io, 0, ldo, .dd.o64, -1, ldft, .single, -1.0, rd, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
