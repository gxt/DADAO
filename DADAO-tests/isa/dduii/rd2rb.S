#*****************************************************************************
# rd2rb.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRI_INIT_REG ( rd )
  TEST_CASE_MULTI_REG( 2, rd2rb, rb1, rd1, 61, rb, 0x1, cmp, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 3, rd2rb, rb1, rd2, 61, rb, 0x2, cmp, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 4, rd2rb, rb48, rd32, 13, rb, 0x20, cmp, 48,49,50,51,52,53,54,55,56,57,58,59,60 )
  TEST_CASE_MULTI_REG( 5, rd2rb, rb33, rd3, 29, rb, 0x3, cmp, 33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61 )
  TEST_CASE_MULTI_REG( 6, rd2rb, rb3, rd36, 17, rb, 0x24, cmp, 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 )
  TEST_CASE_MULTI_REG( 7, rd2rb, rb1, rd0, 1, rb, 0x0, cmp, 1 )

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
