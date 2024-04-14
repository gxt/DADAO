#*****************************************************************************
# andnw.S
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

	TEST_RWII_B( 6, andnw, w0, 0xffff00000000f000, 0xffff00000000f0f0, 0x00ff )
	TEST_RWII_B( 7, andnw, w1, 0xffff0000f0000000, 0xffff0000f0f00000, 0x00ff )
	TEST_RWII_B( 8, andnw, w2, 0x0000f0000000ffff, 0x0000f0f00000ffff, 0x00ff )
	TEST_RWII_B( 9, andnw, w3, 0xf00000000000ffff, 0xf0f000000000ffff, 0x00ff )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
