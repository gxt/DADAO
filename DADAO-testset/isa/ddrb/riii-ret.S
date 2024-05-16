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

	__TEST_CASE_HEAD__( 111 )

		call	lbl_111_high
		brz		rd0, lbl_111_pass

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_111_high:
		ret		RD_RET1, 111

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_111_pass:
		setrd	RD_EXP1, 111
		brne	RD_RET1, RD_EXP1, ___fail

	__TEST_CASE_HEAD__( 112 )

		call	lbl_112_high
		brz		rd0, lbl_112_pass

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_112_high:
		ret		RD_RET1, -112

		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail
		brz		rd0, ___fail

	lbl_112_pass:
		setrd	RD_EXP1, -112
		brne	RD_RET1, RD_EXP1, ___fail

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
 
