# Da Dao Zhi Jian
	.text
	.global	_start
_start:
	setrb	rb1, 0xffffffffffffffff
	setrb	rb1, 0

	setrf	rf0, 0xffffffffffffffff
	setrf	rf0, 0

	setrd	rd1, 0x0000000000001234
	setrd	rd1, 0x00000000f00f1234
	setrd	rd1, 0x0000f00f00001234
	setrd	rd1, 0xf00f000000001234
	setrd	rd1, 0xf00ff00f00001234

	setrd	rd1, 0xffff0000f00f1234
	setrd	rd1, 0xfffffffff00f1234
	setrd	rd1, 0xfffff00fffff1234
	setrd	rd1, 0xf00fffffffff1234
	setrd	rd1, 0xf00ff00fffff1234
