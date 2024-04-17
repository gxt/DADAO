#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_DFF_FT_123( 11, ftcne, 0,  1, 2.5, 1.0 )
	TEST_FRRR_DFF_FT_123( 12, ftcne, 0,  0, 2.5, 2.5 )
	TEST_FRRR_DFF_FT_123( 13, ftcne, 0, -1, sNaNf, 0 )

	TEST_FRRR_DFF_FT_122( 21, ftcne, 0,  0, 2.5, 2.5 )
	TEST_FRRR_DFF_FT_122( 22, ftcne, 0, -1, sNaNf, sNaNf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
