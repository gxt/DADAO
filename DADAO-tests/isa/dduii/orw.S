#*****************************************************************************
# orw.S
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

  TEST_RWII_RD_W0( 2, orw, 0x000000000000ffff, 0x000000000000f0f0, 0x0f0f );
  TEST_RWII_RD_W1( 3, orw, 0x00000000ffff0000, 0x00000000f0f00000, 0x0f0f );
  TEST_RWII_RD_W2( 4, orw, 0x0000ffff00000000, 0x0000f0f000000000, 0x0f0f );
  TEST_RWII_RD_W3( 5, orw, 0xffff000000000000, 0xf0f0000000000000, 0x0f0f );
  TEST_RWII_RB_W0( 6, orw, 0x000000000000ffff, 0x000000000000f0f0, 0x0f0f );
  TEST_RWII_RB_W1( 7, orw, 0x00000000ffff0000, 0x00000000f0f00000, 0x0f0f );
  TEST_RWII_RB_W2( 8, orw, 0x0000ffff00000000, 0x0000f0f000000000, 0x0f0f );
  TEST_RWII_RB_W3( 9, orw, 0xffff000000000000, 0xf0f0000000000000, 0x0f0f );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
