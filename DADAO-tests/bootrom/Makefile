bootrom_img = bootrom.dadao.img
bootrom_dump = bootrom.dadao.dump

include $(src_dir)/../common.mk

default: img dump

img: $(bootrom_img)

dump: $(bootrom_dump)

%.img: %.elf
	$(DADAO_ELF_OBJCOPY) -O binary --change-addresses=-0x10000 $< $@

%.elf: %.S linker.ld
	$(DADAO_ELF_GCC) -Tlinker.ld $< -nostdlib -static -o $@

%.dump: %.elf
	$(DADAO_ELF_OBJDUMP) -d $< > $@

clean:
	rm -f *.elf *.dump *.img
