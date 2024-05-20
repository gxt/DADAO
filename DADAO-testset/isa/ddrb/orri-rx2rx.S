#include "dadao_test.h"
#include "test_macros.h"

DDTEST_CODE_BEGIN

	TEST_ORRI_M1_12( 101, rb2rb, 4444, 4444, rb, rb )
	TEST_ORRI_M1_12( 102, rb2rb, 8888, 8888, rb, rb )
	TEST_ORRI_M1_11( 103, rb2rb, 4444, 4444, rb, rb )
	TEST_ORRI_M1_11( 104, rb2rb, 8888, 8888, rb, rb )

	TEST_ORRI_M8_C1( 111, rb2rb, rb, rb )
	TEST_ORRI_M8_C2( 112, rb2rb, rb, rb )
#	TEST_ORRI_M8_C3( 113, rb2rb, rb, rb )
	TEST_ORRI_M8_C4( 114, rb2rb, rb, rb )
	TEST_ORRI_M8_C5( 115, rb2rb, rb, rb )

	TEST_ORRI_M1_12( 201, rd2rb, 4444, 4444, rb, rd )
	TEST_ORRI_M1_12( 202, rd2rb, 8888, 8888, rb, rd )
	TEST_ORRI_M1_11( 203, rd2rb, 4444, 4444, rb, rd )
	TEST_ORRI_M1_11( 204, rd2rb, 8888, 8888, rb, rd )
	TEST_ORRI_M1_10( 205, rd2rb,  0,  0, rb, rd )

	TEST_ORRI_M8_C1( 211, rd2rb, rb, rd )
	TEST_ORRI_M8_C2( 212, rd2rb, rb, rd )
	TEST_ORRI_M8_C3( 213, rd2rb, rb, rd )
	TEST_ORRI_M8_C4( 214, rd2rb, rb, rd )
	TEST_ORRI_M8_C5( 215, rd2rb, rb, rd )

	TEST_ORRI_M1_12( 301, rb2rd, 4444, 4444, rd, rb )
	TEST_ORRI_M1_12( 302, rb2rd, 8888, 8888, rd, rb )
	TEST_ORRI_M1_11( 303, rb2rd, 4444, 4444, rd, rb )
	TEST_ORRI_M1_11( 304, rb2rd, 8888, 8888, rd, rb )

	TEST_ORRI_M8_C1( 311, rb2rd, rd, rb )
	TEST_ORRI_M8_C2( 312, rb2rd, rd, rb )
	TEST_ORRI_M8_C3( 313, rb2rd, rd, rb )
	TEST_ORRI_M8_C4( 314, rb2rd, rd, rb )
	TEST_ORRI_M8_C5( 315, rb2rd, rd, rb )

	TEST_ORRI_M1_12( 401, rd2ra, 4444, 4444, ra, rd )
	TEST_ORRI_M1_12( 402, rd2ra, 8888, 8888, ra, rd )
	TEST_ORRI_M1_11( 403, rd2ra, 4444, 4444, ra, rd )
	TEST_ORRI_M1_11( 404, rd2ra, 8888, 8888, ra, rd )
	TEST_ORRI_M1_10( 405, rd2ra,  0,  0, ra, rd )

	TEST_ORRI_M8_C1( 411, rd2ra, ra, rd )
	TEST_ORRI_M8_C2( 412, rd2ra, ra, rd )
	TEST_ORRI_M8_C3( 413, rd2ra, ra, rd )
	TEST_ORRI_M8_C4( 414, rd2ra, ra, rd )
	TEST_ORRI_M8_C5( 415, rd2ra, ra, rd )

	TEST_ORRI_M1_12( 501, ra2rd, 4444, 4444, rd, ra )
	TEST_ORRI_M1_12( 502, ra2rd, 8888, 8888, rd, ra )
	TEST_ORRI_M1_11( 503, ra2rd, 4444, 4444, rd, ra )
	TEST_ORRI_M1_11( 504, ra2rd, 8888, 8888, rd, ra )

/*
	TODO: SOMETHING WRONG!

	TEST_ORRI_M8_C1( 511, ra2rd, rd, ra )
	TEST_ORRI_M8_C2( 512, ra2rd, rd, ra )
	TEST_ORRI_M8_C3( 513, ra2rd, rd, ra )
	TEST_ORRI_M8_C4( 514, ra2rd, rd, ra )
	TEST_ORRI_M8_C5( 515, ra2rd, rd, ra )
*/
DDTEST_CODE_END

DDTEST_DATA_BEGIN
DDTEST_DATA_END
