#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftrem, fflags_Z, -0.5, 3.5, 1.0 )

	TEST_FRRR_FFF_FT_112( 121, ftrem, fflags_Z, -0.5, 3.5, 1.0 )

	TEST_FRRR_FFF_FO_123( 211, forem, fflags_Z, -0.5, 3.5, 1.0 )

	TEST_FRRR_FFF_FO_112( 221, forem, fflags_Z, -0.5, 3.5, 1.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
