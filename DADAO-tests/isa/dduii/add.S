#*****************************************************************************
# add.S
#-----------------------------------------------------------------------------
#
# Test add instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, add, 0x0, 0x0, 0x0, 0x0 );
  TEST_RRRR_OP( 3, add, 0x0, 0x2, 0x1, 0x1 );
  TEST_RRRR_OP( 4, add, 0x0, 0xa, 0x3, 0x7 );

  TEST_RRRR_OP( 5, add, 0x0, 0xffffffffffff8000, 0x0000000000000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 6, add, 0x0, 0xffffffff80000000, 0xffffffff80000000, 0x00000000 );

  TEST_RRRR_OP( 7, add, 0x1, 0xffffffff7fff8000, 0xffffffff80000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 8, add, 0x0000000000000000, 0xe000000000000000, 0x7000000000000000, 0x7000000000000000 );

  TEST_RRRR_OP( 9, add, 0x0, 0x0000000000007fff, 0x0000000000000000, 0x0000000000007fff );
  TEST_RRRR_OP( 10, add, 0x0, 0x000000007fffffff, 0x000000007fffffff, 0x0000000000000000 );
  TEST_RRRR_OP( 11, add, 0x0, 0x0000000080007ffe, 0x000000007fffffff, 0x0000000000007fff );

  TEST_RRRR_OP( 12, add, 0x0, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff );
  TEST_RRRR_OP( 13, add, 0x1, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000001 );
  TEST_RRRR_OP( 14, add, 0x1, 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff );
  
  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------


  TEST_RRRR_SRC1_EQ_DEST1( 15, add, 0, 24, 13, 11 );
  TEST_RRRR_SRC1_EQ_DEST2( 16, add, 0, 25, 14, 11 );
  TEST_RRRR_SRC2_EQ_DEST1( 17, add, 0, 26, 15, 11 );
  TEST_RRRR_SRC2_EQ_DEST2( 18, add, 0, 27, 16, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 19, add, 0, 28, 14 );
  TEST_RRRR_SRC12_EQ_DEST2( 20, add, 0, 30, 15 );
  
  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 21, 0,add, 0, 30, 19, 11 );
  TEST_RRRR_DEST_BYPASS( 22, 1,add, 0, 31, 20, 11 );
  TEST_RRRR_DEST_BYPASS( 23, 2,add, 0, 32, 21, 11 );

  TEST_RRRR_ZEROSR1(  24, add, 0, 33, 33 );
  TEST_RRRR_ZEROSR2(  25, add, 0, 34, 34 );
  TEST_RRRR_ZEROSR12( 26, add, 0, 0 );

  TEST_RRRR_ZERODEST1(35, add, 8, 4, 4);
  TEST_RRRR_ZERODEST2(36, add, 0, 5, 5);

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_ORRR_OP_BBD( 27, add, 0x0, 0x0, 0x0 );
  TEST_ORRR_OP_BBD( 28, add, 0x3, 0x1, 0x2 );

  TEST_ORRR_OP_BBD( 29, add, 0xffffffffffffffff, 0xffffffff00000000, 0x00000000ffffffff );
  TEST_ORRR_OP_BBD( 30, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000 );
  TEST_ORRR_OP_BBD( 31, add, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f );


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_ORRR_SRC1_EQ_DEST_BBD( 32, add, 24, 13, 11 );
  TEST_ORRR_SRC1_EQ_DEST_BBD( 33, add, 0xffffffffffffffff, 0xffffffff00000000, 0x00000000ffffffff );

  TEST_ORRR_ZEROSRC2_BBD( 34, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0 );


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

