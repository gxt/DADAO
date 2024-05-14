#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	__TEST_FCSR_SET_RMODE( __DDAEE_FCSR_RDMODE_RTZ__ )

	TEST_FRRI_M1_12( 111, ft2it, fflag_NX, ldts, .dd.t32,     -1, ldft, .single, -1.1, rd, rf )
	TEST_FRRI_M1_12( 112, ft2it, fflags_Z, ldts, .dd.t32,     -1, ldft, .single, -1.0, rd, rf )
	TEST_FRRI_M1_12( 113, ft2it, fflag_NX, ldts, .dd.t32,      0, ldft, .single, -0.9, rd, rf )
	TEST_FRRI_M1_12( 114, ft2it, fflag_NX, ldts, .dd.t32,      0, ldft, .single,  0.9, rd, rf )
	TEST_FRRI_M1_12( 115, ft2it, fflags_Z, ldts, .dd.t32,      1, ldft, .single,  1.0, rd, rf )
	TEST_FRRI_M1_12( 116, ft2it, fflag_NX, ldts, .dd.t32,      1, ldft, .single,  1.1, rd, rf )

	TEST_FRRI_M1_12( 121, ft2it, fflag_NV, ldts, .dd.t32, it_min, ldft, .single, -3e9, rd, rf )
	TEST_FRRI_M1_12( 122, ft2it, fflag_NV, ldts, .dd.t32, it_max, ldft, .single,  3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_M1_12( 131, ft2it, fflag_NV, ldts, .dd.t32, it_max, ldft, .single,           Inf, rd, rf )
	TEST_FRRI_M1_12( 132, ft2it, fflag_NV, ldts, .dd.t32, it_min, ldft, .single,          -Inf, rd, rf )
	TEST_FRRI_M1_12( 133, ft2it, fflag_NV, ldts, .dd.t32, it_max, ldft, .single,       ft_qNaN, rd, rf )
	TEST_FRRI_M1_12( 134, ft2it, fflag_NV, ldts, .dd.t32, it_max, ldft, .single, ft_minus_qNaN, rd, rf )

	TEST_FRRI_M1_12( 211, ft2io, fflag_NX,  ldo, .dd.o64,     -1, ldft, .single, -1.1, rd, rf )
	TEST_FRRI_M1_12( 212, ft2io, fflags_Z,  ldo, .dd.o64,     -1, ldft, .single, -1.0, rd, rf )
	TEST_FRRI_M1_12( 213, ft2io, fflag_NX,  ldo, .dd.o64,      0, ldft, .single, -0.9, rd, rf )
	TEST_FRRI_M1_12( 214, ft2io, fflag_NX,  ldo, .dd.o64,      0, ldft, .single,  0.9, rd, rf )
	TEST_FRRI_M1_12( 215, ft2io, fflags_Z,  ldo, .dd.o64,      1, ldft, .single,  1.0, rd, rf )
	TEST_FRRI_M1_12( 216, ft2io, fflag_NX,  ldo, .dd.o64,      1, ldft, .single,  1.1, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_M1_12( 231, ft2io, fflag_NV,  ldo, .dd.o64, io_max, ldft, .single,           Inf, rd, rf )
	TEST_FRRI_M1_12( 232, ft2io, fflag_NV,  ldo, .dd.o64, io_min, ldft, .single,          -Inf, rd, rf )
	TEST_FRRI_M1_12( 233, ft2io, fflag_NV,  ldo, .dd.o64, io_max, ldft, .single,       ft_qNaN, rd, rf )
	TEST_FRRI_M1_12( 234, ft2io, fflag_NV,  ldo, .dd.o64, io_max, ldft, .single, ft_minus_qNaN, rd, rf )

	TEST_FRRI_M1_12( 311, fo2it, fflag_NX, ldts, .dd.t32,     -1, ldfo, .double, -1.1, rd, rf )
	TEST_FRRI_M1_12( 312, fo2it, fflags_Z, ldts, .dd.t32,     -1, ldfo, .double, -1.0, rd, rf )
	TEST_FRRI_M1_12( 313, fo2it, fflag_NX, ldts, .dd.t32,      0, ldfo, .double, -0.9, rd, rf )
	TEST_FRRI_M1_12( 314, fo2it, fflag_NX, ldts, .dd.t32,      0, ldfo, .double,  0.9, rd, rf )
	TEST_FRRI_M1_12( 315, fo2it, fflags_Z, ldts, .dd.t32,      1, ldfo, .double,  1.0, rd, rf )
	TEST_FRRI_M1_12( 316, fo2it, fflag_NX, ldts, .dd.t32,      1, ldfo, .double,  1.1, rd, rf )

	TEST_FRRI_M1_12( 321, fo2it, fflag_NV, ldts, .dd.t32, it_min, ldfo, .double, -3e9, rd, rf )
	TEST_FRRI_M1_12( 322, fo2it, fflag_NV, ldts, .dd.t32, it_max, ldfo, .double,  3e9, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_M1_12( 331, fo2it, fflag_NV, ldts, .dd.t32, it_max, ldfo, .double,           Inf, rd, rf )
	TEST_FRRI_M1_12( 332, fo2it, fflag_NV, ldts, .dd.t32, it_min, ldfo, .double,          -Inf, rd, rf )
	TEST_FRRI_M1_12( 333, fo2it, fflag_NV, ldts, .dd.t32, it_max, ldfo, .double,       fo_qNaN, rd, rf )
	TEST_FRRI_M1_12( 334, fo2it, fflag_NV, ldts, .dd.t32, it_max, ldfo, .double, fo_minus_qNaN, rd, rf )

	TEST_FRRI_M1_12( 411, fo2io, fflag_NX,  ldo, .dd.o64,     -1, ldfo, .double, -1.1, rd, rf )
	TEST_FRRI_M1_12( 412, fo2io, fflags_Z,  ldo, .dd.o64,     -1, ldfo, .double, -1.0, rd, rf )
	TEST_FRRI_M1_12( 413, fo2io, fflag_NX,  ldo, .dd.o64,      0, ldfo, .double, -0.9, rd, rf )
	TEST_FRRI_M1_12( 414, fo2io, fflag_NX,  ldo, .dd.o64,      0, ldfo, .double,  0.9, rd, rf )
	TEST_FRRI_M1_12( 415, fo2io, fflags_Z,  ldo, .dd.o64,      1, ldfo, .double,  1.0, rd, rf )
	TEST_FRRI_M1_12( 416, fo2io, fflag_NX,  ldo, .dd.o64,      1, ldfo, .double,  1.1, rd, rf )

	TEST_FRRI_M1_12( 421, fo2io, fflags_Z,  ldo, .dd.o64, -3000000000, ldfo, .double,  -3e9, rd, rf )
	TEST_FRRI_M1_12( 422, fo2io, fflags_Z,  ldo, .dd.o64,  3000000000, ldfo, .double,   3e9, rd, rf )
	TEST_FRRI_M1_12( 423, fo2io, fflag_NV,  ldo, .dd.o64,      io_min, ldfo, .double, -3e19, rd, rf )
	TEST_FRRI_M1_12( 424, fo2io, fflag_NV,  ldo, .dd.o64,      io_max, ldfo, .double,  3e19, rd, rf )

	# test Inf, -Inf, NaN, -NaN
	TEST_FRRI_M1_12( 431, fo2io, fflag_NV,  ldo, .dd.o64, io_max, ldfo, .double,           Inf, rd, rf )
	TEST_FRRI_M1_12( 432, fo2io, fflag_NV,  ldo, .dd.o64, io_min, ldfo, .double,          -Inf, rd, rf )
	TEST_FRRI_M1_12( 433, fo2io, fflag_NV,  ldo, .dd.o64, io_max, ldfo, .double,       fo_qNaN, rd, rf )
	TEST_FRRI_M1_12( 434, fo2io, fflag_NV,  ldo, .dd.o64, io_max, ldfo, .double, fo_minus_qNaN, rd, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
