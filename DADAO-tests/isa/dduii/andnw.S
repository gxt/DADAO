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

  TEST_RWII_RD( 2, andnw, 0xffff00000000f000, 0xffff00000000f0f0, 0x00ff, w0 );
  TEST_RWII_RD( 3, andnw, 0xffff0000f0000000, 0xffff0000f0f00000, 0x00ff, w1 );
  TEST_RWII_RD( 4, andnw, 0x0000f0000000ffff, 0x0000f0f00000ffff, 0x00ff, w2 );
  TEST_RWII_RD( 5, andnw, 0xf00000000000ffff, 0xf0f000000000ffff, 0x00ff, w3 );
  TEST_RWII_RB( 6, andnw, 0xffff00000000f000, 0xffff00000000f0f0, 0x00ff, w0 );
  TEST_RWII_RB( 7, andnw, 0xffff0000f0000000, 0xffff0000f0f00000, 0x00ff, w1 );
  TEST_RWII_RB( 8, andnw, 0x0000f0000000ffff, 0x0000f0f00000ffff, 0x00ff, w2 );
  TEST_RWII_RB( 9, andnw, 0xf00000000000ffff, 0xf0f000000000ffff, 0x00ff, w3 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
