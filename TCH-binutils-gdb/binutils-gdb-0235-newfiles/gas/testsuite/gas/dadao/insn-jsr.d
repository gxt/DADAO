# objdump: -dr
tmpdir/insn-jsr.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	6c000004 	call	10 <_rec1>
   4:	64000003 	jump	10 <_rec1>
   8:	10000000 	swym	0x0
   c:	6e000000 	ret	rd0, 0

0000000000000010 <_rec1>:
  10:	6c000004 	call	20 <_rec2>
  14:	6c000003 	call	20 <_rec2>
  18:	64000002 	jump	20 <_rec2>
  1c:	6e05ffff 	ret	rd1, 131071

0000000000000020 <_rec2>:
  20:	6c000006 	call	38 <_rec3>
  24:	6c000005 	call	38 <_rec3>
  28:	6c000004 	call	38 <_rec3>
  2c:	6c000003 	call	38 <_rec3>
  30:	64000002 	jump	38 <_rec3>
  34:	6e060000 	ret	rd1, -131072

0000000000000038 <_rec3>:
  38:	6cfffff6 	call	10 <_rec1>
  3c:	64fffff5 	jump	10 <_rec1>
  40:	650417ff 	jump	rb1, rd1, 2047
  44:	65041800 	jump	rb1, rd1, -2048
  48:	6e000000 	ret	rd0, 0
