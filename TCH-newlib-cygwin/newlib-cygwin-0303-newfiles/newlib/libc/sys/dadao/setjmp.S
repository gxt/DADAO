/* Setjmp and longjmp for dadao.

   jmp_buf[5]:
   0:	fp
   1:	rb01 (return-address)
   2:	sp
   3:	rO *before* the setjmp call.
   4:	temporary storage.  Reserved between setjmp and longjmp.  */

#define arg1 rd39
#define arg2 rd40
#define outret rd39

	.section .text.setjmp,"ax",@progbits
	.global setjmp
setjmp:
# Store fp, sp and return address.  Recycle the static-chain and
# structure-return registers as temporary register, since we need to keep
# the jmp_buf (parameter 1) and the return address across a "POP".
	orr	rd59, arg1, rd0
	sto	rd61, rb59, 0
#	get.rs	rd60, rb01
	sto	rd60, rb59, 8
	sto	rd62, rb59, 16
	setowl	outret, 0

# Jump through hoops to get the value of rO *before* the setjmp call.
# TODO	geta	rd63, 0f
#	put.rs	rb01, rd63
	ret
0:
#	get.rs	rd63, rO
	sto	rd63, rb59, 24
# TODO	call	rd0, rb60, 0
	.size setjmp,.-setjmp

	.section .text.longjmp,"ax",@progbits
	.global longjmp
longjmp:
# Reset arg2 to 1 if it is 0 (see longjmp(2)) and store it in jmp_buf.
# Save arg1 in a global register, since it will be destroyed by the POPs
#	setrd	rd01, 1
#	csz	arg2, arg2, rd01
	sto	arg2, rb63, 32
	orr	rd59, arg1, rd0

# Loop and "POP 0,0" until rO is the expected value, like
# the expansion of nonlocal_goto_receiver, except that we put the return
# value in the right register and make sure that the POP causes it to
# enter the right return-value register as seen by the caller.  For the
# GNU ABI, it is unnecessary to do this in the loop and perhaps the memory
# access can be hoisted outside the loop, but this is safe and simple and
# I see no need to optimize longjmps.
# TODO	geta	rd63, 0f
#	put.rs	rb01, rd63
	ldo	rd63, rb59, 24
0:
#	get.rs	rd60, rO
	cmpu	rd60, rd60, rd63
	brnp	rd60, 1f
	ldo	outret, rb59, 32
	ret
1:
	ldo	rd61, rb59, 0
	ldo	rd63, rb59, 8
	ldo	rd62, rb59, 16
# TODO	call	rd0, rb63, 0
 	.size longjmp,.-longjmp
