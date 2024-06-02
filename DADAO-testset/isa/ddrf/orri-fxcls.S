#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_M1_12( 111, ftcls, fflags_Z, ldo, .dd.o64, 1 << 0, ldft, .dd.t32, 0xff800000, rd, rf )
	TEST_FRRI_M1_12( 112, ftcls, fflags_Z, ldo, .dd.o64, 1 << 1, ldft, .dd.t32, 0xbf800000, rd, rf )
	TEST_FRRI_M1_12( 113, ftcls, fflags_Z, ldo, .dd.o64, 1 << 2, ldft, .dd.t32, 0x807fffff, rd, rf )
	TEST_FRRI_M1_12( 114, ftcls, fflags_Z, ldo, .dd.o64, 1 << 3, ldft, .dd.t32, 0x80000000, rd, rf )
	TEST_FRRI_M1_12( 115, ftcls, fflags_Z, ldo, .dd.o64, 1 << 4, ldft, .dd.t32, 0x00000000, rd, rf )
	TEST_FRRI_M1_12( 116, ftcls, fflags_Z, ldo, .dd.o64, 1 << 5, ldft, .dd.t32, 0x007fffff, rd, rf )
	TEST_FRRI_M1_12( 117, ftcls, fflags_Z, ldo, .dd.o64, 1 << 6, ldft, .dd.t32, 0x3f800000, rd, rf )
	TEST_FRRI_M1_12( 118, ftcls, fflags_Z, ldo, .dd.o64, 1 << 7, ldft, .dd.t32, 0x7f800000, rd, rf )
	TEST_FRRI_M1_12( 119, ftcls, fflags_Z, ldo, .dd.o64, 1 << 8, ldft, .dd.t32, 0x7f800001, rd, rf )
	TEST_FRRI_M1_12( 120, ftcls, fflags_Z, ldo, .dd.o64, 1 << 9, ldft, .dd.t32, 0x7fc00000, rd, rf )

	TEST_FRRI_M1_10( 121, ftcls, fflags_Z, ldo, .dd.o64, 1 << 9, ldft, .dd.t32, 0, rd, rf )

	TEST_FRRI_M1_12( 211, focls, fflags_Z, ldo, .dd.o64, 1 << 0, ldfo, .dd.o64, 0xfff0000000000000, rd, rf )
	TEST_FRRI_M1_12( 212, focls, fflags_Z, ldo, .dd.o64, 1 << 1, ldfo, .dd.o64, 0xbff0000000000000, rd, rf )
	TEST_FRRI_M1_12( 213, focls, fflags_Z, ldo, .dd.o64, 1 << 2, ldfo, .dd.o64, 0x800fffffffffffff, rd, rf )
	TEST_FRRI_M1_12( 214, focls, fflags_Z, ldo, .dd.o64, 1 << 3, ldfo, .dd.o64, 0x8000000000000000, rd, rf )
	TEST_FRRI_M1_12( 215, focls, fflags_Z, ldo, .dd.o64, 1 << 4, ldfo, .dd.o64, 0x0000000000000000, rd, rf )
	TEST_FRRI_M1_12( 216, focls, fflags_Z, ldo, .dd.o64, 1 << 5, ldfo, .dd.o64, 0x000fffffffffffff, rd, rf )
	TEST_FRRI_M1_12( 217, focls, fflags_Z, ldo, .dd.o64, 1 << 6, ldfo, .dd.o64, 0x3ff0000000000000, rd, rf )
	TEST_FRRI_M1_12( 218, focls, fflags_Z, ldo, .dd.o64, 1 << 7, ldfo, .dd.o64, 0x7ff0000000000000, rd, rf )
	TEST_FRRI_M1_12( 219, focls, fflags_Z, ldo, .dd.o64, 1 << 8, ldfo, .dd.o64, 0x7ff0000000000001, rd, rf )
	TEST_FRRI_M1_12( 220, focls, fflags_Z, ldo, .dd.o64, 1 << 9, ldfo, .dd.o64, 0x7ff8000000000000, rd, rf )

	TEST_FRRI_M1_10( 221, focls, fflags_Z, ldo, .dd.o64, 1 << 9, ldfo, .dd.o64, 0, rd, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END