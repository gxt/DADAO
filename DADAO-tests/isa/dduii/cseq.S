#*****************************************************************************
# cseq.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_RRRR_RET3_OP( 2, cseq, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0 );
  TEST_RRRR_RET3_OP( 3, cseq, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f );
  TEST_RRRR_RET3_OP( 4, cseq, 0x0000000000000000, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 );
  TEST_RRRR_RET3_OP( 5, cseq, 0xffffffffffffffff, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0xffffffffffffffff );
  TEST_RRRR_RET3_OP( 6, cseq, 0xf00f0f000f00ffff, 0x0ff0fff0f0f0f0f0, 0x0ff0fff0f0f0f0f0, 0xf00f0f000f00ffff );

  TEST_RRRR_RET3_OP( 7, cseq, 0xf00f0f000f00ffff, 0xffffffffffffffff, 0x0000000000000000, 0x00f0f0f0fff0000f );
  TEST_RRRR_RET3_OP( 8, cseq, 0xf00f0f000f00ffff, 0xff0f0f0f0ffff000, 0x00f00f0f0fffff00, 0xf00f00f00f00f00f );
  TEST_RRRR_RET3_OP( 9, cseq, 0xf00f0f000f00ffff, 0x0ff0fff0f0f0f0f0, 0xf0f0f0ff0000fff0, 0x00f000f000f00fff );
  TEST_RRRR_RET3_OP( 10, cseq, 0xf00f0f000f00ffff, 0xfffffffffffffff0, 0x000000000000000f, 0xf00f0f00f0ff00f0 );

  TEST_RRRR_RET3_ZEROSRC1( 11, cseq, 0xf00f0f000f00ffff, 0xffffffffffffffff, 0x0000000000000000 );
  TEST_RRRR_RET3_ZEROSRC1( 12, cseq, 0xfff0f0f0f0f0f0f0, 0x0000000000000000, 0xfff0f0f0f0f0f0f0 );

  TEST_RRRR_RET3_ZEROSRC2( 13, cseq, 0xfff0f0f0f0f0f0f0, 0xffffffffffffffff, 0x0000000000000000 );
  TEST_RRRR_RET3_ZEROSRC2( 14, cseq, 0xffff000f00ffffff, 0x0000000000000000, 0xffff000f00ffffff );

  TEST_RRRR_RET3_ZEROSRC3( 15, cseq, 0xffff000f00ffffff, 0x00f0ff0f0f0f0fff, 0xf00f000f00ffffff );
  TEST_RRRR_RET3_ZEROSRC3( 16, cseq, 0x0000000000000000, 0x000f0f0ffff00000, 0x000f0f0ffff00000 );

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
