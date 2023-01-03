#=======================================================================
# Makefile for DADAO-tests/isa
#-----------------------------------------------------------------------

src_dir ?= .

include $(src_dir)/../Makerules
include $(src_dir)/dduii/Makefrag

default: all

vpath %.S $(src_dir)

#------------------------------------------------------------
# Build assembly tests

%.dump: %
	$(DADAO_ELF_OBJDUMP) $(DADAO_ELF_OBJDUMP_FLAGS) $< > $@

%.out: %
	$(DADAO_QEMU_USER) $< 2> $@

define compile_template

$$($(1)_p_tests): $(1)-p-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(src_dir)/../env/p -I$(src_dir)/macros/scalar -T$(src_dir)/../env/p/link.ld $$< -o $$@
$(1)_tests += $$($(1)_p_tests)

$$($(1)_v_tests): $(1)-v-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -O2 -I$(src_dir)/../env/v -I$(src_dir)/macros/scalar -T$(src_dir)/../env/v/link.ld $(src_dir)/../env/v/entry.S $(src_dir)/../env/v/*.c $$< -o $$@
$(1)_tests += $$($(1)_v_tests)

$$($(1)_qemu_tests): $(1)-qemu-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(src_dir)/../env/qemu -I$(src_dir)/macros/scalar $$< -o $$@
$(1)_tests += $$($(1)_qemu_tests)

$(1)_tests_dump = $$(addsuffix .dump, $$($(1)_tests))

$(1): $$($(1)_tests_dump)

.PHONY: $(1)

COMPILER_SUPPORTS_$(1) := $$(shell $$(DADAO_ELF_GCC) $(2) -c -x c /dev/null -o /dev/null 2> /dev/null; echo $$$$?)

ifeq ($$(COMPILER_SUPPORTS_$(1)),0)
tests += $$($(1)_tests)
endif

endef

$(eval $(call compile_template,dduii, ))

tests_dump = $(addsuffix .dump, $(tests))
tests_hex = $(addsuffix .hex, $(tests))
tests_out = $(addsuffix .out, $(filter dd%,$(tests)))

run: $(tests_out)

junk += $(tests) $(tests_dump) $(tests_hex) $(tests_out)

#------------------------------------------------------------
# Default

all: $(tests_dump)

#------------------------------------------------------------
# Clean up

clean:
	rm -rf $(junk)
