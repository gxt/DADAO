#*****************************************************************************
# cmps.S
#-----------------------------------------------------------------------------
#
# Test cmps instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRII_OP( 2, cmps, 0x00000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 3, cmps, 0x00000001, 0x00000005, 0x000 );
  TEST_RRII_OP( 4, cmps, 1, 0x00000000, -1);

  TEST_RRII_OP( 5, cmps, 0x0000000000000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 6, cmps, 0xffffffffffffffff, 0xffffffffffffffff, 0x000 );
  TEST_RRII_OP( 7, cmps, 0x0000000000000001, 0x00f0000000000000, 0x000 );
  TEST_RRII_OP( 8, cmps, 0x0000000000000001, -1, -2 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRII_SRC1_EQ_DEST( 9, cmps, 1, 5, 4 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRII_DEST_BYPASS( 10, 0, cmps, 1, 5, 4 );
  TEST_RRII_DEST_BYPASS( 11, 1, cmps, 0, 5, 5 );
  TEST_RRII_DEST_BYPASS( 12, 2, cmps, -1, 4, 5 );

  TEST_RRII_SRC1_BYPASS( 13, 0, cmps, 1, 5, 4 );
  TEST_RRII_SRC1_BYPASS( 14, 1, cmps, 0, 5, 5 );
  TEST_RRII_SRC1_BYPASS( 15, 2, cmps, -1, 4, 5 );

  TEST_RRII_ZEROSRC1( 16, cmps, 1, -1 );
  TEST_RRII_ZERODEST( 17, cmps, 4, 5 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
