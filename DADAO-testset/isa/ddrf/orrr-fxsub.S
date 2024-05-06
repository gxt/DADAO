#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_FFF_FT_123( 111, ftsub, fflags_Z,        1.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FT_123( 112, ftsub, fflag_NX,      -1234,    -1235.1,       -1.1 )
	TEST_FRRR_FFF_FT_123( 113, ftsub, fflag_NX, 3.14159265, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FT_112( 121, ftsub, fflags_Z, 1.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_121( 122, ftsub, fflags_Z, 1.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FT_111( 123, ftsub, fflags_Z, 0.0, 1.0, 1.0 )

	TEST_FRRR_FFF_FT_123( 131, ftsub, fflag_NV, ft_qNaN, Inf, Inf )

	TEST_FRRR_FFF_FO_123( 211, fosub, fflags_Z,        1.5,        2.5,        1.0 )
	TEST_FRRR_FFF_FO_123( 212, fosub, fflag_NX,      -1234,    -1235.1,       -1.1 )
	TEST_FRRR_FFF_FO_123( 213, fosub, fflag_NX, 3.1415926400000001, 3.14159265, 0.00000001 )

	TEST_FRRR_FFF_FO_112( 221, fosub, fflags_Z, 1.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_121( 222, fosub, fflags_Z, 1.5, 2.5, 1.0 )
	TEST_FRRR_FFF_FO_111( 223, fosub, fflags_Z, 0.0, 1.0, 1.0 )

	TEST_FRRR_FFF_FO_123( 231, fosub, fflag_NV, fo_qNaN, Inf, Inf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
