#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_WRRR_FT_1234( 11, ftmadd, fflags_Z, 3.5, 1.0, 2.5, 1.0 )
	TEST_RRRR_WRRR_FT_1234( 12, ftmadd, fflag_NX, 1236.2, -1.0, -1235.1, 1.1 )
	TEST_RRRR_WRRR_FT_1234( 13, ftmadd, fflags_Z, -12.0, 2.0, -5.0, -2.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
