# objdump: -dr
tmpdir/insn-bit.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	10441083 	shlu	rd1, rd2, rd3
   4:	10481083 	shrs	rd1, rd2, rd3
   8:	104c1083 	shru	rd1, rd2, rd3
   c:	10501083 	exts	rd1, rd2, rd3
  10:	10541083 	extz	rd1, rd2, rd3
  14:	10581083 	not	rd1, rd2, rd3
  18:	106410bf 	shlu	rd1, rd2, 0x3f
  1c:	106810bf 	shrs	rd1, rd2, 0x3f
  20:	106c10bf 	shru	rd1, rd2, 0x3f
  24:	107010bf 	exts	rd1, rd2, 0x3f
  28:	107410bf 	extz	rd1, rd2, 0x3f
  2c:	107810bf 	not	rd1, rd2, 0x3f
