#*****************************************************************************
# cmpu.S
#-----------------------------------------------------------------------------
#
# Test cmpu instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRII_OP( 2, cmpu, 0x00000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 3, cmpu, 0x00000001, 0x00000005, 0x000 );
  TEST_RRII_OP( 4, cmpu, -1, 0x00000000, 2 );

  TEST_RRII_OP( 5, cmpu, 0x0000000000000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 6, cmpu, 0x0000000000000001, 0xffffffffffffffff, 0x000 );
  TEST_RRII_OP( 7, cmpu, 0x0000000000000001, 0x00f0000000000000, 0x000 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRII_SRC1_EQ_DEST( 8, cmpu, 1, 5, 4 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRII_DEST_BYPASS( 9, 0, cmpu, 1, 5, 4 );
  TEST_RRII_DEST_BYPASS( 10, 1, cmpu, 0, 5, 5 );
  TEST_RRII_DEST_BYPASS( 11, 2, cmpu, -1, 4, 5 );

  TEST_RRII_SRC1_BYPASS( 12, 0, cmpu, 1, 5, 4 );
  TEST_RRII_SRC1_BYPASS( 13, 1, cmpu, 0, 5, 5 );
  TEST_RRII_SRC1_BYPASS( 14, 2, cmpu, -1, 4, 5 );

  TEST_RRII_ZEROSRC1( 15, cmpu, -1, 1 );
  TEST_RRII_ZERODEST( 16, cmpu, 4, 5 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END


