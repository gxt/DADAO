#=======================================================================
# Makefrag for ddrb tests
#-----------------------------------------------------------------------

ddrb_sc_tests = \
	orri-rx2rx \
	rwii-andnw rwii-orw rwii-setzw \
	orrr-add orrr-sub \
	rrii-addi \
	orrr-cmp \
	riii-adrp \
	riii-brxx \
	rrii-brxx \

ddrb_bare_tests = $(addprefix ddrb-bare-, $(ddrb_sc_tests))
ddrb_qemu_tests = $(addprefix ddrb-qemu-, $(ddrb_sc_tests))
