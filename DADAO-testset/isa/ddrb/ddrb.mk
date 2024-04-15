#=======================================================================
# Makefrag for ddrb tests
#-----------------------------------------------------------------------

ddrb_sc_tests = \
	orri-rb2rb \
	orri-rb2rd \
	orri-rd2rb \
	orrr-add \
	orrr-cmp \
	orrr-sub \
	riii-adrp \
	rrii-addi \
	rrii-breq \
	rrii-brne \
	rwii-andnw \
	rwii-orw \
	rwii-setzw \

ddrb_bare_tests = $(addprefix ddrb-bare-, $(ddrb_sc_tests))
ddrb_qemu_tests = $(addprefix ddrb-qemu-, $(ddrb_sc_tests))
