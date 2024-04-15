#=======================================================================
# Makefrag for ddrd tests
#-----------------------------------------------------------------------

ddrd_sc_tests = \
	orri-exts \
	orri-extz \
	orri-rd2rd \
	orri-shlu \
	orri-shrs \
	orri-shru \
	orrr-and \
	orrr-cmps \
	orrr-cmpu \
	orrr-exts \
	orrr-extz \
	orrr-orr \
	orrr-shlu \
	orrr-shrs \
	orrr-shru \
	orrr-xor \
	rrii-addi \
	rrii-andi \
	rrii-cmps \
	rrii-cmpu \
	rrii-ldo \
	rrrr-add \
	rrrr-cseq \
	rrrr-csn \
	rrrr-csne \
	rrrr-csp \
	rrrr-csz \
	rrrr-divs \
	rrrr-divu \
	rrrr-muls \
	rrrr-mulu \
	rrrr-sub \
	rwii-andnw \
	rwii-orw \
	rwii-setow \
	rwii-setzw \

ddrd_bare_tests = $(addprefix ddrd-bare-, $(ddrd_sc_tests))
ddrd_qemu_tests = $(addprefix ddrd-qemu-, $(ddrd_sc_tests))
