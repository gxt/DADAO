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

  TEST_RRRR_OP( 5, add, 0xffffffffffffffff, 0xffffffffffff8000, 0x0000000000000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 6, add, 0xffffffffffffffff, 0xffffffff80000000, 0xffffffff80000000, 0x0000000000000000 );
  TEST_RRRR_OP( 7, add, 0xffffffffffffffff, 0xffffffff7fff8000, 0xffffffff80000000, 0xffffffffffff8000  );


  TEST_RRRR_OP( 8, add, 0x0000000000000000, 0x0000000000007fff, 0x0000000000000000, 0x0000000000007fff );
  TEST_RRRR_OP( 9, add, 0x0000000000000000, 0x000000007fffffff, 0x000000007fffffff, 0x0000000000000000 );
  TEST_RRRR_OP(10, add, 0x0000000000000000, 0x0000000080007ffe, 0x000000007fffffff, 0x0000000000007fff );

  TEST_RRRR_OP(11, add, 0xffffffffffffffff, 0xffffffff80007fff, 0xffffffff80000000, 0x0000000000007fff );
  TEST_RRRR_OP(12, add, 0x0000000000000000, 0x000000007fff7fff, 0x000000007fffffff, 0xffffffffffff8000 );

  TEST_RRRR_OP(13, add, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff );
  TEST_RRRR_OP(14, add, 0x0000000000000000, 0x0000000000000000, 0xffffffffffffffff, 0x0000000000000001 );
  TEST_RRRR_OP(15, add, 0xffffffffffffffff, 0xfffffffffffffffe, 0xffffffffffffffff, 0xffffffffffffffff );

  TEST_RRRR_OP(16, add, 0x0000000000000000, 0x0000000080000000, 0x0000000000000001, 0x000000007fffffff );

  
  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------


  TEST_RRRR_SRC1_EQ_DEST1( 17, add, 0, 24, 13, 11 );
  TEST_RRRR_SRC1_EQ_DEST2( 18, add, 0, 25, 14, 11 );
  TEST_RRRR_SRC2_EQ_DEST1( 19, add, 0, 26, 15, 11 );
  TEST_RRRR_SRC2_EQ_DEST2( 20, add, 0, 27, 16, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 21, add, 0, 28, 14 );
  TEST_RRRR_SRC12_EQ_DEST2( 22, add, 0, 30, 15 );
  
  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 23, 0,add, 0, 30, 19, 11 );
  TEST_RRRR_DEST_BYPASS( 24, 1,add, 0, 31, 20, 11 );
  TEST_RRRR_DEST_BYPASS( 25, 2,add, 0, 32, 21, 11 );

  TEST_RRRR_ZEROSR1(  26, add, 0, 33, 33 );
  TEST_RRRR_ZEROSR2(  27, add, 0, 34, 34 );
  TEST_RRRR_ZEROSR12( 28, add, 0, 0 );

  TEST_RRRR_ZERODEST1(29, add, 8, 4, 4);
  TEST_RRRR_ZERODEST2(30, add, 0, 5, 5);

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

