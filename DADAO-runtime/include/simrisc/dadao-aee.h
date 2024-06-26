#ifndef __DADAO_AEE_H__
#define __DADAO_AEE_H__

#define __DDAEE_RD_ZERO__								0
#define __DDAEE_RB_NEXT__								0
#define __DDAEE_RA_MRAS__								0
#define __DDAEE_RF_FCSR__								0

#define __DDAEE_FLOAT_CLASS_NEGATIVE_INFINITY__			(1 << 0)
#define __DDAEE_FLOAT_CLASS_NEGATIVE_NORMAL__			(1 << 1)
#define __DDAEE_FLOAT_CLASS_NEGATIVE_SUBNORMAL__		(1 << 2)
#define __DDAEE_FLOAT_CLASS_NEGATIVE_ZERO__				(1 << 3)
#define __DDAEE_FLOAT_CLASS_POSITIVE_ZERO__				(1 << 4)
#define __DDAEE_FLOAT_CLASS_POSITIVE_SUBNORMAL__		(1 << 5)
#define __DDAEE_FLOAT_CLASS_POSITIVE_NORMAL__			(1 << 6)
#define __DDAEE_FLOAT_CLASS_POSITIVE_INFINITY__			(1 << 7)
#define __DDAEE_FLOAT_CLASS_SIGNALING_NAN__				(1 << 8)
#define __DDAEE_FLOAT_CLASS_QUIET_NAN__					(1 << 9)

#define __DDAEE_FLOAT_TETRA_QNAN__						0x7fc00000
#define __DDAEE_FLOAT_TETRA_SNAN__						0x7f800001
#define __DDAEE_FLOAT_OCTA_QNAN__						0x7ff8000000000000ULL
#define __DDAEE_FLOAT_OCTA_SNAN__						0x7ff0000000000001ULL

#define __DDAEE_FCSR_DEFAULT__							(__DDAEE_FLOAT_OCTA_QNAN__ | __DDAEE_FLOAT_TETRA_QNAN__)

#define __DDAEE_FCSR_MASK_RDMODE__						0x30000
#define __DDAEE_FCSR_MASK_FFLAGS__						0x1f

#define __DDAEE_FCSR_FFLAG_INVALID__					(1 << 0)
#define __DDAEE_FCSR_FFLAG_DIVBYZERO__					(1 << 1)
#define __DDAEE_FCSR_FFLAG_OVERFLOW__					(1 << 2)
#define __DDAEE_FCSR_FFLAG_UNDERFLOW__					(1 << 3)
#define __DDAEE_FCSR_FFLAG_INEXACT__					(1 << 4)

#define __DDAEE_FCSR_RDMODE_RNE__						0b00	/* Round to Nearest, ties to Even */
#define __DDAEE_FCSR_RDMODE_RTZ__						0b01	/* Round towards Zero */
#define __DDAEE_FCSR_RDMODE_RDN__						0b10	/* Round Down */
#define __DDAEE_FCSR_RDMODE_RUP__						0b11	/* Round Up */

#endif
