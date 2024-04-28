#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRR_DFF_FT_123( 11, ftqcmp, 0,  1, 2.5, 1.0 )
	TEST_FRRR_DFF_FT_123( 12, ftqcmp, 0,  0, 2.5, 2.5 )
	TEST_FRRR_DFF_FT_123( 13, ftqcmp, 0, -1, 1.0, 2.5 )
//	TEST_FRRR_DFF_FT_123( 14, ftqcmp, 0, ft_qNaN, ft_qNaN, 0 )

	TEST_FRRR_DFF_FT_122( 21, ftqcmp, 0,  0, 2.5, 2.5 )
//	TEST_FRRR_DFF_FT_123( 22, ftqcmp, 0, ft_qNaN, ft_qNaN, ft_qNaN )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
