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

  TEST_ORRR_OP( 2, and, 0x0f000f00, 0xff00ff00, 0x0f0f0f0f );
  TEST_ORRR_OP( 3, and, 0x00f000f0, 0x0ff00ff0, 0xf0f0f0f0 );
  TEST_ORRR_OP( 4, and, 0xf000f000, 0xf00ff00f, 0xf0f0f0f0 );
  TEST_ORRR_OP( 5, and, 0xf000f000, 0xf00ff00f, 0xf0f0f0f0 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_ORRR_SRC1_EQ_DEST( 6, and, 0x0f000f00, 0xff00ff00, 0x0f0f0f0f );
  TEST_ORRR_SRC2_EQ_DEST( 7, and, 0x00f000f0, 0x0ff00ff0, 0xf0f0f0f0 );
  TEST_ORRR_SRC12_EQ_DEST( 8, and, 0xff00ff00, 0xff00ff00 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
