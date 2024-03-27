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

  TEST_RWII_RD( 2, setzw, 0x000000000000ffff, 0xffffffffffff0000, 0xffff, w0 );
  TEST_RWII_RD( 3, setzw, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff, w1 );
  TEST_RWII_RD( 4, setzw, 0x0000ffff00000000, 0xffff0000ffffffff, 0xffff, w2 );
  TEST_RWII_RD( 5, setzw, 0xffff000000000000, 0x0000ffffffffffff, 0xffff, w3 );
  TEST_RWII_RB( 6, setzw, 0x000000000000ffff, 0xffffffffffff0000, 0xffff, w0 );
  TEST_RWII_RB( 7, setzw, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff, w1 );
  TEST_RWII_RB( 8, setzw, 0x0000ffff00000000, 0xffff0000ffff0000, 0xffff, w2 );
  TEST_RWII_RB( 9, setzw, 0xffff000000000000, 0x0000ffffffffffff, 0xffff, w3 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
