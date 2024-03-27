default: test.exe test.elf test.disa

%.exe:	%.c
	$(DADAO_ELF_GCC) $< -save-temps -o $@

%.elf: %.exe
	$(DADAO_ELF_READELF) -a $< > $@

%.disa: %.exe
	$(DADAO_ELF_OBJDUMP) -lDS $< > $@
