bootrom_img = bootrom.dadao.img
bootrom_dump = bootrom.dadao.dump

default: img dump

img: $(bootrom_img)

dump: $(bootrom_dump)

%.img: %.elf
	$(DADAO_ELF_OBJCOPY) -O binary --change-addresses=-0x10000 $< $@

%.elf: %.S linker.ld
	$(DADAO_ELF_GCC) -I$(DIR_DADAO_TARGET)/include -Tlinker.ld $< -nostdlib -static -o $@

%.dump: %.elf
	$(DADAO_ELF_OBJDUMP) -d $< > $@

clean:
	rm -f *.elf *.dump *.img
