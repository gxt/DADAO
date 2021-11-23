# objdump: -dr
tmpdir/insn-jsr.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	d1000001 	call	8 <_rec1>
   4:	dadc0000 	ret

0000000000000008 <_rec1>:
   8:	d1000002 	call	14 <_rec2>
   c:	d1000001 	call	14 <_rec2>
  10:	dadc0000 	ret

0000000000000014 <_rec2>:
  14:	d1000004 	call	28 <_rec3>
  18:	d1000003 	call	28 <_rec3>
  1c:	d1000002 	call	28 <_rec3>
  20:	d1000001 	call	28 <_rec3>
  24:	dadc0000 	ret

0000000000000028 <_rec3>:
  28:	d1fffff7 	call	8 <_rec1>
  2c:	dadc0000 	ret
