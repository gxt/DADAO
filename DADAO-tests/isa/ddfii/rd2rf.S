#*****************************************************************************
# rd2rf.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRI_INIT_REG ( rd )
  rd2rf rf1, rd1, 62;
  rb2rd rd0, rb0, 63;
  TEST_CASE_MULTI_REG( 2, rf2rd, rd0, rf0, 61, rd, 0x0, cmps, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  rd2rf rf16, rd31, 30;
  rb2rd rd31, rb0, 30;
  TEST_CASE_MULTI_REG( 3, rf2rd, rd31, rf16, 30, rd, 0x1f, cmps, 31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  rd2rf rf32, rd31, 15;
  TEST_CASE_MULTI_REG( 4, rf2rd, rd32, rf32, 15, rd, 0x1f, cmps, 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47 )
  rd2rf rf48, rd3, 15;
  TEST_CASE_MULTI_REG( 5, rf2rd, rd4, rf48, 15, rd, 0x3, cmps, 4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 )
  rd2rf rf1, rd32, 15;
  TEST_CASE_MULTI_REG( 6, rf2rd, rd4, rf1, 15, rd, 0x1f, cmps, 4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 )
  rd2rf rf0, rd0, 0;
  TEST_CASE_MULTI_REG( 7, rf2rd, rd1, rf0, 0, rd, 0x47efffffe0000000, cmps, 1 )
  rd2rf rf1, rd0, 0;
  TEST_CASE_MULTI_REG( 8, rf2rd, rd1, rf1, 0, rd, 0x0, cmps, 1 )

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
