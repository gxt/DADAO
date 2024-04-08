#=======================================================================
# Makefrag for dduii tests
#-----------------------------------------------------------------------

dduii_sc_tests = \
	orrr-and \
	orrr-orr \
	orrr-xor \
	orrr-add \
	orrr-sub \
	orrr-cmp \
	orrr-cmps \
	orrr-cmpu \
	orrr-exts \
	orrr-extz \
	orrr-shlu \
	orrr-shru \
	orrr-shrs \
	orri-exts \
	orri-extz \
	orri-shlu \
	orri-shru \
	orri-shrs \
	rrii-addi-rd \
	rrii-addi-rb \
	rrii-andi \
	rrii-cmpu \
	rrii-cmps \
	rwii-setzw-rd \
	rwii-setzw-rb \
	rwii-setow \
	rwii-andnw-rd \
	rwii-andnw-rb \
	rwii-orw-rd \
	rwii-orw-rb \
	add \
	adrp \
	divs \
	divu \
	muls \
	mulu \
	sub \
	rrrr-csn \
	rrrr-csp \
	rrrr-csz \
	rrrr-cseq \
	rrrr-csne \
	rd2rd \
	rb2rb \
	rd2rb \
	rb2rd \

dduii_bare_tests = $(addprefix dduii-bare-, $(dduii_sc_tests))
dduii_qemu_tests = $(addprefix dduii-qemu-, $(dduii_sc_tests))
#dduii_v_tests = $(addprefix dduii-v-, $(dduii_sc_tests))
