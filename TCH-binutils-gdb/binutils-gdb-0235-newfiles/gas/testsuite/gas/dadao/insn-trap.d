# objdump: -dr
tmpdir/insn-trap.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	7007ffff 	trap	cp1, 0x3ffff
   4:	7107ffff 	cpco	cp1, 0x3ffff
   8:	72041081 	cprd	cp1, cr1, cr2, rd1
   c:	73041081 	cpwr	cp1, cr1, cr2, rd1
  10:	74041fff 	cpld	cp1, rb1, 0xfff
  14:	75041fff 	cpst	cp1, rb1, 0xfff
