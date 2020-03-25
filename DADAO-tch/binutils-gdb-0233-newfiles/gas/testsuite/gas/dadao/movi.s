	.text
	.global	main
	.type	main, @function
main:
	movi	rd1.t0, $0x1234
	movi	rd1.t0, $0x5678
	movi	rd1.t0, $0x1234
	movi	rd1.t0, $0x5678
