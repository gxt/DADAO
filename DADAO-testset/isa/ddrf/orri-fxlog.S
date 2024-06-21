#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_B2_12( 111, ftlog, fflags_Z, ldt, .single, 4.0, ldt, .single, 16.0, rf, rf )

	TEST_FRRI_B2_12( 121, ftlog, fflags_Z, ldt, .single, 4.0, ldt, .single, 16.0, rf, rf )

	TEST_FRRI_B2_12( 211, folog, fflags_Z, ldo, .double, 4.0, ldo, .double, 16.0, rf, rf )

	TEST_FRRI_B2_12( 221, folog, fflags_Z, ldo, .double, 4.0, ldo, .double, 16.0, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
