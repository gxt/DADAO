
default: all

src_dir ?= .

#--------------------------------------------------------------------
# Sources
#--------------------------------------------------------------------

bmarks = \
	dhrystone \

#--------------------------------------------------------------------
# Build rules
#--------------------------------------------------------------------
include common.mk

DADAO_ELF_GCC_EXTRA_FLAGS	:= -DPREALLOCATE=1 -std=gnu99 -ffast-math -fno-common -fno-builtin-printf -fno-tree-loop-distribute-patterns -Xassembler --multiple-to-single
DADAO_ELF_LINK_EXTRA_FLAGS	:= -T $(src_dir)/common/benchmarks.ld

incs  += -I$(src_dir)/../env -I$(src_dir)/common $(addprefix -I$(src_dir)/, $(bmarks))
objs  :=

define compile_template
$(1).dadao: $(wildcard $(src_dir)/$(1)/*) $(wildcard $(src_dir)/common/*)
	$$(DADAO_ELF_GCC) $$(incs) $$(DADAO_ELF_GCC_FLAGS) -o $$@ $(wildcard $(src_dir)/$(1)/*.c) $(wildcard $(src_dir)/common/*.c) $(wildcard $(src_dir)/common/*.S)
endef

$(foreach bmark,$(bmarks),$(eval $(call compile_template,$(bmark))))

#------------------------------------------------------------
# Build and run benchmarks on dadao simulator

bmarks_dadao_bin  = $(addsuffix .dadao,  $(bmarks))
bmarks_dadao_dump = $(addsuffix .dadao.dump, $(bmarks))
bmarks_dadao_out  = $(addsuffix .dadao.out,  $(bmarks))

$(bmarks_dadao_dump): %.dadao.dump: %.dadao
	$(DADAO_ELF_OBJDUMP) $(DADAO_ELF_OBJDUMP_FLAGS) $< > $@

$(bmarks_dadao_out): %.dadao.out: %.dadao
	$(DADAO_SIM) $< > $@

dadao: $(bmarks_dadao_dump)
run: $(bmarks_dadao_out)

junk += $(bmarks_dadao_bin) $(bmarks_dadao_dump) $(bmarks_dadao_hex) $(bmarks_dadao_out)

#------------------------------------------------------------
# Default

all: dadao

#------------------------------------------------------------
# Clean up

clean:
	rm -rf $(objs) $(junk)
