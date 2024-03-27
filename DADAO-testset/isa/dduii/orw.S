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

  TEST_RWII_RD( 2, orw, 0x000000000000ffff, 0x000000000000f0f0, 0x0f0f, w0 );
  TEST_RWII_RD( 3, orw, 0x00000000ffff0000, 0x00000000f0f00000, 0x0f0f, w1 );
  TEST_RWII_RD( 4, orw, 0x0000ffff00000000, 0x0000f0f000000000, 0x0f0f, w2 );
  TEST_RWII_RD( 5, orw, 0xffff000000000000, 0xf0f0000000000000, 0x0f0f, w3 );
  TEST_RWII_RB( 6, orw, 0x000000000000ffff, 0x000000000000f0f0, 0x0f0f, w0 );
  TEST_RWII_RB( 7, orw, 0x00000000ffff0000, 0x00000000f0f00000, 0x0f0f, w1 );
  TEST_RWII_RB( 8, orw, 0x0000ffff00000000, 0x0000f0f000000000, 0x0f0f, w2 );
  TEST_RWII_RB( 9, orw, 0xffff000000000000, 0xf0f0000000000000, 0x0f0f, w3 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
