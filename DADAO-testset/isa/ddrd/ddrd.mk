#=======================================================================
# Makefrag for ddrd tests
#-----------------------------------------------------------------------

ddrd_pass2_tests =				\
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

ddrd_passqemu_tests =		\
	rrii-ldxx	rrii-stxx	\
	rrri-ldmxx	rrri-stmxx	\

ddrd_bare_tests = $(addprefix ddrd-bare-, $(ddrd_pass2_tests))
ddrd_qemu_tests = $(addprefix ddrd-qemu-, $(ddrd_pass2_tests) $(ddrd_passqemu_tests))
