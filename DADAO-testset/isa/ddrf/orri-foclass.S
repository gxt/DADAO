#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_FRRI_R1_12( 11, foclass, fflags_Z, ldo, .dd.o64, 1 << 0, ldfo, .dd.o64, 0xfff0000000000000, rd, rf )
	TEST_FRRI_R1_12( 12, foclass, fflags_Z, ldo, .dd.o64, 1 << 1, ldfo, .dd.o64, 0xbff0000000000000, rd, rf )
	TEST_FRRI_R1_12( 13, foclass, fflags_Z, ldo, .dd.o64, 1 << 2, ldfo, .dd.o64, 0x800fffffffffffff, rd, rf )
	TEST_FRRI_R1_12( 14, foclass, fflags_Z, ldo, .dd.o64, 1 << 3, ldfo, .dd.o64, 0x8000000000000000, rd, rf )
	TEST_FRRI_R1_12( 15, foclass, fflags_Z, ldo, .dd.o64, 1 << 4, ldfo, .dd.o64, 0x0000000000000000, rd, rf )
	TEST_FRRI_R1_12( 16, foclass, fflags_Z, ldo, .dd.o64, 1 << 5, ldfo, .dd.o64, 0x000fffffffffffff, rd, rf )
	TEST_FRRI_R1_12( 17, foclass, fflags_Z, ldo, .dd.o64, 1 << 6, ldfo, .dd.o64, 0x3ff0000000000000, rd, rf )
	TEST_FRRI_R1_12( 18, foclass, fflags_Z, ldo, .dd.o64, 1 << 7, ldfo, .dd.o64, 0x7ff0000000000000, rd, rf )
	TEST_FRRI_R1_12( 19, foclass, fflags_Z, ldo, .dd.o64, 1 << 8, ldfo, .dd.o64, 0x7ff0000000000001, rd, rf )
	TEST_FRRI_R1_12( 20, foclass, fflags_Z, ldo, .dd.o64, 1 << 9, ldfo, .dd.o64, 0x7ff8000000000000, rd, rf )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
