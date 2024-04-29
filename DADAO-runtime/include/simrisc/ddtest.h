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

#define RB_IP				rb0
#define RB_SP				rb1
#define RB_SRC				rb5
#define RB_DST				rb6

#define RF_FCSR				rf0

#define ft_qNaN				__DDAEE_FLOAT_TETRA_QNAN__
#define ft_sNaN				__DDAEE_FLOAT_TETRA_SNAN__
#define fo_qNaN				__DDAEE_FLOAT_OCTA_QNAN__
#define fo_sNaN				__DDAEE_FLOAT_OCTA_SNAN__

#define fflag_NV			(1 << __DDAEE_FLOAT_FLAG_INVALID__)
#define fflag_DZ			(1 << __DDAEE_FLOAT_FLAG_DIVBYZERO__)
#define fflag_OF			(1 << __DDAEE_FLOAT_FLAG_OVERFLOW__)
#define fflag_UF			(1 << __DDAEE_FLOAT_FLAG_UNDERFLOW__)
#define fflag_NX			(1 << __DDAEE_FLOAT_FLAG_INEXACT__)

#endif
