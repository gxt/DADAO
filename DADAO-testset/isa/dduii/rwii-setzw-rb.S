#*****************************************************************************
# setzw.S
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

	TEST_RWII_B( 6, setzw, w0, 0x000000000000ffff, 0xffffffffffff0000, 0xffff )
	TEST_RWII_B( 7, setzw, w1, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff )
	TEST_RWII_B( 8, setzw, w2, 0x0000ffff00000000, 0xffff0000ffff0000, 0xffff )
	TEST_RWII_B( 9, setzw, w3, 0xffff000000000000, 0x0000ffffffffffff, 0xffff )

DDTEST_CODE_END

	.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
