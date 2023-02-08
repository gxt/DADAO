#*****************************************************************************
# csne.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_RRRR_RET3_OP( 2, csne, 0x00f0f0f0fff0000f, 0xffffffffffffffff, 0x0000000000000000, 0x00f0f0f0fff0000f );
  TEST_RRRR_RET3_OP( 3, csne, 0xf00f00f00f00f00f, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 0xf00f00f00f00f00f );
  TEST_RRRR_RET3_OP( 4, csne, 0x00f000f000f00fff, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 0x00f000f000f00fff );
  TEST_RRRR_RET3_OP( 5, csne, 0xf00f0f00f0ff00f0, 0xfffffffffffffff0, 0x000000000000000f, 0xf00f0f00f0ff00f0 );
  TEST_RRRR_RET3_OP( 6, csne, 0x0ff0ff0fff0fffff, 0x00000000000000ff, 0x000000000000000f, 0x0ff0ff0fff0fffff );

  TEST_RRRR_RET3_OP( 7, csne, 0x0ff0ff0fff0fffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0 );
  TEST_RRRR_RET3_OP( 8, csne, 0x0ff0ff0fff0fffff, 0x0000000000000000, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f );
  TEST_RRRR_RET3_OP( 9, csne, 0x0ff0ff0fff0fffff, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 );
  TEST_RRRR_RET3_OP( 10, csne, 0x0ff0ff0fff0fffff, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff );
  TEST_RRRR_RET3_OP( 11, csne, 0x0ff0ff0fff0fffff, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 0xf00f0f000f00ffff );

  TEST_RRRR_RET3_ZEROSRC1( 12, csne, 0xff00ff00ff00ff00, 0xffffffffffffffff, 0xff00ff00ff00ff00 );
  TEST_RRRR_RET3_ZEROSRC1( 13, csne, 0xff00ff00ff00ff00, 0x0000000000000000, 0xfff0f0f0f0f0f0f0 );

  TEST_RRRR_RET3_ZEROSRC2( 14, csne, 0x00ff00ff00ff00ff, 0xffffffffffffffff, 0x00ff00ff00ff00ff );
  TEST_RRRR_RET3_ZEROSRC2( 15, csne, 0x00ff00ff00ff00ff, 0x0000000000000000, 0xffff000f00ffffff );

  TEST_RRRR_RET3_ZEROSRC3( 16, csne, 0x00ff00ff00ff00ff, 0x00f0ff0f0f0f0fff, 0x00f0ff0f0f0f0fff );
  TEST_RRRR_RET3_ZEROSRC3( 17, csne, 0x0000000000000000, 0x000f0f0ffff00000, 0x000f0f0ffff000f0 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
