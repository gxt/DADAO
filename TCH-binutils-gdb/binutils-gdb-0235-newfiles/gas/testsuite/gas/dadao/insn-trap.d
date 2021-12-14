# objdump: -dr
tmpdir/insn-trap.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	7007ffff 	trap	cp1, 262144
   4:	7107ffff 	cmd 	cp1, 262144
   8:	72041041 	rg2cr	cp1, cr1, cr2, rg1
   c:	73041041 	cr2rg	cp1, rg1, cr1, cr2
  10:	74041fff 	cpld    cp1, rp1, 4096
  14:	75041fff 	cpst    cp1, rp1, 4096
