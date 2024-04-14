#*****************************************************************************
# xor.S
#-----------------------------------------------------------------------------
#
# Test xor instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_123( 2, xor, 0xf00ff00f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 3, xor, 0xff00ff00, 0x0ff00ff0, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 4, xor, 0x0ff00ff0, 0x00ff00ff, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 5, xor, 0x00ff00ff, 0xf00ff00f, 0xf0f0f0f0 )

	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_112( 6, xor, 0xf00ff00f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_121( 7, xor, 0xf00ff00f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_111( 8, xor, 0x00000000, 0xff00ff00, 0xff00ff00 )

	TEST_ORRR_DDD_102( 24, xor, 0xff00ff00,          0, 0xff00ff00 )
	TEST_ORRR_DDD_120( 25, xor, 0x00ff00ff, 0x00ff00ff,          0 )
	TEST_ORRR_DDD_100( 26, xor,          0,          0,          0 )
	TEST_ORRR_DDD_012( 27, xor,          0, 0x11111111, 0x22222222 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
