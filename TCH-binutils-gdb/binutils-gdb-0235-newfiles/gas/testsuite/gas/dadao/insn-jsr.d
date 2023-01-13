# objdump: -dr
tmpdir/insn-jsr.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	6c000002 	call	8 <_rec1>
   4:	6e000000 	ret	rd0, 0x0

0000000000000008 <_rec1>:
   8:	6c000003 	call	14 <_rec2>
   c:	6c000002 	call	14 <_rec2>
  10:	6e000000 	ret	rd0, 0x0

0000000000000014 <_rec2>:
  14:	6c000005 	call	28 <_rec3>
  18:	6c000004 	call	28 <_rec3>
  1c:	6c000003 	call	28 <_rec3>
  20:	6c000002 	call	28 <_rec3>
  24:	6e000000 	ret	rd0, 0x0

0000000000000028 <_rec3>:
  28:	6cfffff8 	call	8 <_rec1>
  2c:	6e000000 	ret	rd0, 0x0
