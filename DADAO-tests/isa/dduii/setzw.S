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

  TEST_RWII_RD_W0( 2, setzw, 0x000000000000ffff, 0xffffffffffff0000, 0xffff );
  TEST_RWII_RD_W1( 3, setzw, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff );
  TEST_RWII_RD_W2( 4, setzw, 0x0000ffff00000000, 0xffff0000ffffffff, 0xffff );
  TEST_RWII_RD_W3( 5, setzw, 0xffff000000000000, 0x0000ffffffffffff, 0xffff );
  TEST_RWII_RB_W0( 6, setzw, 0x000000000000ffff, 0xffffffffffff0000, 0xffff );
  TEST_RWII_RB_W1( 7, setzw, 0x00000000ffff0000, 0xffffffff0000ffff, 0xffff );
  TEST_RWII_RB_W2( 8, setzw, 0x0000ffff00000000, 0xffff0000ffff0000, 0xffff );
  TEST_RWII_RB_W3( 9, setzw, 0xffff000000000000, 0x0000ffffffffffff, 0xffff );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
