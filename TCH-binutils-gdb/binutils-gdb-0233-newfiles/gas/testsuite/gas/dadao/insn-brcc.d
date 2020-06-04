# objdump: -dr
tmpdir/insn-brcc.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	c0040000 	bn	\$1, 4 <_start\+0x4>
   4:	c00bffff 	bn	\$2, 4 <_start\+0x4>
   8:	c00ffffd 	bn	\$3, 0 <_start>
   c:	c2040000 	bz	\$1, 10 <_start\+0x10>
  10:	c20bffff 	bz	\$2, 10 <_start\+0x10>
  14:	c20ffffa 	bz	\$3, 0 <_start>
  18:	c4040000 	bp	\$1, 1c <_start\+0x1c>
  1c:	c40bffff 	bp	\$2, 1c <_start\+0x1c>
  20:	c40ffff7 	bp	\$3, 0 <_start>
  24:	c6040000 	bod	\$1, 28 <_start\+0x28>
  28:	c60bffff 	bod	\$2, 28 <_start\+0x28>
  2c:	c60ffff4 	bod	\$3, 0 <_start>
  30:	c1040000 	bnn	\$1, 34 <_start\+0x34>
  34:	c10bffff 	bnn	\$2, 34 <_start\+0x34>
  38:	c10ffff1 	bnn	\$3, 0 <_start>
  3c:	c3040000 	bnz	\$1, 40 <_start\+0x40>
  40:	c30bffff 	bnz	\$2, 40 <_start\+0x40>
  44:	c30fffee 	bnz	\$3, 0 <_start>
  48:	c5040000 	bnp	\$1, 4c <_start\+0x4c>
  4c:	c50bffff 	bnp	\$2, 4c <_start\+0x4c>
  50:	c50fffeb 	bnp	\$3, 0 <_start>
  54:	c7040000 	bev	\$1, 58 <_start\+0x58>
  58:	c70bffff 	bev	\$2, 58 <_start\+0x58>
  5c:	c70fffe8 	bev	\$3, 0 <_start>
