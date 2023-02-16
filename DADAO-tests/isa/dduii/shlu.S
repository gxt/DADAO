#*****************************************************************************
# shlu.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRR_OP( 2, shlu, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
  TEST_ORRR_OP( 3, shlu, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )
  TEST_ORRR_OP( 4, shlu, 0x8000000000000000, 0xffffffffffffffff, 0x000000000000003f )
  TEST_ORRR_OP( 5, shlu, 0xfffffffffffffffe, 0xffffffffffffffff, 0x0000000000000001 )
  TEST_ORRR_OP( 6, shlu, 0xf0fff0f0f0f00000, 0xf0f0f0fff0f0f0f0, 0x0000000000000010 )
  TEST_ORRR_OP( 7, shlu, 0x1fe1fe1e00000000, 0xff0ff0ff0ff0ff0f, 0x0000000000000021 )

  TEST_ORRR_SRC1_EQ_DEST( 8, shlu, 0x1e1e1e1e1e1e1e1e, 0x0f0f0f0f0f0f0f0f, 0x0000000000000001 )
  TEST_ORRR_SRC1_EQ_DEST( 9, shlu, 0x0000000000000000, 0xffffffff00000000, 0x0000000000000020 )

  TEST_ORRR_SRC2_EQ_DEST( 10, shlu, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
  TEST_ORRR_SRC2_EQ_DEST( 11, shlu, 0x787f800000000000, 0xf000f0f0f0f0f0ff, 0x000000000000002f )

  TEST_ORRR_ZEROSRC1( 12, shlu, 0x0000000000000000, 0x0000000000000031 )

  TEST_ORRR_ZEROSRC2( 13, shlu, 0xffffffffffffffff, 0xffffffffffffffff )

  TEST_ORRI_OP( 14, shlu, 0x000000000000001e, 0x000000000000000f, 1 )
  TEST_ORRI_OP( 15, shlu, 0xff0f0f0f0f0f0fff, 0xff0f0f0f0f0f0fff, 0 )
  TEST_ORRI_OP( 16, shlu, 0x0f000f0000000000, 0xf000f000f000f000, 28 )
  TEST_ORRI_OP( 17, shlu, 0x0000000000000000, 0xf0f0f0fff0f0f0f0, 63 )
  TEST_ORRI_OP( 18, shlu, 0xf000000000000000, 0xf0f0f0f0fff0000f, 60 )
  TEST_ORRI_OP( 19, shlu, 0xf0ffff0000000000, 0xf0f0f0f0f0ffff00, 32 )
  TEST_ORRI_OP( 20, shlu, 0xffe0000000000000, 0xf0f0f0f0f0ffff00, 45 )

  TEST_ORRI_SRC_OP( 21, shlu, 0xf0f0f0f0f0f00000, 0xf00f0f0f0f0f0f0f, 20 )
  TEST_ORRI_SRC_OP( 22, shlu, 0xc000000000000000, 0xf0f0f0f0fffff0ff, 62 )

  TEST_ORRI_ZERO_SRC( 23, shlu, 0x0000000000000000, 3 )


  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
