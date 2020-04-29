# objdump: -dr
tmpdir/list-insns.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <Main>:
   0:	00000003 	trap	0,0,3
   4:	205c500c 	add	\$12, \$23, \$5
   8:	205c808c 	add	\$12, \$23, \$8 << 2
   c:	205c8f8c 	add	\$12, \$23, \$8 << 62
  10:	21503fd6 	add	\$22, \$20, 255
  14:	2153e816 	add	\$22, \$20, 4000
  18:	225c808c 	addu	\$12, \$23, \$8 << 2
  1c:	2353e816 	addu	\$22, \$20, 4000
  20:	245c808c 	sub	\$12, \$23, \$8 << 2
  24:	2553e816 	sub	\$22, \$20, 4000
  28:	265c808c 	subu	\$12, \$23, \$8 << 2
  2c:	2753e816 	subu	\$22, \$20, 4000
  30:	fd000000 	swym	0,0,0
