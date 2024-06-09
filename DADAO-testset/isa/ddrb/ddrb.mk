#=======================================================================
# Makefrag for ddrb tests
#-----------------------------------------------------------------------

ddrb_pass2_tests =			\
	rwii-andnw	rwii-orw	rwii-setzw	\
	orrr-add	orrr-sub	\
	rrii-addi				\
	orrr-cmp				\
	riii-adrp				\
	riii-brxx	rrii-brxx	\
	iiii-jump	rrii-jump	\
	iiii-call	rrii-call	\
	riii-ret				\

ddrb_passqemu_tests =		\
	rrii-ldo	rrii-sto	\
	rrri-ldmo	rrri-stmo	\
	orri-rx2rx				\

ddrb_bare_tests = $(addprefix ddrb-bare-, $(ddrb_pass2_tests))
ddrb_qemu_tests = $(addprefix ddrb-qemu-, $(ddrb_pass2_tests) $(ddrb_passqemu_tests))
