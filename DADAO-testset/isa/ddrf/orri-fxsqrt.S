#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 111, ftsqrt, fflags_Z, ldft, .single,                100, ldft, .single,      10000, rf, rf )
	TEST_FRRI_R1_12( 112, ftsqrt, fflag_NX, ldft, .single, 1.7724538498928541, ldft, .single, 3.14159265, rf, rf )
	TEST_FRRI_R1_12( 113, ftsqrt, fflag_NX, ldft, .single,          13.076696, ldft, .single,      171.0, rf, rf )

	TEST_FRRI_R1_12( 121, ftsqrt, fflags_Z, ldft, .single,     0.0, ldft, .single,     0.0, rf, rf )
	TEST_FRRI_R1_12( 122, ftsqrt, fflags_Z, ldft, .single,    -0.0, ldft, .single,    -0.0, rf, rf )
	TEST_FRRI_R1_12( 123, ftsqrt, fflags_Z, ldft, .single,     Inf, ldft, .single,     Inf, rf, rf )
	TEST_FRRI_R1_12( 124, ftsqrt, fflag_NV, ldft, .single, ft_qNaN, ldft, .single,    -1.0, rf, rf )

	TEST_FRRI_R1_12( 211, fosqrt, fflags_Z, ldfo, .double,                                100, ldfo, .double,      10000, rf, rf )
	TEST_FRRI_R1_12( 212, fosqrt, fflag_NX, ldfo, .double,                 1.7724538498928541, ldfo, .double, 3.14159265, rf, rf )
	TEST_FRRI_R1_12( 213, fosqrt, fflag_NX, ldfo, .double,                 13.076696830622021, ldfo, .double,      171.0, rf, rf )
	TEST_FRRI_R1_12( 214, fosqrt, fflag_NX, ldfo, .double, 0.00040099251863345283320230749702, ldfo, .double, 1.60795e-7, rf, rf )

	TEST_FRRI_R1_12( 221, fosqrt, fflags_Z, ldfo, .double,     0.0, ldfo, .double,     0.0, rf, rf )
	TEST_FRRI_R1_12( 222, fosqrt, fflags_Z, ldfo, .double,    -0.0, ldfo, .double,    -0.0, rf, rf )
	TEST_FRRI_R1_12( 223, fosqrt, fflags_Z, ldfo, .double,     Inf, ldfo, .double,     Inf, rf, rf )
	TEST_FRRI_R1_12( 224, fosqrt, fflag_NV, ldfo, .double, fo_qNaN, ldfo, .double,    -1.0, rf, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
