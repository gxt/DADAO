#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 11, ftclass, fflags_Z, ldo, .dd.o64, 1 << 0, ldft, .dd.t32, 0xff800000, rd, rf )
	TEST_FRRI_R1_12( 12, ftclass, fflags_Z, ldo, .dd.o64, 1 << 1, ldft, .dd.t32, 0xbf800000, rd, rf )
	TEST_FRRI_R1_12( 13, ftclass, fflags_Z, ldo, .dd.o64, 1 << 2, ldft, .dd.t32, 0x807fffff, rd, rf )
	TEST_FRRI_R1_12( 14, ftclass, fflags_Z, ldo, .dd.o64, 1 << 3, ldft, .dd.t32, 0x80000000, rd, rf )
	TEST_FRRI_R1_12( 15, ftclass, fflags_Z, ldo, .dd.o64, 1 << 4, ldft, .dd.t32, 0x00000000, rd, rf )
	TEST_FRRI_R1_12( 16, ftclass, fflags_Z, ldo, .dd.o64, 1 << 5, ldft, .dd.t32, 0x007fffff, rd, rf )
	TEST_FRRI_R1_12( 17, ftclass, fflags_Z, ldo, .dd.o64, 1 << 6, ldft, .dd.t32, 0x3f800000, rd, rf )
	TEST_FRRI_R1_12( 18, ftclass, fflags_Z, ldo, .dd.o64, 1 << 7, ldft, .dd.t32, 0x7f800000, rd, rf )
	TEST_FRRI_R1_12( 19, ftclass, fflags_Z, ldo, .dd.o64, 1 << 8, ldft, .dd.t32, 0x7f800001, rd, rf )
	TEST_FRRI_R1_12( 20, ftclass, fflags_Z, ldo, .dd.o64, 1 << 9, ldft, .dd.t32, 0x7fc00000, rd, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
