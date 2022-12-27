#*****************************************************************************
# setzw.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RWII_RF_W0( 2, setw, 0x0f0f0f0f0f0fffff, 0x0f0f0f0f0f0f0000, 0xffff );
  TEST_RWII_RF_W1( 3, setw, 0x0f0f0f0fffff0f0f, 0x0f0f0f0f00000f0f, 0xffff );
  TEST_RWII_RF_W2( 4, setw, 0x0f0fffff0f0f0f0f, 0x0f0f00000f0f0f0f, 0xffff );
  TEST_RWII_RF_W3( 5, setw, 0xffff0f0f0f0f0f0f, 0x00000f0f0f0f0f0f, 0xffff );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
