#ifndef _RUNTIME_SIMRISC_DDTEST_H_
#define _RUNTIME_SIMRISC_DDTEST_H_

#include "simrisc/dadao-aee.h"

#define RD_ZERO				rd0
#define RD_FLAG				rd1
#define RD_NAME				rd2
#define RD_PASS				rd3
#define RD_EXP1				rd4
#define RD_EXP2				rd5
#define RD_RET1				rd6
#define RD_RET2				rd7

#define RB_NEXT				rb0
#define RB_SP				rb1
#define RB_SRC				rb5
#define RB_DST				rb6

#define RF_FCSR				rf0

#define ft_qNaN				0f:7fc00000
#define ft_sNaN				0f:7f800001
#define fo_qNaN				0d:7ff8000000000000
#define fo_sNaN				0d:7ff0000000000001

#define it_qNaN				__DDAEE_FLOAT_TETRA_QNAN__
#define it_sNaN				__DDAEE_FLOAT_TETRA_SNAN__
#define io_qNaN				__DDAEE_FLOAT_OCTA_QNAN__
#define io_sNaN				__DDAEE_FLOAT_OCTA_SNAN__

#define ft_minus_qNaN		0f:ffc00000
#define fo_minus_qNaN		0d:fff8000000000000

#define it_min				0x80000000
#define it_max				0x7fffffff
#define io_min				0x8000000000000000
#define io_max				0x7fffffffffffffff

#define ut_max				0xffffffff
#define uo_max				0xffffffffffffffff

#define fflags_Z			0
#define fflag_NV			__DDAEE_FCSR_FFLAG_INVALID__
#define fflag_DZ			__DDAEE_FCSR_FFLAG_DIVBYZERO__
#define fflag_OF			__DDAEE_FCSR_FFLAG_OVERFLOW__
#define fflag_UF			__DDAEE_FCSR_FFLAG_UNDERFLOW__
#define fflag_NX			__DDAEE_FCSR_FFLAG_INEXACT__

#endif
