#ifndef _RUNTIME_SIMRISC_DDTEST_H_
#define _RUNTIME_SIMRISC_DDTEST_H_

#define RD_ZERO				rd0
#define RD_FLAG				rd1
#define RD_NAME				rd2
#define RD_PASS				rd3
#define RD_EXP1				rd4
#define RD_EXP2				rd5
#define RD_RET1				rd6
#define RD_RET2				rd7

#define RB_IP				rb0
#define RB_SP				rb1
#define RB_SRC1				rb4
#define RB_SRC2				rb5
#define RB_DST1				rb6

#define RF_FCSR				rf0

#define qNaNf				0f:7fc00000
#define sNaNf				0f:7f800001
#define qNaN				0d:7ff8000000000000
#define sNaN				0d:7ff0000000000001

#endif
