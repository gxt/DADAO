#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	__TEST_CASE_HEAD__( 101 )

		call	lbl_101_high
		brz		rd0, lbl_101_pass

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_101_high:
		ret		rd0, 0

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_101_pass:
		swym	0

	__TEST_CASE_HEAD__( 102 )

		brz		rd0, lbl_102_test

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_102_low:
		ret		rd0, 0

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_102_test:
		call	lbl_102_low
		brz		rd0, lbl_102_pass

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_102_pass:
		swym	0

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
 
