#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_passqemu_tests =		\
	rrii-ldx	rrii-stx	\
	rrri-ldmx	rrri-stmx	\
	orri-fx2fx	orri-fx2ix	orri-fx2ux	orri-ix2fx	orri-ux2fx	\
	orrr-fxadd	orrr-fxsub	orrr-fxmul	orrr-fxdiv				\
	rrrr-fxmadd				\
	orri-fxroot				\
	orrr-fxsgnj	orrr-fxsgnn	\
	orrr-fxqcmp	orrr-fxscmp	\
	rrrr-csx	orrr-csxx	\
	orri-fxcls				\

ddrf_pass2_tests =			\
	orri-rx2rx				\
	rwii-setw				\

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_pass2_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_pass2_tests) $(ddrf_passqemu_tests))
