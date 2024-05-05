#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftsgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 112, ftsgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_123( 113, ftsgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 114, ftsgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_121( 121, ftsgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 122, ftsgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_121( 123, ftsgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 124, ftsgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_112( 131, ftsgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 132, ftsgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_112( 133, ftsgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 134, ftsgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_120( 141, ftsgnj, fflags_Z,  3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FT_120( 142, ftsgnj, fflags_Z,  3.14, -3.14,   0.0 )

	TEST_FRRR_FFF_FO_123( 211, fosgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 212, fosgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_123( 213, fosgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 214, fosgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_121( 221, fosgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 222, fosgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_121( 223, fosgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 224, fosgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_112( 231, fosgnj, fflags_Z,  3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 232, fosgnj, fflags_Z, -3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_112( 233, fosgnj, fflags_Z,  3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 234, fosgnj, fflags_Z, -3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_120( 241, fosgnj, fflags_Z,  3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FO_120( 242, fosgnj, fflags_Z,  3.14, -3.14,   0.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
