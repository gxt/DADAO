#*****************************************************************************
# orr.S
#-----------------------------------------------------------------------------
#
# Test orr instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

	#-------------------------------------------------------------
	# Logical tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_123( 2, orr, 0xff0fff0f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 3, orr, 0xfff0fff0, 0x0ff00ff0, 0xf0f0f0f0 )
	TEST_ORRR_DDD_123( 4, orr, 0x0fff0fff, 0x00ff00ff, 0x0f0f0f0f )
	TEST_ORRR_DDD_123( 5, orr, 0xf0fff0ff, 0xf00ff00f, 0xf0f0f0f0 )

	#-------------------------------------------------------------
	# Source/Destination tests
	#-------------------------------------------------------------

	TEST_ORRR_DDD_112( 6, orr, 0xff0fff0f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_121( 7, orr, 0xff0fff0f, 0xff00ff00, 0x0f0f0f0f )
	TEST_ORRR_DDD_111( 8, orr, 0xff00ff00, 0xff00ff00, 0xff00ff00 )

	TEST_ORRR_DDD_102( 24, orr, 0xff00ff00,          0, 0xff00ff00 )
	TEST_ORRR_DDD_120( 25, orr, 0x00ff00ff, 0x00ff00ff,          0 )
	TEST_ORRR_DDD_100( 26, orr,          0,          0,          0 )
	TEST_ORRR_DDD_012( 27, orr,          0, 0x11111111, 0x22222222 )

	TEST_PASSFAIL

DDTEST_CODE_END

.data
DDTEST_DATA_BEGIN

	TEST_DATA

DDTEST_DATA_END
