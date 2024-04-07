#*****************************************************************************
# and.S
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

	TEST_ORRR_DDD_123( 2, and, 0x0f000f00, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 3, and, 0x00f000f0, 0x0ff00ff0, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 4, and, 0xf000f000, 0xf00ff00f, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 5, and, 0xf000f000, 0xf00ff00f, 0xf0f0f0f0 )

	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_112( 6, and, 0x0f000f00, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_121( 7, and, 0x00f000f0, 0x0ff00ff0, 0xf0f0f0f0 )
	TEST_ORRR_DDD_111( 8, and, 0xff00ff00, 0xff00ff00, 0xff00ff00 )

	#-------------------------------------------------------------
	# Bypassing tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_102( 24, and, 0,          0, 0xff00ff00 )
	TEST_ORRR_DDD_120( 25, and, 0, 0x00ff00ff,          0 )
	TEST_ORRR_DDD_100( 26, and, 0,          0,          0 )
	TEST_ORRR_DDD_012( 27, and, 0, 0x11111111, 0x22222222 );

	TEST_PASSFAIL

DDTEST_CODE_END

.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
