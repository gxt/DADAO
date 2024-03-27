#*****************************************************************************
# divs.S
#-----------------------------------------------------------------------------
#
# Test divs instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, divs,  2,  3,  20,  6);
  TEST_RRRR_OP( 3, divs, -2, -3, -20,  6);
  TEST_RRRR_OP( 4, divs,  2, -3,  20, -6);
  TEST_RRRR_OP( 5, divs, -2,  3, -20, -6);

  TEST_RRRR_OP( 6, divs, 0, 1, 0xffffffffffffffff, 0xffffffffffffffff);
  TEST_RRRR_OP( 7, divs, 0, 1, 0x7fffffffffffffff, 0x7fffffffffffffff);


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRRR_SRC1_EQ_DEST1( 8, divs, 0, 3, 24, 8 );
  TEST_RRRR_SRC1_EQ_DEST2( 9, divs, 0, 3, 27, 9 );
  TEST_RRRR_SRC2_EQ_DEST1( 10, divs, 0, 3, 30, 10 );
  TEST_RRRR_SRC2_EQ_DEST2( 11, divs, 0, 3, 33, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 12, divs, 0, 1, 25 );
  TEST_RRRR_SRC12_EQ_DEST2( 13, divs, 0, 1, 36 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 14, 0,divs, 0, 3, 30, 10 );
  TEST_RRRR_DEST_BYPASS( 15, 1,divs, 0, 3, 33, 11 );
  TEST_RRRR_DEST_BYPASS( 16, 2,divs, 0, 3, 36, 12 );

  TEST_RRRR_ZEROSR1(  17, divs, 0, 0, 33 );
  TEST_RRRR_ZERODEST1(18, divs, 3,20, 6  );
  TEST_RRRR_ZERODEST2(19, divs, 2,26, 6  ); 

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END



