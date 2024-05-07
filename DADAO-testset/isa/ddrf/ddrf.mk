#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests = \
	rwii-setw \
	orri-fx2ix \
	orri-fx2ux \
	orri-fxclass \
	orri-fxsqrt \
	orri-ix2fx \
	orri-rf2rf \
	orri-rf2rd \
	orri-rd2rf \
	orrr-fxadd \
	orrr-fxdiv \
	orrr-fxmul \
	orrr-fxqcmp \
	orrr-fxscmp \
	orrr-fxsgnj \
	orrr-fxsgnjn \
	orrr-fxsub \
	rrrr-fxmadd \

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
