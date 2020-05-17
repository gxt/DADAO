/* Setjmp and longjmp for dadao.

   jmp_buf[5]:
   0:	fp
   1:	rJ (return-address)
   2:	sp
   3:	rO *before* the setjmp call.
   4:	temporary storage.  Reserved between setjmp and longjmp.  */

#define arg1 $231
#define arg2 $232
#define outret $231

	.section .text.setjmp,"ax",@progbits
	.global setjmp
setjmp:
# Store fp, sp and return address.  Recycle the static-chain and
# structure-return registers as temporary register, since we need to keep
# the jmp_buf (parameter 1) and the return address across a "POP".
	or	$59, arg1, 0
	sto	$61, $59, 0
	get	$60, rJ
	sto	$60, $59, 8
	sto	$62, $59, 16
	setl	outret, 0

# Jump through hoops to get the value of rO *before* the setjmp call.
	geta	$63, 0f
	put	rJ, $63
	ret
0:
	get	$63, rO
	sto	$63, $59, 24
	call	$60, $0, 0
	.size setjmp,.-setjmp

	.section .text.longjmp,"ax",@progbits
	.global longjmp
longjmp:
# Reset arg2 to 1 if it is 0 (see longjmp(2)) and store it in jmp_buf.
# Save arg1 in a global register, since it will be destroyed by the POPs
	csz	arg2, arg2, 1
	sto	arg2, arg1, 32
	or	$59, arg1, 0

# Loop and "POP 0,0" until rO is the expected value, like
# the expansion of nonlocal_goto_receiver, except that we put the return
# value in the right register and make sure that the POP causes it to
# enter the right return-value register as seen by the caller.  For the
# GNU ABI, it is unnecessary to do this in the loop and perhaps the memory
# access can be hoisted outside the loop, but this is safe and simple and
# I see no need to optimize longjmps.
	geta	$63, 0f
	put	rJ, $63
	ldo	$63, $59, 24
0:
	get	$60, rO
	cmpu	$60, $60, $63
	BNP  $252,1f
	ldo	outret, $59, 32
	ret
1:
	ldo	$61, $59, 0
	ldo	$63, $59, 8
	ldo	$62, $59, 16
	call	$63, $0, 0
 	.size longjmp,.-longjmp
