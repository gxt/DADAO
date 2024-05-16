#=======================================================================
# Makefrag for ddrd tests
#-----------------------------------------------------------------------

ddrd_sc_tests = \
	rrii-ldxx rrii-stxx \
	rrri-ldmxx rrri-stmxx \
	orri-rd2rd \
	rwii-setxw rwii-andnw rwii-orw \
	rrrr-csx rrrr-csxx \
	rrrr-add rrrr-sub rrii-addi \
	rrii-cmpx orrr-cmpx \
	rrrr-mulx rrrr-divx \
	orri-exts \
	orri-extz \
	orri-shlu \
	orri-shrs \
	orri-shru \
	orrr-and \
	orrr-exts \
	orrr-extz \
	orrr-orr \
	orrr-shlu \
	orrr-shrs \
	orrr-shru \
	orrr-xor \
	rrii-andi \

ddrd_bare_tests = $(addprefix ddrd-bare-, $(ddrd_sc_tests))
ddrd_qemu_tests = $(addprefix ddrd-qemu-, $(ddrd_sc_tests))
