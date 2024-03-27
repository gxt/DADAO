#*****************************************************************************
# csz.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_RRRR_RET1_OP( 2, csz, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0x0f0f0f0f0f0f0f0f, 0xf0f0f0f0f0f0f0f0 );
  TEST_RRRR_RET1_OP( 3, csz, 0xf00f0f00f0f00f00, 0x7fffffffffffffff, 0x0ff0f0ff0f0f0fff, 0xf00f0f00f0f00f00 );
  TEST_RRRR_RET1_OP( 4, csz, 0x0000f0ff0000f0ff, 0x0000000000000000, 0x0000f0ff0000f0ff, 0x00ff0ff000ff0ff0 );
  TEST_RRRR_RET1_OP( 5, csz, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000, 0xf0f0f0f0f0f0f0f0, 0xf0f0ffff00f0f0ff );
  TEST_RRRR_RET1_OP( 6, csz, 0x00f0f0ffffff0000, 0x0000000f00000000, 0xf0f0fffff0f0f0f0, 0x00f0f0ffffff0000 );

  TEST_RRRR_RET1_SRC1_EQ_DEST( 7, csz, 0xffffffffffffffff, 0x0000000000000000, 0xffffffffffffffff, 0xff00ff0000ff00ff );
  TEST_RRRR_RET1_SRC1_EQ_DEST( 8, csz, 0xffffffffffffff00, 0xf0ff0ff00fff00ff, 0x0f00ff0ff0fff0ff, 0xffffffffffffff00 );

  TEST_RRRR_RET1_SRC2_EQ_DEST( 9, csz, 0xf0f0f0f0f0f0f0f0, 0xff00000000000000, 0xfff000fffff000ff, 0xf0f0f0f0f0f0f0f0 );
  TEST_RRRR_RET1_SRC2_EQ_DEST( 10, csz, 0x00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000ffffffff0000 );

  TEST_RRRR_RET1_SRC3_EQ_DEST( 11, csz, 0x0f000fff0000ffff, 0x0000000000000000, 0x0f000fff0000ffff, 0xf0f0f0f0f0f0f0f0 );
  TEST_RRRR_RET1_SRC3_EQ_DEST( 12, csz, 0x00f0f0ff0f00f0ff, 0x0fffffffffffffff, 0x000fff0ff0f0f0ff, 0x00f0f0ff0f00f0ff );

  TEST_RRRR_RET1_ZEROSRC1( 13, csz, 0x0000000000000000, 0x0000000000000000, 0xfff0f0f00f0f0f0f)
  TEST_RRRR_RET1_ZEROSRC2( 14, csz, 0x0000000000000000, 0x0000000000000000, 0xfff0f0f0ffffffff)
  TEST_RRRR_RET1_ZEROSRC3( 15, csz, 0x0000000000000000, 0x000000000000000f, 0xffffffffffffffff)


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
