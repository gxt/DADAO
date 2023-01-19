#*****************************************************************************
# andi.S
#-----------------------------------------------------------------------------
#
# Test andi instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRII_OP( 2, andi, 0x0000000000000f00, 0xffffffffff00ff00, 0xf0f );
  TEST_RRII_OP( 3, andi, 0x00000000000000f0, 0x000000000ff00ff0, 0x0f0 );
  TEST_RRII_OP( 4, andi, 0x000000000000000f, 0x0000000000ff00ff, 0x70f );
  TEST_RRII_OP( 5, andi, 0x0000000000000000, 0xfffffffff00ff00f, 0x0f0 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRII_SRC1_EQ_DEST( 6, andi, 0x0000000000000000, 0xffffffffff00ff00, 0x0f0 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRII_DEST_BYPASS( 7,  0, andi, 0x00000700, 0x0ff00ff0, 0x70f );
  TEST_RRII_DEST_BYPASS( 8,  1, andi, 0x000000f0, 0x00ff00ff, 0x0f0 );
  TEST_RRII_DEST_BYPASS( 9,  2, andi, 0x0000000f, 0xf00ff00f, 0xf0f );

  TEST_RRII_SRC1_BYPASS( 10, 0, andi, 0x00000700, 0x0ff00ff0, 0x70f );
  TEST_RRII_SRC1_BYPASS( 11, 1, andi, 0x000000f0, 0x00ff00ff, 0x0f0 );
  TEST_RRII_SRC1_BYPASS( 12, 2, andi, 0x0000000f, 0xf00ff00f, 0x70f );

  TEST_RRII_ZEROSRC1( 13, andi,   0, 0xf);
  TEST_RRII_ZERODEST( 14, andi, 0xf, 0xf);

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
