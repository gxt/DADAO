#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftadd, fflags_Z,        3.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FT_123( 112, ftadd, fflag_NX,      -1234,    -1235.1,        1.1 )
	TEST_FRRR_FFF_FT_123( 113, ftadd, fflag_NX, 3.14159265, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FT_112( 121, ftadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_121( 122, ftadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_111( 123, ftadd, fflags_Z, 2.0, 1.0, 1.0 )

	TEST_FRRR_FFF_FO_123( 211, foadd, fflags_Z,        3.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FO_123( 212, foadd, fflag_NX,      -1234,    -1235.1,        1.1 )
	TEST_FRRR_FFF_FO_123( 213, foadd, fflag_NX, 3.14159266, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FO_112( 221, foadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_121( 222, foadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_111( 223, foadd, fflags_Z, 2.0, 1.0, 1.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
