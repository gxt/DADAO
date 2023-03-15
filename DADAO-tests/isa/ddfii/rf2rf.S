#*****************************************************************************
# rf2rf.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRI_INIT_REG ( rf )
  rd2rf rf0, rd0, 0;
  rf2rf rf0, rf0, 61;
  TEST_CASE_MULTI_REG( 2, rf2rd, rd1, rf1, 61, rd, 0x1, cmps, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  rf2rf rf1, rf2, 61;
  TEST_CASE_MULTI_REG( 3, rf2rd, rd1, rf1, 60, rd, 0x2, cmps, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 4, rf2rd, rd3, rf62, 0, rd, 0x3f, cmps, 3 )
  rf2rf rf63, rf0, 0;
  TEST_CASE_MULTI_REG( 5, rf2rd, rd3, rf63, 0, rd, 0x0000000000000000, cmps, 3 )
  rf2rf rf1, rf17, 15;
  TEST_CASE_MULTI_REG( 6, rf2rd, rd3, rf1, 15, rd, 0x12, cmps, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 )
  rf2rf rf17, rf33, 15;
  TEST_CASE_MULTI_REG( 7, rf2rd, rd3, rf17, 15, rd, 0x22, cmps, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 )
  rf2rf rf33, rf49, 13;
  TEST_CASE_MULTI_REG( 8, rf2rd, rd3, rf33, 13, rd, 0x32, cmps, 3,4,5,6,7,8,9,10,11,12,13,14,15,16 )
  rf2rf rf0, rf1, 0;
  TEST_CASE_MULTI_REG( 9, rf2rd, rd3, rf0, 0, rd, 0x12, cmps, 3 )

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
