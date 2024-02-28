%.qemulog: %.exe
	$(DADAO_QEMU_USER) -d in_asm,cpu -D $@ $<

# -singlestep is deprecated, should be replace with -one-insn-per-tb
# -strace
