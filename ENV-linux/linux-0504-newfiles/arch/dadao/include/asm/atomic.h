#ifndef __ASM_DADAO_ATOMIC_H
#define __ASM_DADAO_ATOMIC_H

#include <asm/cmpxchg.h>
#include <asm-generic/atomic.h>

#include <linux/types.h>

#define ATOMIC64_INIT(i)	{ (i) }
static __always_inline s64 atomic64_read(const atomic64_t *v)
{
	return READ_ONCE(v->counter);
}
static __always_inline void atomic64_set(atomic64_t *v, s64 i)
{
	WRITE_ONCE(v->counter, i);
}

#define ATOMIC64_OP(op)							\
static __always_inline void atomic64_##op(s64 a, atomic64_t *v)		\
{									\
	s64 result;							\
	unsigned long tmp;						\
									\
	__asm__ __volatile__ (						\
		"218:	lro_nn	rd0, %0, %2\n"				\
		"	" #op "	rd0, %0, %0, %3\n"			\
		"	sco_nn	%1, %0, %2\n"				\
		"	brnz	%1, 218b\n"				\
		: "=&r" (result), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (a));						\
}

ATOMIC64_OP(add)
ATOMIC64_OP(sub)
#undef ATOMIC64_OP

#define ATOMIC64_OP(op, asm_op)						\
static __always_inline void atomic64_##op(s64 a, atomic64_t *v)		\
{									\
	s64 result;							\
	unsigned long tmp;						\
									\
	__asm__ __volatile__ (						\
		"218:	lro_nn	rd0, %0, %2\n"				\
		"	" #asm_op "	%0, %0, %3\n"			\
		"	sco_nn	%1, %0, %2\n"				\
		"	brnz	%1, 218b\n"				\
		"	"						\
		: "=&r" (result), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (a));						\
}

ATOMIC64_OP(and, and)
ATOMIC64_OP(or,  orr)
ATOMIC64_OP(xor, xor)
#undef ATOMIC64_OP

#define ATOMIC64_OP_RETURN(name, op, acq, rel, cl)			\
static __always_inline s64						\
atomic64_##op##_return##name(s64 a, atomic64_t *v)			\
{									\
	s64 result;							\
	unsigned long tmp;						\
									\
	__asm__ __volatile__ (						\
		"218:	lro_" #acq "	rd0, %0, %2\n"			\
		"	" #op "	rd0, %0, %0, %3\n"			\
		"	sco_" #rel "	%1, %0, %2\n"			\
		"	brnz	%1, 218b\n"				\
		: "=&r" (result), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (a)						\
		: cl);							\
									\
	return result;							\
}

#define ATOMIC64_FETCH_OP(name, op, acq, rel, cl)				\
static __always_inline s64 atomic64_fetch_##op##name(s64 a, atomic64_t *v)	\
{										\
	s64 result, val;							\
	unsigned long tmp;							\
										\
	__asm__ __volatile__ (							\
		"218:	lro_" #acq "	rd0, %0, %3\n"				\
		"	" #op "	rd0, %1, %0, %4\n"				\
		"	sco_" #rel "	%2, %1, %3\n"				\
		"	brnz	%2, 218b\n"					\
		: "=&r" (result), "=&r" (val), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (a)							\
		: cl);								\
										\
	return result;								\
}

#define ATOMIC64_FETCH_OPS(op)						\
	ATOMIC64_OP_RETURN(_relaxed, op, nn, nn,         )		\
	ATOMIC64_OP_RETURN(_acquire, op, an, nn, "memory")		\
	ATOMIC64_OP_RETURN(_release, op, nn, nr, "memory")		\
	ATOMIC64_OP_RETURN(        , op, nn, nr, "memory")		\
	ATOMIC64_FETCH_OP( _relaxed, op, nn, nn,         )		\
	ATOMIC64_FETCH_OP( _acquire, op, an, nn, "memory")		\
	ATOMIC64_FETCH_OP( _release, op, nn, nr, "memory")		\
	ATOMIC64_FETCH_OP(         , op, nn, nr, "memory")

ATOMIC64_FETCH_OPS(add)
ATOMIC64_FETCH_OPS(sub)
#undef ATOMIC64_OP_RETURN
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_FETCH_OPS

#define ATOMIC64_FETCH_OP(name, op, asm_op, acq, rel, cl)			\
static __always_inline s64 atomic64_fetch_##op##name(s64 a, atomic64_t *v)	\
{										\
	s64 result, val;							\
	unsigned long tmp;							\
										\
	__asm__ __volatile__ (							\
		"218:	lro_" #acq "	rd0, %0, %3\n"				\
		"	" #asm_op "	%1, %0, %4\n"				\
		"	sco_" #rel "	%2, %1, %3\n"				\
		"	brnz	%2, 218b\n"					\
		: "=&r" (result), "=&r" (val), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (a)							\
		: cl);								\
										\
	return result;								\
}

