#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftmul, fflags_Z,           2.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FT_123( 112, ftmul, fflag_NX,       1358.61,    -1235.1,       -1.1 )
	TEST_FRRR_FFF_FT_123( 113, ftmul, fflag_NX, 3.14159265e-8, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FT_112( 121, ftmul, fflags_Z, 2.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_121( 122, ftmul, fflags_Z, 2.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_111( 123, ftmul, fflags_Z, 1.0, 1.0, 1.0 )

	TEST_FRRR_FFF_FO_123( 211, fomul, fflags_Z,           2.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FO_123( 212, fomul, fflag_NX,       1358.61,    -1235.1,       -1.1 )
	TEST_FRRR_FFF_FO_123( 213, fomul, fflag_NX, 3.14159265e-8, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FO_112( 221, fomul, fflags_Z, 2.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_121( 222, fomul, fflags_Z, 2.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_111( 223, fomul, fflags_Z, 1.0, 1.0, 1.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
