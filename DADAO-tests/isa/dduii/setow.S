#*****************************************************************************
# setow.S
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

  TEST_RWII_RD_W0( 2, setow, 0xffffffffffff0000, 0x000000000000ffff, 0x0 );
  TEST_RWII_RD_W1( 3, setow, 0xffffffff0000ffff, 0x00000000ffff0000, 0x0 );
  TEST_RWII_RD_W2( 4, setow, 0xffff0000ffffffff, 0x0000ffff00000000, 0x0 );
  TEST_RWII_RD_W3( 5, setow, 0x0000ffffffffffff, 0xffff000000000000, 0x0 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
