#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_DFF_FT_123( 111, ftqcmp, fflags_Z,  1,   2.5,   1.0 )
	TEST_FRRR_DFF_FT_123( 112, ftqcmp, fflags_Z,  0,   2.5,   2.5 )
	TEST_FRRR_DFF_FT_123( 113, ftqcmp, fflags_Z, -1,   1.0,   2.5 )
	TEST_FRRR_DFF_FT_123( 114, ftqcmp, fflags_Z,  1, -1.36, -1.37 )
	TEST_FRRR_DFF_FT_123( 115, ftqcmp, fflags_Z,  0, -1.36, -1.36 )
	TEST_FRRR_DFF_FT_123( 116, ftqcmp, fflags_Z, -1, -1.37, -1.36 )

	TEST_FRRR_DFF_FT_123( 121, ftqcmp, fflags_Z, it_qNaN, ft_qNaN, 0 )
	TEST_FRRR_DFF_FT_123( 122, ftqcmp, fflags_Z, it_qNaN, ft_qNaN, ft_qNaN )
	TEST_FRRR_DFF_FT_123( 123, ftqcmp, fflags_Z, it_qNaN, ft_sNaN, 0 )
	TEST_FRRR_DFF_FT_123( 124, ftqcmp, fflags_Z, it_qNaN, ft_sNaN, ft_sNaN )

	TEST_FRRR_DFF_FO_123( 211, foqcmp, fflags_Z,  1,   2.5,   1.0 )
	TEST_FRRR_DFF_FO_123( 212, foqcmp, fflags_Z,  0,   2.5,   2.5 )
	TEST_FRRR_DFF_FO_123( 213, foqcmp, fflags_Z, -1,   1.0,   2.5 )
	TEST_FRRR_DFF_FO_123( 214, foqcmp, fflags_Z,  1, -1.36, -1.37 )
	TEST_FRRR_DFF_FO_123( 215, foqcmp, fflags_Z,  0, -1.36, -1.36 )
	TEST_FRRR_DFF_FO_123( 216, foqcmp, fflags_Z, -1, -1.37, -1.36 )

	TEST_FRRR_DFF_FO_123( 221, foqcmp, fflags_Z, io_qNaN, fo_qNaN, 0 )
	TEST_FRRR_DFF_FO_123( 222, foqcmp, fflags_Z, io_qNaN, fo_qNaN, fo_qNaN )
	TEST_FRRR_DFF_FO_123( 223, foqcmp, fflags_Z, io_qNaN, fo_sNaN, 0 )
	TEST_FRRR_DFF_FO_123( 224, foqcmp, fflags_Z, io_qNaN, fo_sNaN, fo_sNaN )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
