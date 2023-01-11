#*****************************************************************************
# andnw.S
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

  TEST_RWII_RD( 2, andnw, 0x000000000000f00f, 0x0000000000000fff, 0xfff0, w0 );
  TEST_RWII_RD( 3, andnw, 0x00000000f00f0000, 0x000000000fff0000, 0xfff0, w1 );
  TEST_RWII_RD( 4, andnw, 0x0000f00f00000000, 0x00000fff00000000, 0xfff0, w2 );
  TEST_RWII_RD( 5, andnw, 0xf00f000000000000, 0x0fff000000000000, 0xfff0, w3 );
  TEST_RWII_RB( 6, andnw, 0x000000000000f00f, 0x0000000000000fff, 0xfff0, w0 );
  TEST_RWII_RB( 7, andnw, 0x00000000f00f0000, 0x000000000fff0000, 0xfff0, w1 );
  TEST_RWII_RB( 8, andnw, 0x0000f00f00000000, 0x00000fff00000000, 0xfff0, w2 );
  TEST_RWII_RB( 9, andnw, 0xf00f000000000000, 0x0fff000000000000, 0xfff0, w3 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
