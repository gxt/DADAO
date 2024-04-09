#*****************************************************************************
# adrp.S
#-----------------------------------------------------------------------------
#
# Test adrp instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_RIII( 2, adrp, 0x80000000, 0)
	TEST_RIII( 3, adrp, 0x80005000, 5)
	TEST_RIII( 4, adrp, 0x90000000, 0x10000)
	TEST_RIII( 5, adrp, 0x7fffb000, -5)
	TEST_RIII( 6, adrp, 0x9ffff000, 0x1ffff)
	TEST_RIII( 7, adrp, 0x60000000, -0x20000)

	TEST_PASSFAIL

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END

 
