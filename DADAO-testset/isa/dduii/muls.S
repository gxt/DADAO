#*****************************************************************************
# muls.S
#-----------------------------------------------------------------------------
#
# Test muls instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, muls, 0x0, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000 );
  TEST_RRRR_OP( 3, muls, 0x0, 0x0000000000000001, 0x0000000000000001, 0x0000000000000001 );
  TEST_RRRR_OP( 4, muls, 0x0, 0x0000000000000015, 0x0000000000000003, 0x0000000000000007 );

  
  TEST_RRRR_OP( 5, muls, 0x0, 0x0000000000000000, 0x0000000000000000, 0xffffffffffff8000 );
  TEST_RRRR_OP( 6, muls, 0x0, 0x0000000000000000, 0xffffffff80000000, 0x00000000 );
  TEST_RRRR_OP( 7, muls, 0x0, 0x0000400000000000, 0xffffffff80000000, 0xffffffffffff8000 );

  TEST_RRRR_OP( 8, muls, 0xffffffffffffffff, 0xfffffffffffffffe, 0x0000000000000002, 0xffffffffffffffff );
  TEST_RRRR_OP( 9, muls, 0xffffffffffff0081, 0x000000000000ff7f, 0x000000000002fe7d, 0xaaaaaaaaaaaaaaab );


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRRR_SRC1_EQ_DEST1( 10, muls, 0, 24, 3, 8 );
  TEST_RRRR_SRC1_EQ_DEST2( 11, muls, 0, 27, 3, 9 );
  TEST_RRRR_SRC2_EQ_DEST1( 12, muls, 0, 30, 3, 10 );
  TEST_RRRR_SRC2_EQ_DEST2( 13, muls, 0, 33, 3, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 14, muls, 0, 25, 5 );
  TEST_RRRR_SRC12_EQ_DEST2( 15, muls, 0, 36, 6 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 16, 0,muls, 0, 30, 3, 10 );
  TEST_RRRR_DEST_BYPASS( 17, 1,muls, 0, 33, 3, 11 );
  TEST_RRRR_DEST_BYPASS( 18, 2,muls, 0, 36, 3, 12 );

  TEST_RRRR_ZEROSR1(  19, muls, 0, 0, 33 );
  TEST_RRRR_ZEROSR2(  20, muls, 0, 0, 34 );
  TEST_RRRR_ZEROSR12( 21, muls, 0, 0 );
  TEST_RRRR_ZERODEST1(22, muls, 39,3, 13 );
  TEST_RRRR_ZERODEST2(23, muls, 0, 3, 14 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END



