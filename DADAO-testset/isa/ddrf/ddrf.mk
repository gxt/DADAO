#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests = \
	rwii-setw \
	orri-fo2io \
	orri-foclass \
	orri-ft2io \
	orri-ftclass \
	orri-ftsqrt \
	orri-rf2rf \
	orri-rf2rd \
	orri-io2fo \
	orri-io2ft \
	orri-rd2rf \
	orrr-foadd \
	orrr-ftadd \
	orrr-ftcne \

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
