#include "dadao_test.h"
#include "test_macros.h"

#define __TEST_JUMP_TAIL__( tcname )		\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
	lbl_ ## tcname ## _target:				\
		brz		rd0, lbl_##tcname##_pass;	\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
		brz		rd0, ___fail;				\
	lbl_ ## tcname ## _pass:				\
		swym	0;							\

DDTEST_CODE_BEGIN

	__TEST_CASE_HEAD__( 101 )

		setrb	rb16, lbl_101_target
		jump	rb16, rd0, 0

	__TEST_JUMP_TAIL__( 101 )

	__TEST_CASE_HEAD__( 102 )

		setrb	rb16, lbl_102_target
		addi	rb16, rb16, 100
		setrd	rd16, -100
		jump	rb16, rd16, 0

	__TEST_JUMP_TAIL__( 102 )

	__TEST_CASE_HEAD__( 103 )

		setrb	rb16, lbl_103_target
		addi	rb16, rb16, -100
		setrd	rd16, 100
		jump	rb16, rd16, 0

	__TEST_JUMP_TAIL__( 103 )

	__TEST_CASE_HEAD__( 104 )

		setrb	rb16, lbl_104_target
		addi	rb16, rb16, 100
		jump	rb16, rd0, -25

	__TEST_JUMP_TAIL__( 104 )

	__TEST_CASE_HEAD__( 105 )

		setrb	rb16, lbl_105_target
		addi	rb16, rb16, -100
		jump	rb16, rd0, 25

	__TEST_JUMP_TAIL__( 105 )

	__TEST_CASE_HEAD__( 106 )

		setrb	rb16, lbl_106_target
		setrd	rd16, -100
		jump	rb16, rd16, 25

	__TEST_JUMP_TAIL__( 106 )

	__TEST_CASE_HEAD__( 107 )

		setrb	rb16, lbl_107_target
		setrd	rd16, 100
		jump	rb16, rd16, -25

	__TEST_JUMP_TAIL__( 107 )

DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
 
