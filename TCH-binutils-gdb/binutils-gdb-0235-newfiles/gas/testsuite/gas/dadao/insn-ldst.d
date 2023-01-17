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
  58:	3404107f 	ldmbs	rd1, rb1, rd1, 0x3f
  5c:	3504107f 	ldmws	rd1, rb1, rd1, 0x3f
  60:	3604107f 	ldmts	rd1, rb1, rd1, 0x3f
  64:	3704107f 	ldmo	rd1, rb1, rd1, 0x3f
  68:	4404107f 	ldmbu	rd1, rb1, rd1, 0x3f
  6c:	4504107f 	ldmwu	rd1, rb1, rd1, 0x3f
  70:	4604107f 	ldmtu	rd1, rb1, rd1, 0x3f
  74:	3c04107f 	stmb	rd1, rb1, rd1, 0x3f
  78:	3d04107f 	stmw	rd1, rb1, rd1, 0x3f
  7c:	3e04107f 	stmt	rd1, rb1, rd1, 0x3f
  80:	3f04107f 	stmo	rd1, rb1, rd1, 0x3f
  84:	430427ff 	ldrb	rb1, rb2, 2047
  88:	43042800 	ldrb	rb1, rb2, -2048
  8c:	4b0427ff 	strb	rb1, rb2, 2047
  90:	4b042800 	strb	rb1, rb2, -2048
  94:	4704107f 	ldmrb	rb1, rb1, rd1, 0x3f
  98:	4f04107f 	stmrb	rb1, rb1, rd1, 0x3f
  9c:	520417ff 	ldft	rf1, rb1, 2047
  a0:	52041800 	ldft	rf1, rb1, -2048
  a4:	530417ff 	ldfo	rf1, rb1, 2047
  a8:	53041800 	ldfo	rf1, rb1, -2048
  ac:	5a0417ff 	stft	rf1, rb1, 2047
  b0:	5a041800 	stft	rf1, rb1, -2048
  b4:	5b0417ff 	stfo	rf1, rb1, 2047
  b8:	5b041800 	stfo	rf1, rb1, -2048
  bc:	5604107f 	ldmft	rf1, rb1, rd1, 0x3f
  c0:	5704107f 	ldmfo	rf1, rb1, rd1, 0x3f
  c4:	5e04107f 	stmft	rf1, rb1, rd1, 0x3f
  c8:	5f04107f 	stmfo	rf1, rb1, rd1, 0x3f
  cc:	6704107f 	ldmra	ra1, rb1, rd1, 0x3f
  d0:	6f04107f 	stmra	ra1, rb1, rd1, 0x3f
