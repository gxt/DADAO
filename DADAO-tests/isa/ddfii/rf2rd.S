#*****************************************************************************
# rf2rd.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRI_INIT_REG ( rf )
  TEST_CASE_MULTI_REG( 2, rf2rd, rd1, rf1, 60, rd, 0x1, cmps, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 3, rf2rd, rd1, rf2, 60, rd, 0x2, cmps, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 4, rf2rd, rd48, rf32, 12, rd, 0x20, cmps, 48,49,50,51,52,53,54,55,56,57,58,59,60 )
  TEST_CASE_MULTI_REG( 5, rf2rd, rd33, rf3, 28, rd, 0x3, cmps, 33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 6, rf2rd, rd3, rf36, 16, rd, 0x24, cmps, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 )
  TEST_CASE_MULTI_REG( 7, rf2rd, rd32, rf16, 16, rd, 0x10, cmps, 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48 )
  TEST_CASE_MULTI_REG( 8, rf2rd, rd15, rf32, 16, rd, 0x20, cmps, 15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 )
  TEST_CASE_MULTI_REG( 9, rf2rd, rd0, rf1, 0, rd, 0x0, cmps, 0 )
  TEST_CASE_MULTI_REG( 10, rf2rd, rd1, rf0, 0, rd, 0x47efffffe0000000, cmps, 1 )

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
