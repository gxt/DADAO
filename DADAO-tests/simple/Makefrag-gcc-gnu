default: test.exe test.elf test.disa

%.exe:	%.c
	$(DADAO_GNU_GCC) $<														\
			-save-temps														\
			$(DIR_DADAO_INSTALL)/lib/gcc/dadao-linux-gnu/10.3.0/crti.o		\
			$(DIR_DADAO_INSTALL)/lib/gcc/dadao-linux-gnu/10.3.0/crtn.o		\
			-nostdlib -lgcc													\
			-B$(DIR_DADAO_BUILD)/gcc-$(VER_GCC)/gcc/						\
			-B$(DIR_DADAO_INSTALL)/dadao-linux-gnu/bin/						\
			-B$(DIR_DADAO_INSTALL)/dadao-linux-gnu/lib/						\
			-isystem $(DIR_DADAO_INSTALL)/dadao-linux-gnu/include			\
			-isystem $(DIR_DADAO_INSTALL)/dadao-linux-gnu/sys-include		\
			-o $@

%.elf: %.exe
	$(DADAO_GNU_READELF) -a $< > $@

%.disa: %.exe
	$(DADAO_GNU_OBJDUMP) -lDS $< > $@
