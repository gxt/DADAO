M4		:= m4
OPCODES_TARGET	?= opcodes-output

all:
	@echo ""
	@echo "Use m4 to generate opcodes"
	@echo ""

opcodes-binutils:
	$(M4) gen-opcodes-binutils.m4	> $(OPCODES_TARGET)
