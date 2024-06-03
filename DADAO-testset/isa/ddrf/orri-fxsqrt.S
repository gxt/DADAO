#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_M1_12( 111, ftsqrt, fflags_Z, ldt, .single,                100, ldt, .single,      10000, rf, rf )
	TEST_FRRI_M1_12( 112, ftsqrt, fflag_NX, ldt, .single, 1.7724538498928541, ldt, .single, 3.14159265, rf, rf )
	TEST_FRRI_M1_12( 113, ftsqrt, fflag_NX, ldt, .single,          13.076696, ldt, .single,      171.0, rf, rf )

	TEST_FRRI_M1_12( 121, ftsqrt, fflags_Z, ldt, .single,     0.0, ldt, .single,     0.0, rf, rf )
	TEST_FRRI_M1_12( 122, ftsqrt, fflags_Z, ldt, .single,    -0.0, ldt, .single,    -0.0, rf, rf )
	TEST_FRRI_M1_12( 123, ftsqrt, fflags_Z, ldt, .single,     Inf, ldt, .single,     Inf, rf, rf )
	TEST_FRRI_M1_12( 124, ftsqrt, fflag_NV, ldt, .single, ft_qNaN, ldt, .single,    -1.0, rf, rf )

	TEST_FRRI_M1_12( 211, fosqrt, fflags_Z, ldo, .double,                                100, ldo, .double,      10000, rf, rf )
	TEST_FRRI_M1_12( 212, fosqrt, fflag_NX, ldo, .double,                 1.7724538498928541, ldo, .double, 3.14159265, rf, rf )
	TEST_FRRI_M1_12( 213, fosqrt, fflag_NX, ldo, .double,                 13.076696830622021, ldo, .double,      171.0, rf, rf )
	TEST_FRRI_M1_12( 214, fosqrt, fflag_NX, ldo, .double, 0.00040099251863345283320230749702, ldo, .double, 1.60795e-7, rf, rf )

	TEST_FRRI_M1_12( 221, fosqrt, fflags_Z, ldo, .double,     0.0, ldo, .double,     0.0, rf, rf )
	TEST_FRRI_M1_12( 222, fosqrt, fflags_Z, ldo, .double,    -0.0, ldo, .double,    -0.0, rf, rf )
	TEST_FRRI_M1_12( 223, fosqrt, fflags_Z, ldo, .double,     Inf, ldo, .double,     Inf, rf, rf )
	TEST_FRRI_M1_12( 224, fosqrt, fflag_NV, ldo, .double, fo_qNaN, ldo, .double,    -1.0, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
