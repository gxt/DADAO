#objdump: -dr
tmpdir/insn-float.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	5004107f 	ft2fo	rf1, rf1, 0x3f
   4:	5010107f 	fo2ft	rf1, rf1, 0x3f
   8:	500c107f 	ft2rd	rd1, rf1, 0x3f
   c:	501c107f 	fo2rd	rd1, rf1, 0x3f
  10:	5030107f 	rd2ft	rf1, rd1, 0x3f
  14:	5034107f 	rd2fo	rf1, rd1, 0x3f
  18:	50401083 	ftadd	rf1, rf2, rf3
  1c:	50441083 	ftsub	rf1, rf2, rf3
  20:	50481083 	ftmul	rf1, rf2, rf3
  24:	504c1083 	ftdiv	rf1, rf2, rf3
  28:	50801083 	foadd	rf1, rf2, rf3
  2c:	50841083 	fosub	rf1, rf2, rf3
  30:	50881083 	fomul	rf1, rf2, rf3
  34:	508c1083 	fodiv	rf1, rf2, rf3
  38:	5050107f 	ftabs	rf1, rf1, 0x3f
  3c:	5054107f 	ftneg	rf1, rf1, 0x3f
  40:	5058107f 	ftsqrt	rf1, rf1, 0x3f
  44:	5090107f 	foabs	rf1, rf1, 0x3f
  48:	5094107f 	foneg	rf1, rf1, 0x3f
  4c:	5098107f 	fosqrt	rf1, rf1, 0x3f
  50:	50601042 	ftcun	rd1, rf1, rf2
  54:	50641042 	ftcor	rd1, rf1, rf2
  58:	50681042 	ftcne	rd1, rf1, rf2
  5c:	506c1042 	ftceq	rd1, rf1, rf2
  60:	50701042 	ftclt	rd1, rf1, rf2
  64:	50741042 	ftcge	rd1, rf1, rf2
  68:	50781042 	ftcgt	rd1, rf1, rf2
  6c:	507c1042 	ftcle	rd1, rf1, rf2
  70:	50a01042 	focun	rd1, rf1, rf2
  74:	50a41042 	focor	rd1, rf1, rf2
  78:	50a81042 	focne	rd1, rf1, rf2
  7c:	50ac1042 	foceq	rd1, rf1, rf2
  80:	50b01042 	foclt	rd1, rf1, rf2
  84:	50b41042 	focge	rd1, rf1, rf2
  88:	50b81042 	focgt	rd1, rf1, rf2
  8c:	50bc1042 	focle	rd1, rf1, rf2
