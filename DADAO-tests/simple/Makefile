include $(src_dir)/../common.mk

default: min.ll min.s

%.ll: %.c
	$(DADAO_LLVM_CLANG) -cc1 -emit-llvm $< -o $@

%.s: %.ll
	$(DADAO_LLVM_LLC) $< -o $@
