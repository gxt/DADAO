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

	TEST_RWII_F( 2, setw, w0, 0x0f0f0f0f0f0fffff, 0x0f0f0f0f0f0f0000, 0xffff )
	TEST_RWII_F( 3, setw, w1, 0x0f0f0f0fffff0f0f, 0x0f0f0f0f00000f0f, 0xffff )
	TEST_RWII_F( 4, setw, w2, 0x0f0fffff0f0f0f0f, 0x0f0f00000f0f0f0f, 0xffff )
	TEST_RWII_F( 5, setw, w3, 0xffff0f0f0f0f0f0f, 0x00000f0f0f0f0f0f, 0xffff )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
