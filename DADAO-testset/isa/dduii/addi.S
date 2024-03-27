#*****************************************************************************
# addi.S
#-----------------------------------------------------------------------------
#
# Test addi instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRII_OP( 2,  addi, 0x00000000, 0x00000000, 0x000 );
  TEST_RRII_OP( 3,  addi, 0x00000002, 0x00000001, 0x001 );
  TEST_RRII_OP( 4,  addi, 0x0000000a, 0x00000003, 0x007 );

  TEST_RRII_OP( 5,  addi, 0xffffffffffffffff, 0x0000000000000000, -1 );
  TEST_RRII_OP( 6,  addi, 0xffffffff80000000, 0xffffffff80000000, 0x000 );
  TEST_RRII_OP( 7,  addi, 0xffffffff7ffff800, 0xffffffff80000000, -0x800 );

  TEST_RRII_OP( 8,  addi, 0x00000000000007ff, 0x00000000, 0x7ff );
  TEST_RRII_OP( 9,  addi, 0x000000007fffffff, 0x7fffffff, 0x000 );
  TEST_RRII_OP( 10, addi, 0x00000000800007fe, 0x7fffffff, 0x7ff );

  TEST_RRII_OP( 11, addi, 0xffffffff800007ff, 0xffffffff80000000, 0x7ff );
  TEST_RRII_OP( 12, addi, 0x000000007ffff7ff, 0x000000007fffffff, -0x800 );

  TEST_RRII_OP( 13, addi, 0xffffffffffffffff, 0x0000000000000000, -1 );
  TEST_RRII_OP( 14, addi, 0x0000000000000000, 0xffffffffffffffff, 0x001 );
  TEST_RRII_OP( 15, addi, 0xfffffffffffffffe, 0xffffffffffffffff, -1 );

  TEST_RRII_OP( 16, addi, 0x0000000080000000, 0x7fffffff, 0x001 );

  TEST_RRII_OP_RB( 17,  addi, 0x00000000, 0x00000000, 0x000 );
  TEST_RRII_OP_RB( 18,  addi, 0x00000002, 0x00000001, 0x001 );
  TEST_RRII_OP_RB( 19,  addi, 0x0000000a, 0x00000003, 0x007 );

  TEST_RRII_OP_RB( 20,  addi, 0xffffffffffffffff, 0x0000000000000000, -1 );
  TEST_RRII_OP_RB( 21,  addi, 0xffffffff80000000, 0xffffffff80000000, 0x000 );
  TEST_RRII_OP_RB( 22,  addi, 0xffffffff7ffff800, 0xffffffff80000000, -0x800 );

  TEST_RRII_OP_RB( 23,  addi, 0x00000000000007ff, 0x00000000, 0x7ff );
  TEST_RRII_OP_RB( 24,  addi, 0x000000007fffffff, 0x7fffffff, 0x000 );
  TEST_RRII_OP_RB( 25, addi, 0x00000000800007fe, 0x7fffffff, 0x7ff );

  TEST_RRII_OP_RB( 26, addi, 0xffffffff800007ff, 0xffffffff80000000, 0x7ff );
  TEST_RRII_OP_RB( 27, addi, 0x000000007ffff7ff, 0x000000007fffffff, -0x800 );

  TEST_RRII_OP_RB( 28, addi, 0xffffffffffffffff, 0x0000000000000000, -1 );
  TEST_RRII_OP_RB( 29, addi, 0x0000000000000000, 0xffffffffffffffff, 0x001 );
  TEST_RRII_OP_RB( 30, addi, 0xfffffffffffffffe, 0xffffffffffffffff, -1 );

  TEST_RRII_OP_RB( 31, addi, 0x0000000080000000, 0x7fffffff, 0x001 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRII_SRC1_EQ_DEST( 32, addi, 9, 5, 4);
  TEST_RRII_SRC1_EQ_DEST_RB( 33, addi, 10, 5, 5);  

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRII_DEST_BYPASS( 34, 0, addi, 24, 13, 11 );
  TEST_RRII_DEST_BYPASS( 35, 1, addi, 23, 13, 10 );
  TEST_RRII_DEST_BYPASS( 36, 2, addi, 22, 13,  9 );

  TEST_RRII_SRC1_BYPASS( 37, 0, addi, 24, 13, 11 );
  TEST_RRII_SRC1_BYPASS( 38, 1, addi, 23, 13, 10 );
  TEST_RRII_SRC1_BYPASS( 39, 2, addi, 22, 13,  9 );

  TEST_RRII_ZEROSRC1( 40, addi, 32, 32 );
  TEST_RRII_ZERODEST( 41, addi, 33, 33 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
  
