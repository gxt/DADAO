bootrom_img = bootrom.img
bootrom_dump = bootrom.dump

GCC=dadao-linux-elf-gcc
CFLAGS_DADAO=
OBJCOPY=dadao-linux-elf-objcopy
OBJDUMP=dadao-linux-elf-objdump

default: img dump

img: $(bootrom_img)

dump: $(bootrom_dump)

%.img: %.elf
	$(OBJCOPY) -O binary --change-addresses=-0x10000 $< $@

%.elf: %.S linker.ld
	$(GCC) $(CFLAGS_DADAO) -Tlinker.ld $< -nostdlib -static -o $@

%.dump: %.elf
	$(OBJDUMP) -d $< > $@

clean:
	rm -f *.elf *.dump *.img
