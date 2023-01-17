# objdump: -dr
tmpdir/insn-pseudo.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	4e04ffff 	setzw	rb1, w0, 0xffff
   4:	4c05ffff 	orw	rb1, w1, 0xffff
   8:	4c06ffff 	orw	rb1, w2, 0xffff
   c:	4c07ffff 	orw	rb1, w3, 0xffff
  10:	4e040000 	setzw	rb1, w0, 0x0
  14:	16041234 	setzw	rd1, w0, 0x1234
  18:	16041234 	setzw	rd1, w0, 0x1234
  1c:	1405f00f 	orw	rd1, w1, 0xf00f
  20:	16041234 	setzw	rd1, w0, 0x1234
  24:	1406f00f 	orw	rd1, w2, 0xf00f
  28:	16041234 	setzw	rd1, w0, 0x1234
  2c:	1407f00f 	orw	rd1, w3, 0xf00f
  30:	16041234 	setzw	rd1, w0, 0x1234
  34:	1406f00f 	orw	rd1, w2, 0xf00f
  38:	1407f00f 	orw	rd1, w3, 0xf00f
  3c:	16041234 	setzw	rd1, w0, 0x1234
  40:	1405f00f 	orw	rd1, w1, 0xf00f
  44:	1407ffff 	orw	rd1, w3, 0xffff
  48:	16041234 	setzw	rd1, w0, 0x1234
  4c:	1406f00f 	orw	rd1, w2, 0xf00f
  50:	1407ffff 	orw	rd1, w3, 0xffff
  54:	16041234 	setzw	rd1, w0, 0x1234
  58:	1406ffff 	orw	rd1, w2, 0xffff
  5c:	1407f00f 	orw	rd1, w3, 0xf00f
  60:	17041234 	setow	rd1, w0, 0x1234
  64:	17041234 	setow	rd1, w0, 0x1234
  68:	15050ff0 	andnw	rd1, w1, 0xff0
  6c:	17041234 	setow	rd1, w0, 0x1234
  70:	15060ff0 	andnw	rd1, w2, 0xff0
  74:	17041234 	setow	rd1, w0, 0x1234
  78:	15070ff0 	andnw	rd1, w3, 0xff0
  7c:	17041234 	setow	rd1, w0, 0x1234
  80:	15060ff0 	andnw	rd1, w2, 0xff0
  84:	15070ff0 	andnw	rd1, w3, 0xff0
