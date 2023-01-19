#*****************************************************************************
# adrp.S
#-----------------------------------------------------------------------------
#
# Test adrp instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RIII_OP( 2, adrp, 0x400000, 0x0, 0);
  TEST_RIII_OP( 3, adrp, 0x405000, 0x0, 5);
  TEST_RIII_OP( 4, adrp, 0x10400000, 0xfffff, 0x10000);
  TEST_RIII_OP( 5, adrp, 0x3fb000, 0xffffffffffffffffff,-5);

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

 
