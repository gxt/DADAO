#objdump: -dr
tmpdir/insn-float.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	5004003f 	ft2fo	rf0, rf0, 0x3f
   4:	5024003f 	fo2ft	rf0, rf0, 0x3f
   8:	5008003f 	ft2rd	rd0, rf0, 0x3f
   c:	5028003f 	fo2rd	rd0, rf0, 0x3f
  10:	500c003f 	rd2ft	rf0, rd0, 0x3f
  14:	502c003f 	rd2fo	rf0, rd0, 0x3f
  18:	50401083 	ftadd	rf1, rf2, rf3
  1c:	50441083 	ftsub	rf1, rf2, rf3
  20:	50481083 	ftmul	rf1, rf2, rf3
  24:	504c1083 	ftdiv	rf1, rf2, rf3
  28:	50601083 	foadd	rf1, rf2, rf3
  2c:	50641083 	fosub	rf1, rf2, rf3
  30:	50681083 	fomul	rf1, rf2, rf3
  34:	506c1083 	fodiv	rf1, rf2, rf3
  38:	5010003f 	ftabs	rf0, rf0, 0x3f
  3c:	5014003f 	ftneg	rf0, rf0, 0x3f
  40:	5018003f 	ftsqrt	rf0, rf0, 0x3f
  44:	5030003f 	foabs	rf0, rf0, 0x3f
  48:	5034003f 	foneg	rf0, rf0, 0x3f
  4c:	5038003f 	fosqrt	rf0, rf0, 0x3f
  50:	50801042 	ftcun	rd1, rf1, rf2
  54:	50841042 	ftcor	rd1, rf1, rf2
  58:	50881042 	ftcne	rd1, rf1, rf2
  5c:	508c1042 	ftceq	rd1, rf1, rf2
  60:	50901042 	ftclt	rd1, rf1, rf2
  64:	50941042 	ftcge	rd1, rf1, rf2
  68:	50981042 	ftcgt	rd1, rf1, rf2
  6c:	509c1042 	ftcle	rd1, rf1, rf2
  70:	50a01042 	focun	rd1, rf1, rf2
  74:	50a41042 	focor	rd1, rf1, rf2
  78:	50a81042 	focne	rd1, rf1, rf2
  7c:	50ac1042 	foceq	rd1, rf1, rf2
  80:	50b01042 	foclt	rd1, rf1, rf2
  84:	50b41042 	focge	rd1, rf1, rf2
  88:	50b81042 	focgt	rd1, rf1, rf2
  8c:	50bc1042 	focle	rd1, rf1, rf2
