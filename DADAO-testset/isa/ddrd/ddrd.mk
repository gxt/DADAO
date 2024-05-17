#=======================================================================
# Makefrag for ddrd tests
#-----------------------------------------------------------------------

ddrd_sc_tests =				\
	rrii-ldxx	rrii-stxx	\
	rrri-ldmxx	rrri-stmxx	\
	orri-rd2rd				\
	rwii-setxw	rwii-andnw	rwii-orw	\
	rrrr-csx	rrrr-csxx	\
	rrrr-add	rrrr-sub	rrii-addi	\
	rrii-cmpx	orrr-cmpx	\
	rrrr-mulx	rrrr-divx	\
	orrr-and	orrr-orr	orrr-xor	orrr-xnor	\
	rrii-andi				\
	orri-shxx	orrr-shxx	\
	orri-extx	orrr-extx	\

ddrd_bare_tests = $(addprefix ddrd-bare-, $(ddrd_sc_tests))
ddrd_qemu_tests = $(addprefix ddrd-qemu-, $(ddrd_sc_tests))
