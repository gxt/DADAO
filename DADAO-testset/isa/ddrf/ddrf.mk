#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests = \
	rwii-setw \
	orri-fo2io \
	orri-ft2io \
	orri-ftsqrt \
	orri-fxclass \
	orri-rf2rf \
	orri-rf2rd \
	orri-io2fo \
	orri-io2ft \
	orri-rd2rf \
	orrr-fxadd \
	orrr-fxdiv \
	orrr-fxmul \
	orrr-fxsgnj \
	orrr-fxsgnjn \
	orrr-fxsub \
	orrr-ftqcmp \
	rrrr-ftmadd \

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
