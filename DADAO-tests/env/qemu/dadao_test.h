#define TESTNUM rd2

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
	swym	0;

#define DDTEST_CODE_END
#define DDTEST_DATA_BEGIN
#define DDTEST_DATA_END

#define DDTEST_FAIL							\
	setrd    rd16, 1;						\
	setrd    rd17, FAIL;						\
	setrd    rd18, FLEN;						\
	setrd    rd15, 64; /* write */					\
	trap    cp0, 0;							\
	rd2rd   rd16, TESTNUM, 1;					\
	setrd    rd15, 93; /* exit */					\
	trap    cp0, 0;

#define DDTEST_PASS							\
	setrd	rd16, 1;						\
	setrd	rd17, PASS;						\
	setrd	rd18, PLEN;						\
	setrd	rd15, 64; /* write */					\
	trap	cp0, 0;							\
	setrd    rd16, 0;						\
	setrd    rd15, 93; /* exit */					\
	trap    cp0, 0;
