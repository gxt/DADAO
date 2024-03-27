#*****************************************************************************
# cmp.S
#-----------------------------------------------------------------------------
#
# Test cmp instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_ORRR_OP_RB( 2, cmp, 0x00000000, 0x00000000, 0x000 );
  TEST_ORRR_OP_RB( 3, cmp, 0x00000001, 0x00000005, 0x000 );
  TEST_ORRR_OP_RB( 4, cmp, -1, 0x00000000, 1);

  TEST_ORRR_OP_RB( 5, cmp, 0x0000000000000000, 0x00000000, 0x000 );
  TEST_ORRR_OP_RB( 6, cmp, 0x0000000000000001, 0xffffffffffffffff, 0x0000000000000000 );
  TEST_ORRR_OP_RB( 7, cmp, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff );
  TEST_ORRR_OP_RB( 8, cmp, 0x0000000000000001, 0xffffffffffffffff, 0xfffffffffffffffe );

  TEST_ORRR_SRC1_EQ_SRC2_RB( 9, cmp, 0x0, 0xffffffffffffffff );
  TEST_ORRR_ZERODEST_RB( 10, cmp, 0x0, 0xffffffffffffffff );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

