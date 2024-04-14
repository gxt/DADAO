#=======================================================================
# Makefrag for ddfii tests
#-----------------------------------------------------------------------

ddfii_sc_tests = \
	rwii-setw \
	orri-rf2rf \
	orri-rf2rd \
	orri-rd2rf \

ddfii_bare_tests = $(addprefix ddfii-bare-, $(ddfii_sc_tests))
ddfii_qemu_tests = $(addprefix ddfii-qemu-, $(ddfii_sc_tests))
