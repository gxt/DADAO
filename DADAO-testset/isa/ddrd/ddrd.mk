#=======================================================================
# Makefrag for ddrd tests
#-----------------------------------------------------------------------

ddrd_sc_tests = \
	rrii-ldxx rrii-stxx \
	rrri-ldmxx rrri-stmxx \
	orri-rd2rd \
	rwii-setxw rwii-andnw rwii-orw \
	orri-exts \
	orri-extz \
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
	rrrr-add \
	rrrr-csx rrrr-csxx \
	rrrr-divs \
	rrrr-divu \
	rrrr-muls \
	rrrr-mulu \
	rrrr-sub \

ddrd_bare_tests = $(addprefix ddrd-bare-, $(ddrd_sc_tests))
ddrd_qemu_tests = $(addprefix ddrd-qemu-, $(ddrd_sc_tests))
