#=======================================================================
# Makefrag for ddfii tests
#-----------------------------------------------------------------------

ddfii_sc_tests = \
	rwii-setw \
	rf2rf \
	rf2rd \
	rd2rf \

#	ft2rd \

ddfii_bare_tests = $(addprefix ddfii-bare-, $(ddfii_sc_tests))
ddfii_qemu_tests = $(addprefix ddfii-qemu-, $(ddfii_sc_tests))
#ddfii_v_tests = $(addprefix ddfii-v-, $(ddfii_sc_tests))
