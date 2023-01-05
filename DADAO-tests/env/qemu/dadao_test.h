#define TESTNUM rd3

#define DDTEST_DDUII							\
	.text;								\
FAIL:									\
	.ascii  "FAIL\n";						\
	.equ FLEN, .-FAIL;						\
	.type FAIL, object;						\
	.size FAIL, FLEN;						\
	.text;								\
	.align 4;							\
PASS:									\
	.ascii  "PASS";							\
	.equ PLEN, .-PASS;						\
	.type PASS, object;						\
	.size PASS, PLEN;						\
	.text;								\
	.align 4;

#define DDTEST_CODE_BEGIN						\
	.text;								\
	.globl _start;							\
	.align 4;							\
_start:									\
	swym;

#define DDTEST_CODE_END
#define DDTEST_DATA_BEGIN
#define DDTEST_DATA_END

#define DDTEST_FAIL							\
	move    rd16, 1;						\
	move    rd17, FAIL;						\
	move    rd18, FLEN;						\
	move    rd15, 64; /* write */					\
	trap    cp0, 0;							\
	rd2rd   rd16, TESTNUM, 0;					\
	move    rd15, 93; /* exit */					\
	trap    cp0, 0;

#define DDTEST_PASS							\
	move	rd16, 1;						\
	move	rd17, PASS;						\
	move	rd18, PLEN;						\
	move	rd15, 64; /* write */					\
	trap	cp0, 0;							\
	move    rd16, 0;						\
	move    rd15, 93; /* exit */					\
	trap    cp0, 0;
