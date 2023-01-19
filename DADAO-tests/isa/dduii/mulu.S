#*****************************************************************************
# mulu.S
#-----------------------------------------------------------------------------
#
# Test mulu instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, mulu, 0x0, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 );
  TEST_RRRR_OP( 3, mulu, 0x0, 0x0000000000000001, 0x0000000000000001, 0x0000000000000001 );
  TEST_RRRR_OP( 4, mulu, 0x0, 0x0000000000000015, 0x0000000000000003, 0x0000000000000007 );

  
  TEST_RRRR_OP( 5, mulu, 0x0, 0x0000000000000000, 0x0000000000000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 6, mulu, 0x0, 0x0000000000000000, 0xffffffff80000000, 0x00000000 );

  TEST_RRRR_OP( 7, mulu, 0xffffffff7fff8000, 0x0000400000000000, 0xffffffff80000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 8, mulu, 0x0000000000000001, 0xfffffffffffffffe, 0x0000000000000002, 0xffffffffffffffff );
  TEST_RRRR_OP( 9, mulu, 0x000000000001fefe, 0x000000000000ff7f, 0x000000000002fe7d, 0xaaaaaaaaaaaaaaab );


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRRR_SRC1_EQ_DEST1( 15, mulu, 0, 24, 3, 8 );
  TEST_RRRR_SRC1_EQ_DEST2( 16, mulu, 0, 27, 3, 9 );
  TEST_RRRR_SRC2_EQ_DEST1( 17, mulu, 0, 30, 3, 10 );
  TEST_RRRR_SRC2_EQ_DEST2( 18, mulu, 0, 33, 3, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 19, mulu, 0, 25, 5 );
  TEST_RRRR_SRC12_EQ_DEST2( 20, mulu, 0, 36, 6 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 21, 0,mulu, 0, 30, 3, 10 );
  TEST_RRRR_DEST_BYPASS( 22, 1,mulu, 0, 33, 3, 11 );
  TEST_RRRR_DEST_BYPASS( 23, 2,mulu, 0, 36, 3, 12 );

  TEST_RRRR_ZEROSR1(  24, mulu, 0, 0, 33 );
  TEST_RRRR_ZEROSR2(  25, mulu, 0, 0, 34 );
  TEST_RRRR_ZEROSR12( 26, mulu, 0, 0 );
  TEST_RRRR_ZERODEST1(27, mulu, 39,3, 13 );
  TEST_RRRR_ZERODEST2(28, mulu, 0, 3, 14 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END



