#*****************************************************************************
# sub.S
#-----------------------------------------------------------------------------
#
# Test sub instruction.
#

#include "dadao_test.h"
#include "test_macros.h"

DDTEST_DDUII
DDTEST_CODE_BEGIN

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_RRRR_OP( 2, sub, 0x0, 0x0, 0x0, 0x0 );
  TEST_RRRR_OP( 3, sub, 0x0, 0x1, 0x2, 0x1 );
  TEST_RRRR_OP( 4, sub, 0x0, 0x0, 0x1, 0x1 );

  TEST_RRRR_OP( 5, sub, 0xffffffffffffffff, 0x0000000000008000, 0x0000000000000000, 0xffffffffffff8000);
  TEST_RRRR_OP( 6, sub, 0x0000000000000000, 0xffffffff80000000, 0xffffffff80000000, 0x0000000000000000 );
  TEST_RRRR_OP( 7, sub, 0x0000000000000000, 0x000000007fffffff, 0x000000007fffffff, 0x0000000000000000 );

  TEST_RRRR_OP( 8, sub, 0x0, 0x0000000000000000, 0xffffffffffffffff, 0xffffffffffffffff );
  TEST_RRRR_OP( 9, sub, 0x0, 0xfffffffffffffffe, 0xffffffffffffffff, 0x0000000000000001 );

  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_RRRR_SRC1_EQ_DEST1( 15, sub, 0, 13, 24, 11 );
  TEST_RRRR_SRC1_EQ_DEST2( 16, sub, 0, 14, 25, 11 );
  TEST_RRRR_SRC2_EQ_DEST1( 17, sub, 0, 15, 26, 11 );
  TEST_RRRR_SRC2_EQ_DEST2( 18, sub, 0, 16, 27, 11 );

  TEST_RRRR_SRC12_EQ_DEST1( 19, sub, 0, 0, 14 );
  TEST_RRRR_SRC12_EQ_DEST2( 20, sub, 0, 0, 15 );

  #-------------------------------------------------------------
  # Bypassing tests
  #-------------------------------------------------------------

  TEST_RRRR_DEST_BYPASS( 21, 0, sub, 0, 19, 30, 11 );
  TEST_RRRR_DEST_BYPASS( 22, 1, sub, 0, 20, 31, 11 );
  TEST_RRRR_DEST_BYPASS( 23, 2, sub, 0, 21, 32, 11 );

  TEST_RRRR_ZEROSR1(  24, sub, 0xffffffffffffffff, 0xffffffffffffffff, 1 );
  TEST_RRRR_ZEROSR2(  25, sub, 0, 34, 34 );
  TEST_RRRR_ZEROSR12( 26, sub, 0 , 0 );

  TEST_RRRR_ZERODEST1(35, sub, 4, 8, 4);
  TEST_RRRR_ZERODEST2(36, sub, 0, 5, 5); 

  #-------------------------------------------------------------
  # Logical tests
  #-------------------------------------------------------------

  TEST_ORRR_OP_BBD( 27, sub, 0x0, 0x0, 0x0 );
  TEST_ORRR_OP_BBD( 28, sub, 0x1, 0x3, 0x2 );

  TEST_ORRR_OP_BBD( 29, sub, 0xfffffffe00000001, 0xffffffff00000000, 0x00000000ffffffff );
  TEST_ORRR_OP_BBD( 30, sub, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0, 0x0000000000000000 );
  TEST_ORRR_OP_BBD( 31, sub, 0x0f0f0f0f0f0f0f0f, 0x0f0f0f0f0f0f0f0f, 0x0000000000000000 );


  #-------------------------------------------------------------
  # Source/Destination tests
  #-------------------------------------------------------------

  TEST_ORRR_SRC1_EQ_DEST_BBD( 32, sub, 13, 24, 11 );
  TEST_ORRR_SRC1_EQ_DEST_BBD( 33, sub, 0xfffffffe00000001, 0xffffffff00000000, 0x00000000ffffffff );

  TEST_ORRR_ZEROSRC2_BBD( 34, add, 0xf0f0f0f0f0f0f0f0, 0xf0f0f0f0f0f0f0f0 );
	

  TEST_PASSFAIL

DDTEST_CODE_END

  .data
DDTEST_DATA_BEGIN

  TEST_DATA

DDTEST_DATA_END

 

