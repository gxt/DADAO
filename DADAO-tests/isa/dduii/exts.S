#*****************************************************************************
# exts.S
#-----------------------------------------------------------------------------
#
# Test and instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  TEST_ORRR_OP( 2, exts, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 )
  TEST_ORRR_OP( 3, exts, 0xffffffffffffffff, 0xffffffffffffffff, 0x0000000000000000 )
  TEST_ORRR_OP( 4, exts, 0xfffff0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000010 )
  TEST_ORRR_OP( 5, exts, 0x000000000ff00000, 0xf00f0f0f0ff00000, 0x0000000000000020 )
  TEST_ORRR_OP( 6, exts, 0x00000f0f00f000f0, 0xf0000f0f00f000f0, 0x0000000000000012 )
  TEST_ORRR_OP( 7, exts, 0xffffffffffffffff, 0x0000000000000001, 0x000000000000003f )
  TEST_ORRR_OP( 8, exts, 0x0000000000000000, 0x0000000000000000, 0x000000000000003f )

  TEST_ORRR_SRC1_EQ_DEST( 9, exts, 0xfff0f0f0f0f0f0ff, 0xfff0f0f0f0f0f0ff, 0x0000000000000003 )
  TEST_ORRR_SRC1_EQ_DEST( 10, exts, 0x0000f0f0f000ffff, 0x00f0f0f0f000ffff, 0x000000000000000f )

  TEST_ORRR_SRC2_EQ_DEST( 11, exts, 0x0000000000000000, 0xf0f0f0f0000f0000, 0x000000000000003f )
  TEST_ORRR_SRC2_EQ_DEST( 12, exts, 0xffffffffffffffff, 0x00f0f0f0f000ffff, 0x0000000000000030 )

  TEST_ORRR_ZEROSRC1( 13, exts, 0x0000000000000000, 0x0000000000000025 )
  TEST_ORRR_ZEROSRC2( 14, exts, 0x0f0f0f0f0000f0f0, 0x0f0f0f0f0000f0f0 )

  TEST_ORRI_OP( 15, exts, 0xfff0f0f0f0f0000f, 0xfff0f0f0f0f0000f, 0 )
  TEST_ORRI_OP( 16, exts, 0xffffffffffffffff, 0xfff0f0f0f0f0000f, 63 )
  TEST_ORRI_OP( 17, exts, 0x000000000f0000ff, 0xff0f0f0f0f0000ff, 32 )
  TEST_ORRI_OP( 18, exts, 0x0000000000000000, 0x0000000000000000, 63 )
  TEST_ORRI_OP( 19, exts, 0xffffffffffffff00, 0xfff0f0f0f0ffff00, 47 )
  TEST_ORRI_OP( 20, exts, 0xffffffffffffffff, 0xfff0000f0f0f0f0f, 60 )

  TEST_ORRI_SRC_OP( 21, exts, 0xfff0f000f0f0ffff, 0xfff0f000f0f0ffff, 3 )
  TEST_ORRI_SRC_OP( 22, exts, 0x00000000000000f0, 0x0000f0f0f00000f0, 48 )

  TEST_ORRI_ZERO_SRC( 23, exts, 0x0000000000000000, 31 )

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END
