# objdump: -dr
tmpdir/insn-pseudo.o:     file format elf64-dadao


Disassembly of section .text:

0000000000000000 <_start>:
   0:	4e04ffff 	setzw	rb1, w0, 0xffff
   4:	4c05ffff 	orw	rb1, w1, 0xffff
   8:	4c06ffff 	orw	rb1, w2, 0xffff
   c:	4c07ffff 	orw	rb1, w3, 0xffff
  10:	4e040000 	setzw	rb1, w0, 0x0
  14:	5400ffff 	setw	rf0, w0, 0xffff
  18:	5401ffff 	setw	rf0, w1, 0xffff
  1c:	5402ffff 	setw	rf0, w2, 0xffff
  20:	5403ffff 	setw	rf0, w3, 0xffff
  24:	54000000 	setw	rf0, w0, 0x0
  28:	54010000 	setw	rf0, w1, 0x0
  2c:	54020000 	setw	rf0, w2, 0x0
  30:	54030000 	setw	rf0, w3, 0x0
  34:	16041234 	setzw	rd1, w0, 0x1234
  38:	16041234 	setzw	rd1, w0, 0x1234
  3c:	1405f00f 	orw	rd1, w1, 0xf00f
  40:	16041234 	setzw	rd1, w0, 0x1234
  44:	1406f00f 	orw	rd1, w2, 0xf00f
  48:	16041234 	setzw	rd1, w0, 0x1234
  4c:	1407f00f 	orw	rd1, w3, 0xf00f
  50:	16041234 	setzw	rd1, w0, 0x1234
  54:	1406f00f 	orw	rd1, w2, 0xf00f
  58:	1407f00f 	orw	rd1, w3, 0xf00f
  5c:	16041234 	setzw	rd1, w0, 0x1234
  60:	1405f00f 	orw	rd1, w1, 0xf00f
  64:	1407ffff 	orw	rd1, w3, 0xffff
  68:	17041234 	setow	rd1, w0, 0x1234
  6c:	15050ff0 	andnw	rd1, w1, 0xff0
  70:	17041234 	setow	rd1, w0, 0x1234
  74:	15060ff0 	andnw	rd1, w2, 0xff0
  78:	17041234 	setow	rd1, w0, 0x1234
  7c:	15070ff0 	andnw	rd1, w3, 0xff0
  80:	17041234 	setow	rd1, w0, 0x1234
  84:	15060ff0 	andnw	rd1, w2, 0xff0
  88:	15070ff0 	andnw	rd1, w3, 0xff0
