#*****************************************************************************
# shru.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRR_OP( 2, shru, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
  TEST_ORRR_OP( 3, shru, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
  TEST_ORRR_OP( 4, shru, 0x0000000000000001, 0xffffffffffffffff, 0x000000000000003f )
  TEST_ORRR_OP( 5, shru, 0x7fffffffffffffff, 0xffffffffffffffff, 0x0000000000000001 )
  TEST_ORRR_OP( 6, shru, 0x0000f0f0f0fff0f0, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
  TEST_ORRR_OP( 7, shru, 0x000000007f87f87f, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

  TEST_ORRR_SRC1_EQ_DEST( 8, shru, 0x0787878787878787, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
  TEST_ORRR_SRC1_EQ_DEST( 9, shru, 0x00000000ffffffff, 0xffffffff00000000, 0x0000000000000020 )

  TEST_ORRR_SRC2_EQ_DEST( 10, shru, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
  TEST_ORRR_SRC2_EQ_DEST( 11, shru, 0x000000000001e001, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

  TEST_ORRR_ZEROSRC1( 12, shru, 0x0000000000000000, 0x0000000000000031 )

  TEST_ORRR_ZEROSRC2( 13, shru, 0xffffffffffffffff, 0xffffffffffffffff )

  TEST_ORRI_OP( 14, shru, 0x0000000000000007, 0x000000000000000f, 1 )
  TEST_ORRI_OP( 15, shru, 0xff0f0f0f0f0f0fff, 0xff0f0f0f0f0f0fff, 0 )
  TEST_ORRI_OP( 16, shru, 0x0000000f000f000f, 0xf000f000f000f000, 28 )
  TEST_ORRI_OP( 17, shru, 0x0000000000000001, 0xf0f0f0fff0f0f0f0, 63 )
  TEST_ORRI_OP( 18, shru, 0x000000000000000f, 0xf0f0f0f0fff0000f, 60 )
  TEST_ORRI_OP( 19, shru, 0x00000000f0f0f0f0, 0xf0f0f0f0f0ffff00, 32 )
  TEST_ORRI_OP( 20, shru, 0x0000000000078787, 0xf0f0f0f0f0ffff00, 45 )

  TEST_ORRI_SRC_OP( 21, shru, 0x00000f00f0f0f0f0, 0xf00f0f0f0f0f0f0f, 20 )
  TEST_ORRI_SRC_OP( 22, shru, 0x0000000000000003, 0xf0f0f0f0fffff0ff, 62 )

  TEST_ORRI_ZERO_SRC( 23, shru, 0x0000000000000000, 3 )


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
