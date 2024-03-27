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

  TEST_RRII_OP( 2, cmpu, 0x0000000000000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 3, cmpu, 0x0000000000000001, 0x00000005, 0x000 );
  TEST_RRII_OP( 4, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 );

  TEST_RRII_OP( 5, cmpu, 0x0000000000000000, 0x00000fff, 0xfff );
  TEST_RRII_OP( 6, cmpu, 0x0000000000000001, 0xffffffffffffffff, 0xfff );
  TEST_RRII_OP( 7, cmpu, 0xffffffffffffffff, 0x00000000000000ff, 0xfff );

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

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_ORRR_OP( 17, cmpu, 0x0, 0x0, 0x0 );
  TEST_ORRR_OP( 18, cmpu, 0x1, 0x5, 0x0 );
  TEST_ORRR_OP( 19, cmpu, 0xffffffffffffffff, 0x00000000, 0x002 );

  TEST_ORRR_OP( 20, cmpu, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff );
  TEST_ORRR_OP( 21, cmpu, 0x0000000000000001, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f );
  TEST_ORRR_OP( 22, cmpu, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_ORRR_SRC1_EQ_DEST( 23, cmpu, 1, 5, 4 );
  TEST_ORRR_SRC2_EQ_DEST( 24, cmpu,-1, 4, 5 );
  TEST_ORRR_SRC12_EQ_DEST(25, cmpu, 0, 6 );

  TEST_ORRR_ZEROSRC1( 26, cmpu, -1, 1 );
  TEST_ORRR_ZEROSRC2( 27, cmpu,  1, 5 );
  TEST_ORRR_ZEROSRC12(28, cmpu,  0); 
  TEST_ORRR_ZERODEST( 29, cmpu,  5, 6 );
 

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END


