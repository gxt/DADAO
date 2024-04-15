#=======================================================================
# Makefrag for ddrf tests
#-----------------------------------------------------------------------

ddrf_sc_tests = \
	rwii-setw \
	orri-rf2rf \
	orri-rf2rd \
	orri-rd2rf \

ddrf_bare_tests = $(addprefix ddrf-bare-, $(ddrf_sc_tests))
ddrf_qemu_tests = $(addprefix ddrf-qemu-, $(ddrf_sc_tests))
