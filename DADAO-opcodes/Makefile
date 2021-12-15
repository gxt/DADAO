all:
	@echo ""
	@echo "Use m4 to generate opcodes"
	@echo ""

M4			:= m4
GEN_OPCODES		:= gen-opcodes-binutils.m4
TARGET_OPCODES		?= target-opcodes.opc

opcodes-binutils:
	$(M4) $(GEN_OPCODES)	> $(TARGET_OPCODES)

PYTHON			:= /usr/bin/python3
# copy from qemu-0600/scripts/decodetree.py
DECODETREE		?= decodetree-0600.py
TARGET_DECODE_QEMU	?= $(shell pwd)/target-decode.inc.c

decode-qemu: simrisc.decode $(DECODETREE)
	$(PYTHON) $(DECODETREE) --decode disas_dadao -o $(TARGET_DECODE_QEMU) $<
