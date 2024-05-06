#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_RRRR_WRRR_FT_1234( 111, ftmadd, fflags_Z,    3.5,  1.0,     2.5,  1.0 )
	TEST_RRRR_WRRR_FT_1234( 112, ftmadd, fflag_NX, 1236.2, -1.0, -1235.1,  1.1 )
	TEST_RRRR_WRRR_FT_1234( 113, ftmadd, fflags_Z,  -12.0,  2.0,    -5.0, -2.0 )

	TEST_RRRR_WRRR_FO_1234( 211, fomadd, fflags_Z,                3.5,  1.0,     2.5,  1.0 )
	TEST_RRRR_WRRR_FO_1234( 212, fomadd, fflag_NX, 1236.1999999999999, -1.0, -1235.1,  1.1 )
	TEST_RRRR_WRRR_FO_1234( 213, fomadd, fflags_Z,              -12.0,  2.0,    -5.0, -2.0 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
