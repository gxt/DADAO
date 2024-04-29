#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 11, ftadd, 0, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_123( 12, ftadd, fflag_NX, -1234, -1235.1, 1.1 )
	TEST_FRRR_FFF_FT_123( 13, ftadd, fflag_NX, 3.14159265, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FT_112( 21, ftadd, 0, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_121( 22, ftadd, 0, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_111( 23, ftadd, 0, 2.0, 1.0, 1.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
