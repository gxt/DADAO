#=======================================================================
# Makefrag for ddrb tests
#-----------------------------------------------------------------------

ddrb_sc_tests = \
	orri-rx2rx \
	orrr-add \
	orrr-cmp \
	orrr-sub \
	riii-adrp \
	riii-brxx \
	rrii-addi \
	rrii-brxx \
	rwii-andnw \
	rwii-orw \
	rwii-setzw \

ddrb_bare_tests = $(addprefix ddrb-bare-, $(ddrb_sc_tests))
ddrb_qemu_tests = $(addprefix ddrb-qemu-, $(ddrb_sc_tests))
