#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 111, fo2ft, fflags_Z, ldft, .single,  1.5, ldfo, .double,  1.5, rf, rf )
	TEST_FRRI_R1_12( 112, fo2ft, fflags_Z, ldft, .single, -1.5, ldfo, .double, -1.5, rf, rf )
	TEST_FRRI_R1_12( 113, fo2ft, fflags_Z, ldft, .single,  0.0, ldfo, .double,  0.0, rf, rf )
	TEST_FRRI_R1_12( 114, fo2ft, fflags_Z, ldft, .single, -0.0, ldfo, .double, -0.0, rf, rf )

	TEST_FRRI_R1_12( 211, ft2fo, fflags_Z, ldfo, .double,  1.5, ldft, .single,  1.5, rf, rf )
	TEST_FRRI_R1_12( 212, ft2fo, fflags_Z, ldfo, .double, -1.5, ldft, .single, -1.5, rf, rf )
	TEST_FRRI_R1_12( 213, ft2fo, fflags_Z, ldfo, .double,  0.0, ldft, .single,  0.0, rf, rf )
	TEST_FRRI_R1_12( 214, ft2fo, fflags_Z, ldfo, .double, -0.0, ldft, .single, -0.0, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
