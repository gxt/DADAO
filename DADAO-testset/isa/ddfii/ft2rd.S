#*****************************************************************************
# ft2rd.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Round to Nearest, ties to Even
  #-------------------------------------------------------------
  setrf rf0, 0x0000000000000000;
  setrf rf1, 0x00000000bfc00000;
  setrf rf2, 0x00000000bfa00000;
  setrf rf3, 0x00000000007fffff;
  setrf rf4, 0x000000003fbfffff;
  setrf rf5, 0x000000003fffffff;
  TEST_CASE_MULTI_REG( 2, ft2rd, rd3, rf1, 5, rd, -2, cmps, 3,4,5,6,7 )

  #-------------------------------------------------------------
  # Round towards Zero
  #-------------------------------------------------------------
  setrf rf0, 0x0000000000010000;
  setrf rf1, 0x00000000c0300000;
  setrf rf2, 0x00000000bfa00000;
  setrf rf3, 0x0000000000000000;
  setrf rf4, 0x000000003f800000;
  setrf rf5, 0x0000000040300000;
  TEST_CASE_MULTI_REG( 3, ft2rd, rd8, rf1, 5, rd, -2, cmps, 8,9,10,11,12 )

  #-------------------------------------------------------------
  # Round Down
  #-------------------------------------------------------------
  setrf rf0, 0x0000000000020000;
  setrf rf1, 0x00000000bfd99998;
  setrf rf2, 0x00000000bdcccccc;
  setrf rf3, 0x000000003f7d70a3;
  setrf rf4, 0x000000003f800000;
  setrf rf5, 0x0000000040200000;
  TEST_CASE_MULTI_REG( 4, ft2rd, rd13, rf1, 5, rd, -2, cmps, 13,14,15,16,17 )

  #-------------------------------------------------------------
  # Round Up
  #-------------------------------------------------------------
  setrf rf0, 0x0000000000030000;
  setrf rf1, 0x00000000c0399999;
  setrf rf2, 0x00000000bfc00000;
  setrf rf3, 0x0000000000000000;
  setrf rf4, 0x000000003f000000;
  setrf rf5, 0x000000003f8ccccc;
  TEST_CASE_MULTI_REG( 5, ft2rd, rd18, rf1, 5, rd, -2, cmps, 18,19,20,21,22 )


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
