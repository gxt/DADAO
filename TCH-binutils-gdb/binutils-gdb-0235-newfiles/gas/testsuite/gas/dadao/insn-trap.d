# objdump: -dr
tmpdir/insn-trap.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	7007ffff 	trap	cp1, 262143
   4:	7107ffff 	cpcoop	cp1, 262143
   8:	72041081 	rd2cr	cp1, cr1, cr2, rd1
   c:	73041042 	cr2rd	cp1, rd1, cr1, cr2
  10:	74041fff 	cpload	cp1, rb1, 4095
  14:	75041fff 	cpstor	cp1, rb1, 4095
