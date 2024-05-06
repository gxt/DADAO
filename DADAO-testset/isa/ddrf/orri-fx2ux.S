#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	__TEST_FCSR_SET_RMODE( __DDAEE_FCSR_RDMODE_RTZ__ )

	TEST_FRRI_R1_12( 111, ft2ut, fflag_NV, ldtu, .dd.t32,      0, ldft, .single, -3.0, rd, rf )
	TEST_FRRI_R1_12( 112, ft2ut, fflag_NV, ldtu, .dd.t32,      0, ldft, .single, -1.0, rd, rf )
	TEST_FRRI_R1_12( 113, ft2ut, fflag_NX, ldtu, .dd.t32,      0, ldft, .single, -0.9, rd, rf )
	TEST_FRRI_R1_12( 114, ft2ut, fflag_NX, ldtu, .dd.t32,      0, ldft, .single,  0.9, rd, rf )
	TEST_FRRI_R1_12( 115, ft2ut, fflags_Z, ldtu, .dd.t32,      1, ldft, .single,  1.0, rd, rf )
	TEST_FRRI_R1_12( 116, ft2ut, fflag_NX, ldtu, .dd.t32,      1, ldft, .single,  1.1, rd, rf )

	TEST_FRRI_R1_12( 121, ft2ut, fflag_NV, ldtu, .dd.t32,          0, ldft, .single, -3e9, rd, rf )
	TEST_FRRI_R1_12( 122, ft2ut, fflags_Z, ldtu, .dd.t32, 3000000000, ldft, .single,  3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_R1_12( 131, ft2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldft, .single,           Inf, rd, rf )
	TEST_FRRI_R1_12( 132, ft2ut, fflag_NV, ldtu, .dd.t32,      0, ldft, .single,          -Inf, rd, rf )
	TEST_FRRI_R1_12( 133, ft2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldft, .single,       ft_qNaN, rd, rf )
	TEST_FRRI_R1_12( 134, ft2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldft, .single, ft_minus_qNaN, rd, rf )

	TEST_FRRI_R1_12( 211, ft2uo, fflag_NV,  ldo, .dd.o64,      0, ldft, .single, -3.0, rd, rf )
	TEST_FRRI_R1_12( 212, ft2uo, fflag_NV,  ldo, .dd.o64,      0, ldft, .single, -1.0, rd, rf )
	TEST_FRRI_R1_12( 213, ft2uo, fflag_NX,  ldo, .dd.o64,      0, ldft, .single, -0.9, rd, rf )
	TEST_FRRI_R1_12( 214, ft2uo, fflag_NX,  ldo, .dd.o64,      0, ldft, .single,  0.9, rd, rf )
	TEST_FRRI_R1_12( 215, ft2uo, fflags_Z,  ldo, .dd.o64,      1, ldft, .single,  1.0, rd, rf )
	TEST_FRRI_R1_12( 216, ft2uo, fflag_NX,  ldo, .dd.o64,      1, ldft, .single,  1.1, rd, rf )

	TEST_FRRI_R1_12( 221, ft2uo, fflag_NV,  ldo, .dd.o64,      0, ldft, .single, -3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_R1_12( 231, ft2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldft, .single,           Inf, rd, rf )
	TEST_FRRI_R1_12( 232, ft2uo, fflag_NV,  ldo, .dd.o64,      0, ldft, .single,          -Inf, rd, rf )
	TEST_FRRI_R1_12( 233, ft2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldft, .single,       ft_qNaN, rd, rf )
	TEST_FRRI_R1_12( 234, ft2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldft, .single, ft_minus_qNaN, rd, rf )

	TEST_FRRI_R1_12( 311, fo2ut, fflag_NV, ldtu, .dd.t32,      0, ldfo, .double, -3.0, rd, rf )
	TEST_FRRI_R1_12( 312, fo2ut, fflag_NV, ldtu, .dd.t32,      0, ldfo, .double, -1.0, rd, rf )
	TEST_FRRI_R1_12( 313, fo2ut, fflag_NX, ldtu, .dd.t32,      0, ldfo, .double, -0.9, rd, rf )
	TEST_FRRI_R1_12( 314, fo2ut, fflag_NX, ldtu, .dd.t32,      0, ldfo, .double,  0.9, rd, rf )
	TEST_FRRI_R1_12( 315, fo2ut, fflags_Z, ldtu, .dd.t32,      1, ldfo, .double,  1.0, rd, rf )
	TEST_FRRI_R1_12( 316, fo2ut, fflag_NX, ldtu, .dd.t32,      1, ldfo, .double,  1.1, rd, rf )

	TEST_FRRI_R1_12( 321, fo2ut, fflag_NV, ldtu, .dd.t32,          0, ldfo, .double, -3e9, rd, rf )
	TEST_FRRI_R1_12( 322, fo2ut, fflags_Z, ldtu, .dd.t32, 3000000000, ldfo, .double,  3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_R1_12( 331, fo2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldfo, .double,           Inf, rd, rf )
	TEST_FRRI_R1_12( 332, fo2ut, fflag_NV, ldtu, .dd.t32,      0, ldfo, .double,          -Inf, rd, rf )
	TEST_FRRI_R1_12( 333, fo2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldfo, .double,       fo_qNaN, rd, rf )
	TEST_FRRI_R1_12( 334, fo2ut, fflag_NV, ldtu, .dd.t32, ut_max, ldfo, .double, fo_minus_qNaN, rd, rf )

	TEST_FRRI_R1_12( 411, fo2uo, fflag_NV,  ldo, .dd.o64,      0, ldfo, .double, -3.0, rd, rf )
	TEST_FRRI_R1_12( 412, fo2uo, fflag_NV,  ldo, .dd.o64,      0, ldfo, .double, -1.0, rd, rf )
	TEST_FRRI_R1_12( 413, fo2uo, fflag_NX,  ldo, .dd.o64,      0, ldfo, .double, -0.9, rd, rf )
	TEST_FRRI_R1_12( 414, fo2uo, fflag_NX,  ldo, .dd.o64,      0, ldfo, .double,  0.9, rd, rf )
	TEST_FRRI_R1_12( 415, fo2uo, fflags_Z,  ldo, .dd.o64,      1, ldfo, .double,  1.0, rd, rf )
	TEST_FRRI_R1_12( 416, fo2uo, fflag_NX,  ldo, .dd.o64,      1, ldfo, .double,  1.1, rd, rf )

	TEST_FRRI_R1_12( 421, fo2uo, fflag_NV,  ldo, .dd.o64,          0, ldfo, .double, -3e9, rd, rf )
	TEST_FRRI_R1_12( 422, fo2uo, fflags_Z, ldtu, .dd.o64, 3000000000, ldfo, .double,  3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_R1_12( 431, fo2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldfo, .double,           Inf, rd, rf )
	TEST_FRRI_R1_12( 432, fo2uo, fflag_NV,  ldo, .dd.o64,      0, ldfo, .double,          -Inf, rd, rf )
	TEST_FRRI_R1_12( 433, fo2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldfo, .double,       fo_qNaN, rd, rf )
	TEST_FRRI_R1_12( 434, fo2uo, fflag_NV,  ldo, .dd.o64, uo_max, ldfo, .double, fo_minus_qNaN, rd, rf )


DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
