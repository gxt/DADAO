/* Setjmp and longjmp for dadao.

   jmp_buf[5]:
   0:	fp
   1:	rJ (return-address)
   2:	sp
   3:	rO *before* the setjmp call.
   4:	temporary storage.  Reserved between setjmp and longjmp.  */

#define arg1 rg39
#define arg2 rg40
#define outret rg39

	.section .text.setjmp,"ax",@progbits
	.global setjmp
setjmp:
# Store fp, sp and return address.  Recycle the static-chain and
# structure-return registers as temporary register, since we need to keep
# the jmp_buf (parameter 1) and the return address across a "POP".
	or	rg59, arg1, 0
	sto	rg61, rp59, 0
	get.rs	rg60, rJ
	sto	rg60, rp59, 8
	sto	rg62, rp59, 16
	setwl	outret, 0

# Jump through hoops to get the value of rO *before* the setjmp call.
	geta	rp63, 0f
	put.rs	rJ, rg63
	ret
0:
	get.rs	rg63, rO
	sto	rg63, rp59, 24
	call	rg0, rp60, 0
	.size setjmp,.-setjmp

	.section .text.longjmp,"ax",@progbits
	.global longjmp
longjmp:
# Reset arg2 to 1 if it is 0 (see longjmp(2)) and store it in jmp_buf.
# Save arg1 in a global register, since it will be destroyed by the POPs
	csz	arg2, arg2, 1
	sto	arg2, rp63, 32
	or	rg59, arg1, 0

# Loop and "POP 0,0" until rO is the expected value, like
# the expansion of nonlocal_goto_receiver, except that we put the return
# value in the right register and make sure that the POP causes it to
# enter the right return-value register as seen by the caller.  For the
# GNU ABI, it is unnecessary to do this in the loop and perhaps the memory
# access can be hoisted outside the loop, but this is safe and simple and
# I see no need to optimize longjmps.
	geta	rg63, 0f
	put.rs	rJ, rg63
	ldo	rg63, rp59, 24
0:
	get.rs	rg60, rO
	cmpu	rg60, rg60, rg63
	br.np	rg60, 1f
	ldo	outret, rp59, 32
	ret
1:
	ldo	rg61, rp59, 0
	ldo	rg63, rp59, 8
	ldo	rg62, rp59, 16
	call	rg0, rp63, 0
 	.size longjmp,.-longjmp
