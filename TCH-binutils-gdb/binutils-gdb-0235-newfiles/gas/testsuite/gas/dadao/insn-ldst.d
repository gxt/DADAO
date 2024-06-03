# objdump: -dr
tmpdir/insn-ldst.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	300417ff 	ldbs	rd1, rb1, 2047
   4:	30041800 	ldbs	rd1, rb1, -2048
   8:	310417ff 	ldws	rd1, rb1, 2047
   c:	31041800 	ldws	rd1, rb1, -2048
  10:	320417ff 	ldts	rd1, rb1, 2047
  14:	32041800 	ldts	rd1, rb1, -2048
  18:	330417ff 	ldo	rd1, rb1, 2047
  1c:	33041800 	ldo	rd1, rb1, -2048
  20:	400417ff 	ldbu	rd1, rb1, 2047
  24:	40041800 	ldbu	rd1, rb1, -2048
  28:	410417ff 	ldwu	rd1, rb1, 2047
  2c:	41041800 	ldwu	rd1, rb1, -2048
  30:	420417ff 	ldtu	rd1, rb1, 2047
  34:	42041800 	ldtu	rd1, rb1, -2048
  38:	380417ff 	stb	rd1, rb1, 2047
  3c:	38041800 	stb	rd1, rb1, -2048
  40:	390417ff 	stw	rd1, rb1, 2047
  44:	39041800 	stw	rd1, rb1, -2048
  48:	3a0417ff 	stt	rd1, rb1, 2047
  4c:	3a041800 	stt	rd1, rb1, -2048
  50:	3b0417ff 	sto	rd1, rb1, 2047
  54:	3b041800 	sto	rd1, rb1, -2048
  58:	3400107f 	ldmbs	rd0, rb1, rd1, 0x3f
  5c:	3500107f 	ldmws	rd0, rb1, rd1, 0x3f
  60:	3600107f 	ldmts	rd0, rb1, rd1, 0x3f
  64:	3700107f 	ldmo	rd0, rb1, rd1, 0x3f
  68:	4400107f 	ldmbu	rd0, rb1, rd1, 0x3f
  6c:	4500107f 	ldmwu	rd0, rb1, rd1, 0x3f
  70:	4600107f 	ldmtu	rd0, rb1, rd1, 0x3f
  74:	3c00107f 	stmb	rd0, rb1, rd1, 0x3f
  78:	3d00107f 	stmw	rd0, rb1, rd1, 0x3f
  7c:	3e00107f 	stmt	rd0, rb1, rd1, 0x3f
  80:	3f00107f 	stmo	rd0, rb1, rd1, 0x3f
  84:	430427ff 	ldo	rb1, rb2, 2047
  88:	43042800 	ldo	rb1, rb2, -2048
  8c:	4b0427ff 	sto	rb1, rb2, 2047
  90:	4b042800 	sto	rb1, rb2, -2048
  94:	4700107f 	ldmrb	rb0, rb1, rd1, 0x3f
  98:	4f00107f 	stmrb	rb0, rb1, rd1, 0x3f
  9c:	520417ff 	ldt	rf1, rb1, 2047
  a0:	52041800 	ldt	rf1, rb1, -2048
  a4:	530417ff 	ldo	rf1, rb1, 2047
  a8:	53041800 	ldo	rf1, rb1, -2048
  ac:	5a0417ff 	stt	rf1, rb1, 2047
  b0:	5a041800 	stt	rf1, rb1, -2048
  b4:	5b0417ff 	sto	rf1, rb1, 2047
  b8:	5b041800 	sto	rf1, rb1, -2048
  bc:	5600107f 	ldmt	rf0, rb1, rd1, 0x3f
  c0:	5700107f 	ldmo	rf0, rb1, rd1, 0x3f
  c4:	5e00107f 	stmt	rf0, rb1, rd1, 0x3f
  c8:	5f00107f 	stmo	rf0, rb1, rd1, 0x3f
  cc:	6700107f 	ldmra	ra0, rb1, rd1, 0x3f
  d0:	6f00107f 	stmra	ra0, rb1, rd1, 0x3f