#define ATOMIC64_FETCH_OPS(op, asm_op)					\
	ATOMIC64_FETCH_OP( _relaxed, op, asm_op, nn, nn,         )	\
	ATOMIC64_FETCH_OP( _acquire, op, asm_op, an, nn, "memory")	\
	ATOMIC64_FETCH_OP( _release, op, asm_op, nn, nr, "memory")	\
	ATOMIC64_FETCH_OP(         , op, asm_op, nn, nr, "memory")

ATOMIC64_FETCH_OPS(and, and)
ATOMIC64_FETCH_OPS(or,  orr)
ATOMIC64_FETCH_OPS(xor, xor)
#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_FETCH_OPS

static __always_inline s64 atomic64_dec_if_positive(atomic64_t *v)
{
       s64 prev;
       long rc;

	__asm__ __volatile__ (
		"218:	lro_nn	rd0, %[p], %[c]\n"
		"	brnp	%[p], 219f"
		"	addi	%[rc], %[p], -1\n"
		"	sco_nr	%[rc], %[rc], %[c]\n"
		"	brnz	%[rc], 218b\n"
		"	fence	0\n"
		"219:\n"
		: [p]"=&r" (prev), [rc]"=&r" (rc), [c]"+Rb" (v->counter)
		: : "memory");
	return prev - 1;
}
#define atomic64_dec_if_positive atomic64_dec_if_positive

static __always_inline s64 atomic64_fetch_add_unless(atomic64_t *v, s64 a, s64 u)
{
       s64 prev;
       long rc;

	__asm__ __volatile__ (
		"218:	lro_nn	rd0, %[p],  %[c]\n"
		"	breq	%[p],  %[u], 219f\n"
		"	add	rd0, %[rc], %[p], %[a]\n"
		"	sco_nr	%[rc], %[rc], %[c]\n"
		"	brnz	%[rc], 218b\n"
		"	fence	0\n"
		"219:\n"
		: [p]"=&r" (prev), [rc]"=&r" (rc), [c]"+Rb" (v->counter)
		: [a]"r" (a), [u]"r" (u)
		: "memory");
	return prev;
}
#define atomic64_fetch_add_unless atomic64_fetch_add_unless

#define ATOMIC64_XCHG_OP(name, acq, rel, cl)				\
static __always_inline s64						\
atomic64_xchg##name(atomic64_t *v, s64 new)				\
{									\
	s64 result;							\
	unsigned long tmp;						\
									\
	__asm__ __volatile__ (						\
		"218:	lro_" #acq "	rd0, %0, %2\n"			\
		"	sco_" #rel "	%1, %3, %2\n"			\
		"	brnz	%1, 218b\n"				\
		: "=&r" (result), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (new)						\
		: cl);							\
									\
	return result;							\
}

ATOMIC64_XCHG_OP(_relaxed, nn, nn,         )
ATOMIC64_XCHG_OP(_acquire, an, nn, "memory")
ATOMIC64_XCHG_OP(_release, nn, nr, "memory")
ATOMIC64_XCHG_OP(        , nn, nr, "memory")

#undef ATOMIC64_XCHG_OP

#define ATOMIC64_CMPXCHG_OP(name, acq, rel, cl)				\
static __always_inline s64						\
atomic64_cmpxchg##name(atomic64_t *v, s64 old, s64 new)			\
{									\
	s64 result;							\
	unsigned long tmp;						\
									\
	__asm__ __volatile__ (						\
		"218:	lro_" #acq "	rd0, %0, %2\n"			\
		"	brne	%0, %3, 219f\n"				\
		"	sco_" #rel "	%1, %4, %2\n"			\
		"	brnz	%1, 218b\n"				\
		"219:\n"						\
		: "=&r" (result), "=&r" (tmp), "+Rb" (v->counter)	\
		: "r" (old), "r" (new)					\
		: cl);							\
									\
	return result;							\
}

ATOMIC64_CMPXCHG_OP(_relaxed, nn, nn,         )
ATOMIC64_CMPXCHG_OP(_acquire, an, nn, "memory")
ATOMIC64_CMPXCHG_OP(_release, nn, nr, "memory")
ATOMIC64_CMPXCHG_OP(        , nn, nr, "memory")

#undef ATOMIC64_CMPXCHG_OP

#endif /* __ASM_DADAO_ATOMIC_H */
