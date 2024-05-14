#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_M1_12( 111, ut2ft, fflags_Z, ldft, .single,          2.0, ldtu, .dd.t32,  2, rf, rd )
	TEST_FRRI_M1_12( 112, ut2ft, fflag_NX, ldft, .single,  4.2949673e9, ldtu, .dd.t32, -2, rf, rd )
	TEST_FRRI_M1_12( 113, ut2ft, fflags_Z, ldft, .single,          0.0, ldtu, .dd.t32,  0, rf, rd )

	TEST_FRRI_M1_12( 211, uo2ft, fflags_Z, ldft, .single,          2.0,  ldo, .dd.o64,  2, rf, rd )
	TEST_FRRI_M1_12( 212, uo2ft, fflag_NX, ldft, .single, 1.8446744e19,  ldo, .dd.o64, -2, rf, rd )
	TEST_FRRI_M1_10( 213, uo2ft, fflags_Z, ldft, .single,          0.0,  ldo, .dd.o64,  0, rf, rd )

	TEST_FRRI_M1_12( 311, ut2fo, fflags_Z, ldfo, .double,          2.0, ldtu, .dd.t32,  2, rf, rd )
	TEST_FRRI_M1_12( 312, ut2fo, fflags_Z, ldfo, .double,   4294967294, ldtu, .dd.t32, -2, rf, rd )
	TEST_FRRI_M1_12( 313, ut2fo, fflags_Z, ldfo, .double,          0.0, ldtu, .dd.t32,  0, rf, rd )

	TEST_FRRI_M1_12( 411, uo2fo, fflags_Z, ldfo, .double,                   2.0,  ldo, .dd.o64,  2, rf, rd )
	TEST_FRRI_M1_12( 412, uo2fo, fflag_NX, ldfo, .double, 1.8446744073709552e19,  ldo, .dd.o64, -2, rf, rd )
	TEST_FRRI_M1_10( 413, uo2fo, fflags_Z, ldfo, .double,                   0.0,  ldo, .dd.o64,  0, rf, rd )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
