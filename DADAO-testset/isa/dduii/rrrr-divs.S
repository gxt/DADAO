#*****************************************************************************
# divs.S
#-----------------------------------------------------------------------------
#
# Test divs instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1234( 2, divs,  2,  3,  20,  6)
	TEST_RRRR_WWRR_1234( 3, divs, -2, -3, -20,  6)
	TEST_RRRR_WWRR_1234( 4, divs,  2, -3,  20, -6)
	TEST_RRRR_WWRR_1234( 5, divs, -2,  3, -20, -6)

	TEST_RRRR_WWRR_1234( 6, divs, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff)
	TEST_RRRR_WWRR_1234( 7, divs, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff)


	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_RRRR_WWRR_1213( 8, divs, 0, 3, 24, 8 )
	TEST_RRRR_WWRR_1223( 9, divs, 0, 3, 27, 9 )
	TEST_RRRR_WWRR_1231( 10, divs, 0, 3, 30, 10 )
	TEST_RRRR_WWRR_1232( 11, divs, 0, 3, 33, 11 )

	TEST_RRRR_WWRR_1211( 12, divs, 0, 1, 25, 25 )
	TEST_RRRR_WWRR_1222( 13, divs, 0, 1, 36, 36 )

	TEST_RRRR_WWRR_1203( 17, divs, 0, 0, 0, 33 )
	TEST_RRRR_WWRR_0123( 18, divs, 0, 3, 20, 6 )
	TEST_RRRR_WWRR_1023( 19, divs, 2, 0, 26, 6 ) 

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END



