#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests = \
	rwii-setw \
	orri-fo2rd \
	orri-foclass \
	orri-ft2rd \
	orri-ftclass \
	orri-ftsqrt \
	orri-rf2rf \
	orri-rf2rd \
	orri-rd2fo \
	orri-rd2ft \
	orri-rd2rf \
	orrr-foadd \
	orrr-ftadd \
	orrr-ftcne \

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
