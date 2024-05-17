#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests =				\
	rrii-ldfx	rrii-stfx	\
	rrri-ldmfx	rrri-stmfx	\
	orri-rx2rx				\
	rwii-setw				\
	orri-fx2fx	orri-fx2ix	orri-fx2ux	orri-ix2fx	orri-ux2fx	\
	orrr-fxadd	orrr-fxsub	orrr-fxmul	orrr-fxdiv				\
	rrrr-fxmadd				\
	orri-fxsqrt				\
	orrr-fxsgnj	orrr-fxsgnjn	\
	orrr-fxqcmp	orrr-fxscmp		\
	rrrr-csx	orrr-csxx		\
	orri-fxclass			\

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
