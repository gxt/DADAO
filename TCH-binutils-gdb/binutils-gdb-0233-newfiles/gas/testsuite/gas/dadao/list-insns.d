# objdump: -dr
tmpdir/list-insns.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <Main>:
   0:	205c500c 	add	\$12, \$23, \$5
   4:	205c808c 	add	\$12, \$23, \$8 << 2
   8:	205c8f8c 	add	\$12, \$23, \$8 << 62
   c:	21503fd6 	add	\$22, \$20, 255
  10:	2153e816 	add	\$22, \$20, 4000
  14:	225c808c 	addu	\$12, \$23, \$8 << 2
  18:	2353e816 	addu	\$22, \$20, 4000
  1c:	245c808c 	sub	\$12, \$23, \$8 << 2
  20:	2553e816 	sub	\$22, \$20, 4000
  24:	265c808c 	subu	\$12, \$23, \$8 << 2
  28:	2753e816 	subu	\$22, \$20, 4000
  2c:	fd000000 	swym	0,0,0
