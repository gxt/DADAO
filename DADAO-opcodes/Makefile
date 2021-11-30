M4		:= m4
OPCODES_TARGET	?= opcodes-output

all:
	@echo ""
	@echo "Use m4 to generate opcodes"
	@echo ""

opcodes-binutils:
	$(M4) gen-opcodes-binutils.m4	> $(OPCODES_TARGET)

PYTHON			:= /usr/bin/python3
# copy from qemu-0600/scripts/decodetree.py
DECODETREE		?= decodetree-0600.py
TARGET_DECODE_QEMU	?= $(shell pwd)/decode-dadao.inc.c

decode-qemu: dadao.decode $(DECODETREE)
	$(PYTHON) $(DECODETREE) --decode disas_dadao -o $(TARGET_DECODE_QEMU) $<
