#*****************************************************************************
# orw.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_RWII_B( 6, orw, w0, 0x000000000000ffff, 0x000000000000f0f0, 0x0f0f )
	TEST_RWII_B( 7, orw, w1, 0x00000000ffff0000, 0x00000000f0f00000, 0x0f0f )
	TEST_RWII_B( 8, orw, w2, 0x0000ffff00000000, 0x0000f0f000000000, 0x0f0f )
	TEST_RWII_B( 9, orw, w3, 0xffff000000000000, 0xf0f0000000000000, 0x0f0f )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
