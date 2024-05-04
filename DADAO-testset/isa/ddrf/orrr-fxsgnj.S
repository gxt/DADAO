#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftsgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 112, ftsgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_123( 113, ftsgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 114, ftsgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_123( 211, fosgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 212, fosgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_123( 213, fosgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 214, fosgnj, fflags_Z, -3.14, -3.14, -6.25 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
