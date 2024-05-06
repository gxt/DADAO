#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_DFF_FT_123( 111, ftscmp, fflags_Z,  1,   2.5,   1.0 )
	TEST_FRRR_DFF_FT_123( 112, ftscmp, fflags_Z,  0,   2.5,   2.5 )
	TEST_FRRR_DFF_FT_123( 113, ftscmp, fflags_Z, -1,   1.0,   2.5 )
	TEST_FRRR_DFF_FT_123( 114, ftscmp, fflags_Z,  1, -1.36, -1.37 )
	TEST_FRRR_DFF_FT_123( 115, ftscmp, fflags_Z,  0, -1.36, -1.36 )
	TEST_FRRR_DFF_FT_123( 116, ftscmp, fflags_Z, -1, -1.37, -1.36 )

	TEST_FRRR_DFF_FT_123( 121, ftscmp, fflag_NV, it_sNaN, ft_qNaN, 0 )
	TEST_FRRR_DFF_FT_123( 122, ftscmp, fflag_NV, it_sNaN, ft_qNaN, ft_qNaN )
	TEST_FRRR_DFF_FT_123( 123, ftscmp, fflag_NV, it_sNaN, ft_sNaN, 0 )
	TEST_FRRR_DFF_FT_123( 124, ftscmp, fflag_NV, it_sNaN, ft_sNaN, ft_sNaN )

	TEST_FRRR_DFF_FO_123( 211, foscmp, fflags_Z,  1,   2.5,   1.0 )
	TEST_FRRR_DFF_FO_123( 212, foscmp, fflags_Z,  0,   2.5,   2.5 )
	TEST_FRRR_DFF_FO_123( 213, foscmp, fflags_Z, -1,   1.0,   2.5 )
	TEST_FRRR_DFF_FO_123( 214, foscmp, fflags_Z,  1, -1.36, -1.37 )
	TEST_FRRR_DFF_FO_123( 215, foscmp, fflags_Z,  0, -1.36, -1.36 )
	TEST_FRRR_DFF_FO_123( 216, foscmp, fflags_Z, -1, -1.37, -1.36 )

	TEST_FRRR_DFF_FO_123( 221, foscmp, fflag_NV, io_sNaN, fo_qNaN, 0 )
	TEST_FRRR_DFF_FO_123( 222, foscmp, fflag_NV, io_sNaN, fo_qNaN, fo_qNaN )
	TEST_FRRR_DFF_FO_123( 223, foscmp, fflag_NV, io_sNaN, fo_sNaN, 0 )
	TEST_FRRR_DFF_FO_123( 224, foscmp, fflag_NV, io_sNaN, fo_sNaN, fo_sNaN )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
