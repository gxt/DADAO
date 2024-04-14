#*****************************************************************************
# muls.S
#-----------------------------------------------------------------------------
#
# Test muls instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1234( 2, muls, 0x0, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 )
	TEST_RRRR_WWRR_1234( 3, muls, 0x0, 0x0000000000000001, 0x0000000000000001, 0x0000000000000001 )
	TEST_RRRR_WWRR_1234( 4, muls, 0x0, 0x0000000000000015, 0x0000000000000003, 0x0000000000000007 )

	
	TEST_RRRR_WWRR_1234( 5, muls, 0x0, 0x0000000000000000, 0x0000000000000000, 0xffffffffffff8000 )
	TEST_RRRR_WWRR_1234( 6, muls, 0x0, 0x0000000000000000, 0xffffffff80000000, 0x00000000 )
	TEST_RRRR_WWRR_1234( 7, muls, 0x0, 0x0000400000000000, 0xffffffff80000000, 0xffffffffffff8000 )

	TEST_RRRR_WWRR_1234( 8, muls, 0xffffffffffffffff, 0xfffffffffffffffe, 0x0000000000000002, 0xffffffffffffffff )
	TEST_RRRR_WWRR_1234( 9, muls, 0xffffffffffff0081, 0x000000000000ff7f, 0x000000000002fe7d, 0xaaaaaaaaaaaaaaab )


	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1213( 10, muls, 0, 24, 3, 8 )
	TEST_RRRR_WWRR_1223( 11, muls, 0, 27, 3, 9 )
	TEST_RRRR_WWRR_1231( 12, muls, 0, 30, 3, 10 )
	TEST_RRRR_WWRR_1232( 13, muls, 0, 33, 3, 11 )

	TEST_RRRR_WWRR_1211( 14, muls, 0, 25, 5, 5 )
	TEST_RRRR_WWRR_1222( 15, muls, 0, 36, 6, 6 )

	TEST_RRRR_WWRR_1203( 19, muls, 0, 0, 0, 33 )
	TEST_RRRR_WWRR_1230( 20, muls, 0, 0, 34, 0 )
	TEST_RRRR_WWRR_1200( 21, muls, 0, 0, 0, 0 )
	TEST_RRRR_WWRR_0123( 22, muls, 0, 39, 3, 13 )
	TEST_RRRR_WWRR_1023( 23, muls, 0, 0, 3, 14 )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END



