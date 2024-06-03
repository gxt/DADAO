#=======================================================================
# Makefrag for ddrb tests
#-----------------------------------------------------------------------

ddrb_sc_tests =				\
	rrii-ldo	rrii-sto	\
	rrri-ldmo	rrri-stmo	\
	orri-rx2rx				\
	rwii-andnw	rwii-orw	rwii-setzw	\
	orrr-add	orrr-sub	\
	rrii-addi				\
	orrr-cmp				\
	riii-adrp				\
	riii-brxx	rrii-brxx	\
	iiii-jump	rrii-jump	\
	iiii-call	rrii-call	\
	riii-ret				\

ddrb_bare_tests = $(addprefix ddrb-bare-, $(ddrb_sc_tests))
ddrb_qemu_tests = $(addprefix ddrb-qemu-, $(ddrb_sc_tests))
