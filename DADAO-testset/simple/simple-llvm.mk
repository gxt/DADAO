default: test.ll test.bc test.s test.o test.exe test.qemu

%.ll: %.c
	$(DADAO_LLVM_CLANG) -target dadao -S -emit-llvm \
		-I$(DIR_DADAO_INSTALL)/dadao-unknown-elf/include/ -D__DADAO__=1 $< -o $@

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

%.exe:	%.o
	$(DADAO_ELF_COLLECT2)											\
		-plugin $(DIR_DADAO_INSTALL)/libexec/gcc/dadao-unknown-elf/10.3.0/liblto_plugin.so		\
		-plugin-opt=$(DIR_DADAO_INSTALL)/libexec/gcc/dadao-unknown-elf/10.3.0/lto-wrapper		\
		-plugin-opt=-fresolution=/tmp/ccQddCQF.res							\
		-plugin-opt=-pass-through=-lgcc									\
		-plugin-opt=-pass-through=-lc									\
		-plugin-opt=-pass-through=-lgcc									\
		--defsym __.DADAO.start..text=0x400000								\
		-o $@												\
		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0/crti.o					\
		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0/crtbegin.o				\
		-L$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0						\
		-L$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0/../../../../dadao-unknown-elf/lib	\
		$< -lgcc -lc -lgcc										\
		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0/crtend.o					\
		$(DIR_DADAO_INSTALL)/lib/gcc/dadao-unknown-elf/10.3.0/crtn.o

%.lld:	%.o
	$(DADAO_LLVM_LLD) -nostdlib -nostartfiles -o $@ $<

%.qemu: %.exe
	$(DIR_DADAO_INSTALL)/bin/qemu-dadao $<
