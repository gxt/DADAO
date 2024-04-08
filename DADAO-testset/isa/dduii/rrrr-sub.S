#*****************************************************************************
# sub.S
#-----------------------------------------------------------------------------
#
# Test sub instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1234( 2, sub, 0x0, 0x0, 0x0, 0x0 )
	TEST_RRRR_WWRR_1234( 3, sub, 0x0, 0x1, 0x2, 0x1 )
	TEST_RRRR_WWRR_1234( 4, sub, 0x0, 0x0, 0x1, 0x1 )

	TEST_RRRR_WWRR_1234( 5,  sub, 0x0000000000000000, 0x0000000000008000, 0x0000000000000000, 0xffffffffffff8000 )
	TEST_RRRR_WWRR_1234( 6,  sub, 0xffffffffffffffff, 0xffffffff80000000, 0xffffffff80000000, 0x0000000000000000 )
	TEST_RRRR_WWRR_1234( 7,  sub, 0xffffffffffffffff, 0xffffffff80008000, 0xffffffff80000000, 0xffffffffffff8000 )

	TEST_RRRR_WWRR_1234( 8,  sub, 0xffffffffffffffff, 0xffffffffffff8001, 0x0000000000000000, 0x0000000000007fff )
	TEST_RRRR_WWRR_1234( 9,  sub, 0x0000000000000000, 0x000000007fffffff, 0x000000007fffffff, 0x0000000000000000 )
	TEST_RRRR_WWRR_1234( 10, sub, 0x0000000000000000, 0x000000007fff8000, 0x000000007fffffff, 0x0000000000007fff )

	TEST_RRRR_WWRR_1234( 11, sub, 0xffffffffffffffff, 0xffffffff7fff8001, 0xffffffff80000000, 0x0000000000007fff )
	TEST_RRRR_WWRR_1234( 12, sub, 0x0000000000000000, 0x0000000080007fff, 0x000000007fffffff, 0xffffffffffff8000 )

	TEST_RRRR_WWRR_1234( 38, sub, 0x0000000000000000, 0x100000007fffffff, 0x000000007fffffff, 0xf000000000000000 )

	TEST_RRRR_WWRR_1234( 13, sub, 0x0000000000000000, 0x0000000000000001, 0x0000000000000000, 0xffffffffffffffff )
	TEST_RRRR_WWRR_1234( 14, sub, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff, 0x0000000000000001 )
	TEST_RRRR_WWRR_1234( 15, sub, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff )

	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1213( 16, sub, 0, 13, 24, 11 )
	TEST_RRRR_WWRR_1223( 17, sub, 0, 14, 25, 11 )
	TEST_RRRR_WWRR_1231( 18, sub, 0, 15, 26, 11 )
	TEST_RRRR_WWRR_1232( 19, sub, 0, 16, 27, 11 )

	TEST_RRRR_WWRR_1211( 20, sub, 0, 0, 14, 14 )
	TEST_RRRR_WWRR_1222( 21, sub, 0, 0, 15, 15 )

	TEST_RRRR_WWRR_1203( 25, sub, 0xffffffffffffffff, 0xffffffffffffffff, 0, 1 )
	TEST_RRRR_WWRR_1230( 26, sub, 0, 34, 34, 0 )
	TEST_RRRR_WWRR_1200( 27, sub, 0, 0, 0, 0 )

	TEST_RRRR_WWRR_0123(28, sub, 0, 4, 8, 4 )
	TEST_RRRR_WWRR_1023(29, sub, 0, 0, 5, 5 ) 

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

 

