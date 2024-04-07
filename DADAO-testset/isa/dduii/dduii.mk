#=======================================================================
# Makefrag for dduii tests
#-----------------------------------------------------------------------

dduii_sc_tests = \
	orrr-and \
	orrr-orr \
	xor \
	setzw \
	setow \
	andnw \
	orw \
	addi \
	add \
	adrp \
	andi \
	divs \
	divu \
	muls \
	mulu \
	sub \
	cmpu \
	cmps \
	cmp \
	csn \
	csz \
	cseq \
	csne \
	csp \
	shlu \
	shru \
	shrs \
	rd2rd \
	rb2rb \
	rd2rb \
	rb2rd \
	exts \
	extz \

dduii_bare_tests = $(addprefix dduii-bare-, $(dduii_sc_tests))
dduii_qemu_tests = $(addprefix dduii-qemu-, $(dduii_sc_tests))
#dduii_v_tests = $(addprefix dduii-v-, $(dduii_sc_tests))
