#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftsgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 112, ftsgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_123( 113, ftsgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_123( 114, ftsgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_121( 121, ftsgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 122, ftsgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_121( 123, ftsgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_121( 124, ftsgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_112( 131, ftsgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 132, ftsgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FT_112( 133, ftsgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FT_112( 134, ftsgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FT_120( 141, ftsgnn, fflags_Z, -3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FT_120( 142, ftsgnn, fflags_Z, -3.14, -3.14,   0.0 )

	TEST_FRRR_FFF_FO_123( 211, fosgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 212, fosgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_123( 213, fosgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_123( 214, fosgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_121( 221, fosgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 222, fosgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_121( 223, fosgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_121( 224, fosgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_112( 231, fosgnn, fflags_Z, -3.14,  3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 232, fosgnn, fflags_Z,  3.14,  3.14, -6.25 )
	TEST_FRRR_FFF_FO_112( 233, fosgnn, fflags_Z, -3.14, -3.14,  6.25 )
	TEST_FRRR_FFF_FO_112( 234, fosgnn, fflags_Z,  3.14, -3.14, -6.25 )

	TEST_FRRR_FFF_FO_120( 241, fosgnn, fflags_Z, -3.14,  3.14,   0.0 )
	TEST_FRRR_FFF_FO_120( 242, fosgnn, fflags_Z, -3.14, -3.14,   0.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
