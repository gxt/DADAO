#*****************************************************************************
# divu.S
#-----------------------------------------------------------------------------
#
# Test divu instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, divu,  2,  3,  20,  6);

  TEST_RRRR_OP( 3, divu, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff);
  TEST_RRRR_OP( 4, divu, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff);


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRRR_SRC1_EQ_DEST1( 5, divu, 0, 3, 24, 8 );
  TEST_RRRR_SRC1_EQ_DEST2( 6, divu, 0, 3, 27, 9 );
  TEST_RRRR_SRC2_EQ_DEST1( 7, divu, 0, 3, 30, 10 );
  TEST_RRRR_SRC2_EQ_DEST2( 8, divu, 0, 3, 33, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 9, divu, 0, 1, 25 );
  TEST_RRRR_SRC12_EQ_DEST2( 10, divu, 0, 1, 36 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 11, 0,divu, 0, 3, 30, 10 );
  TEST_RRRR_DEST_BYPASS( 12, 1,divu, 0, 3, 33, 11 );
  TEST_RRRR_DEST_BYPASS( 13, 2,divu, 0, 3, 36, 12 );

  TEST_RRRR_ZEROSR1(  14, divu, 0, 0, 33 );
  TEST_RRRR_ZERODEST1(15, divu, 3,20, 6  );
  TEST_RRRR_ZERODEST2(16, divu, 2,26, 6  );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

