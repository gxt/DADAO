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

  TEST_RRII_OP( 2, cmps, 0x0000000000000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 3, cmps, 0x0000000000000001, 0x00000005, 0x000 );
  TEST_RRII_OP( 4, cmps, 0xffffffffffffffff, 0x000,0x000000005 );
  TEST_RRII_OP( 5, cmps, 0xffffffffffffffff, 0xffffffffffffffff, 0x000 );
  TEST_RRII_OP( 6, cmps, 0x0000000000000001, 0x000, 0xffffffffffffffff );

  TEST_RRII_OP( 7, cmps, 0x0000000000000000, 0x0000000000000005, 0x005 );
  TEST_RRII_OP( 8, cmps, 0x0000000000000000, 0xffffffffffffffff,    -1 );

  TEST_RRII_OP( 9, cmps, 0x0000000000000001, 0x00ffffffffffffff,  0x7ff );
  TEST_RRII_OP(10, cmps, 0xffffffffffffffff, 0x000000000000000f,  0x7ff );

  TEST_RRII_OP(11, cmps, 0x0000000000000001, 0xffffffffffffffff, -0x800 );
  TEST_RRII_OP(12, cmps, 0xffffffffffffffff, 0xffffff0000000000,  0x00f );
  TEST_RRII_OP(13, cmps, 0xffffffffffffffff, 0xfffffffffffffffe,     -1 );   

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRII_SRC1_EQ_DEST( 14, cmps, 1, 5, 4 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRII_DEST_BYPASS( 15, 0, cmps, 1, 5, 4 );
  TEST_RRII_DEST_BYPASS( 16, 1, cmps, 0, 5, 5 );
  TEST_RRII_DEST_BYPASS( 17, 2, cmps, -1, 4, 5 );

  TEST_RRII_SRC1_BYPASS( 18, 0, cmps, 1, 5, 4 );
  TEST_RRII_SRC1_BYPASS( 19, 1, cmps, 0, 5, 5 );
  TEST_RRII_SRC1_BYPASS( 20, 2, cmps, -1, 4, 5 );

  TEST_RRII_ZEROSRC1( 21, cmps, 1, -1 );
  TEST_RRII_ZERODEST( 22, cmps, 4, 5 );

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------
  
  TEST_ORRR_OP( 23, cmps, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 );
  TEST_ORRR_OP( 24, cmps, 0x0000000000000000, 0x0fffffffffffffff, 0x0fffffffffffffff );
  TEST_ORRR_OP( 25, cmps, 0xffffffffffffffff, 0x0000000000000003, 0x0000000000000005 );
  TEST_ORRR_OP( 26, cmps, 0x0000000000000001, 0x0000000000000005, 0x0000000000000002 );

  TEST_ORRR_OP( 27, cmps, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff );
  TEST_ORRR_OP( 28, cmps, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff );
  TEST_ORRR_OP( 29, cmps, 0x0000000000000001, 0xffffffffffffffff, 0xfffffffffffffffe );

  TEST_ORRR_OP( 30, cmps, 0x0000000000000001, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 );
  TEST_ORRR_OP( 31, cmps, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0x0f0f0f0f0f0f0f0f );
  
  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_ORRR_SRC1_EQ_DEST( 32, cmps, 1, 6, 4 );
  TEST_ORRR_SRC2_EQ_DEST( 33, cmps, 1, 7, 5 );
  TEST_ORRR_SRC12_EQ_DEST(34, cmps, 0, 8);

  TEST_ORRR_ZEROSRC1( 35, cmps, 1, -1);
  TEST_ORRR_ZEROSRC1( 36, cmps, 0,  0);
  TEST_ORRR_ZEROSRC2( 37, cmps, 1,  1);
  TEST_ORRR_ZEROSRC2( 38, cmps, 0,  0);
  TEST_ORRR_ZEROSRC12(39, cmps, 0);
  TEST_ORRR_ZERODEST( 40, cmps, 5, 6 );


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
