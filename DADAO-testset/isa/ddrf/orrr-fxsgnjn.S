#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftsgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 112, ftsgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_123( 113, ftsgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 114, ftsgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_121( 121, ftsgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 122, ftsgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_121( 123, ftsgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 124, ftsgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_112( 131, ftsgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 132, ftsgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_112( 133, ftsgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 134, ftsgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_120( 141, ftsgnjn, fflags_Z, -3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FT_120( 142, ftsgnjn, fflags_Z, -3.14, -3.14,   0.0 )

	TEST_FRRR_FFF_FO_123( 211, fosgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 212, fosgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_123( 213, fosgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 214, fosgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_121( 221, fosgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 222, fosgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_121( 223, fosgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 224, fosgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_112( 231, fosgnjn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 232, fosgnjn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_112( 233, fosgnjn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 234, fosgnjn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_120( 241, fosgnjn, fflags_Z, -3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FO_120( 242, fosgnjn, fflags_Z, -3.14, -3.14,   0.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
