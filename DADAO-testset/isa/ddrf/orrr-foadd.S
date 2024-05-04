#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FO_123( 11, foadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_123( 12, foadd, fflag_NX, -1234, -1235.1, 1.1 )
	TEST_FRRR_FFF_FO_123( 13, foadd, fflag_NX, 3.14159266, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FO_112( 21, foadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_121( 22, foadd, fflags_Z, 3.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_111( 23, foadd, fflags_Z, 2.0, 1.0, 1.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
