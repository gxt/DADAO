#=======================================================================
# Makefile for DADAO-tests/isa
#-----------------------------------------------------------------------

src_dir ?= .

include $(src_dir)/../common.mk
include $(src_dir)/dduii/Makefrag

default: all

vpath %.S $(src_dir)

#------------------------------------------------------------
# Build assembly tests

%.dump: %
	$(DADAO_ELF_OBJDUMP) $(DADAO_ELF_OBJDUMP_FLAGS) $< > $@

dduii-bare%.out: dduii-bare%
	@echo -n "$<	"
	@make -C $(DIR_DADAO_TOP) CHIPYARD_0107_DADAO_BINARY=$(shell pwd)/$< chipyard-0107-run-binary	> /dev/null
	@cp $(DIR_DADAO_BUILD)/chipyard-0107/output/chipyard.TestHarness.Wuming1StageConfig/$@ .
	@tail -n 1 $@

dduii-qemu%.out: dduii-qemu%
	$(DADAO_QEMU_USER) $< 2> $@
	@echo "		$<"

define compile_template

$$($(1)_bare_tests): $(1)-bare-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(src_dir)/../env/bare -I$(src_dir)/macros/scalar -T$(src_dir)/../env/bare/link.ld $$< -o $$@
$(1)_tests += $$($(1)_bare_tests)

$$($(1)_v_tests): $(1)-v-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -O2 -I$(src_dir)/../env/v -I$(src_dir)/macros/scalar -T$(src_dir)/../env/v/link.ld $(src_dir)/../env/v/entry.S $(src_dir)/../env/v/*.c $$< -o $$@
$(1)_tests += $$($(1)_v_tests)

$$($(1)_qemu_tests): $(1)-qemu-%: $(1)/%.S
	$$(DADAO_ELF_GCC) $(2) $$(DADAO_ELF_GCC_FLAGS) -I$(src_dir)/../env/qemu -I$(src_dir)/macros/scalar -T$(src_dir)/../env/qemu/link.ld $$< -o $$@
$(1)_tests += $$($(1)_qemu_tests)

$(1)_tests_dump = $$(addsuffix .dump, $$($(1)_tests))

$(1): $$($(1)_tests_dump)

$(1)_bare_tests_dump = $$(addsuffix .dump, $$($(1)_bare_tests))

$(1)-bare: $$($(1)_bare_tests_dump)

$(1)_qemu_tests_dump = $$(addsuffix .dump, $$($(1)_qemu_tests))

$(1)-qemu: $$($(1)_qemu_tests_dump)

.PHONY: $(1)

COMPILER_SUPPORTS_$(1) := $$(shell $$(DADAO_ELF_GCC) $(2) -c -x c /dev/null -o /dev/null 2> /dev/null; echo $$$$?)

ifeq ($$(COMPILER_SUPPORTS_$(1)),0)
tests += $$($(1)_tests)
endif

endef

$(eval $(call compile_template,dduii, -Xassembler --multiple-to-single ))

tests_dump = $(addsuffix .dump, $(tests))
tests_hex = $(addsuffix .hex, $(tests))
tests_bare_out = $(addsuffix .out, $(filter dduii-bare%,$(tests)))
tests_qemu_out = $(addsuffix .out, $(filter dduii-qemu%,$(tests)))

run-bare: $(tests_bare_out)
run-qemu: $(tests_qemu_out)

junk += $(tests) $(tests_dump) $(tests_hex) $(tests_bare_out) $(tests_qemu_out)

#------------------------------------------------------------
# Default

all: $(tests_dump)

#------------------------------------------------------------
# Clean up

clean:
	rm -rf $(junk)
