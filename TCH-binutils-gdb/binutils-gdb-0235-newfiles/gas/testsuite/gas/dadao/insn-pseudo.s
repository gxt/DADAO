# Da Dao Zhi Jian
	.text
	.global	_start
_start:
	move	rb1, 0xffffffffffffffff
	move	rb1, 0

	move	rd1, 0x0000000000001234
	move	rd1, 0x00000000f00f1234
	move	rd1, 0x0000f00f00001234
	move	rd1, 0xf00f000000001234
	move	rd1, 0xf00ff00f00001234

	move	rd1, 0xffff0000f00f1234
	move	rd1, 0xfffff00f00001234
	move	rd1, 0xf00fffff00001234

	move	rd1, 0xffffffffffff1234
	move	rd1, 0xfffffffff00f1234
	move	rd1, 0xfffff00fffff1234
	move	rd1, 0xf00fffffffff1234
	move	rd1, 0xf00ff00fffff1234
