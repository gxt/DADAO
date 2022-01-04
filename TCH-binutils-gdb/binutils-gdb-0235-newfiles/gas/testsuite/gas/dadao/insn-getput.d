# objdump: -dr
tmpdir/insn-getput.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	10a01040 	rd2rd	rd1, rd1, 0
   4:	10a41041 	rd2rb	rb1, rd1, 1
   8:	10a81042 	rb2rd	rd1, rb1, 2
   c:	10ae1043 	rb2rb	rb1, rb1, 3
  10:	10b41042    cmprb rd1, rb1, rb2
  14:	10b81081    addrb rb1, rb2, rd1
  18: 10bc1081    subrb rb1, rb2, rd1
  1c: 10c41040 	rd2rf	rf1, rd1, 0
  20:	10c81041 	rf2rd	rd1, rf1, 1
  24: 10ce1042 	rf2rf	rf1, rf1, 2
  28: 10e41043    rd2ra ra1, rd1, 3
  2c: 10e81044    ra2rd rd1, ra1, 4

  
