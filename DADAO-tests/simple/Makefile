include $(src_dir)/../common.mk

default: min.ll min.bc min.s min.o min.dump min.disa

%.ll: %.c
	$(DADAO_LLVM_CLANG) -target dadao -S -emit-llvm $< -o $@

%.bc: %.ll
	$(DADAO_LLVM_AS) $< -o $@

%.s: %.bc
	$(DADAO_LLVM_LLC) $< -o $@

%.o: %.s
	$(DADAO_LLVM_MC) $< -triple dadao -filetype=obj -o $@

%.dump: %.o
	$(DADAO_LLVM_READOBJ) --all $< > $@

%.disa: %.o
	$(DADAO_LLVM_OBJDUMP) --full-contents --disassemble $< > $@
