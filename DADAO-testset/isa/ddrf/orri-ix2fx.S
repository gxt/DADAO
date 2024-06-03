#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_M1_12( 111, it2ft, fflags_Z, ldt, .single,  2.0, ldts, .dd.t32,  2, rf, rd )
	TEST_FRRI_M1_12( 112, it2ft, fflags_Z, ldt, .single, -2.0, ldts, .dd.t32, -2, rf, rd )
	TEST_FRRI_M1_12( 113, it2ft, fflags_Z, ldt, .single,  0.0, ldts, .dd.t32,  0, rf, rd )

	TEST_FRRI_M1_12( 211, io2ft, fflags_Z, ldt, .single,  2.0,  ldo, .dd.o64,  2, rf, rd )
	TEST_FRRI_M1_12( 212, io2ft, fflags_Z, ldt, .single, -2.0,  ldo, .dd.o64, -2, rf, rd )
	TEST_FRRI_M1_10( 213, io2ft, fflags_Z, ldt, .single,  0.0,  ldo, .dd.o64,  0, rf, rd )

	TEST_FRRI_M1_12( 311, it2fo, fflags_Z, ldo, .double,  2.0, ldts, .dd.t32,  2, rf, rd )
	TEST_FRRI_M1_12( 312, it2fo, fflags_Z, ldo, .double, -2.0, ldts, .dd.t32, -2, rf, rd )
	TEST_FRRI_M1_12( 313, it2fo, fflags_Z, ldo, .double,  0.0, ldts, .dd.t32,  0, rf, rd )

	TEST_FRRI_M1_12( 411, io2fo, fflags_Z, ldo, .double,  2.0,  ldo, .dd.o64,  2, rf, rd )
	TEST_FRRI_M1_12( 412, io2fo, fflags_Z, ldo, .double, -2.0,  ldo, .dd.o64, -2, rf, rd )
	TEST_FRRI_M1_10( 413, io2fo, fflags_Z, ldo, .double,  0.0,  ldo, .dd.o64,  0, rf, rd )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
