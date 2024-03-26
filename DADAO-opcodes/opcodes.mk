PYTHON			:= /usr/bin/python3

OPCODES_SRC_DIR		:= $(DIR_DADAO_TOP)/DADAO-opcodes
OPCODES_DST_DIR		?= $(DIR_DADAO_TARGET)/__opcodes/

OPCODES_PARSE		:= $(OPCODES_DST_DIR)/parse-opcodes.py
OPCODES_INPUT		:= $(OPCODES_DST_DIR)/simrisc.opcodes
OPCODES_OUTPUT_QEMU	:= $(OPCODES_DST_DIR)/output.qemu
OPCODES_OUTPUT_BIN	:= $(OPCODES_DST_DIR)/output.binutils
OPCODES_OUTPUT_ENV	:= $(OPCODES_DST_DIR)/output.encoding
OPCODES_OUTPUT_DIS      := $(OPCODES_DST_DIR)/output.disassemble
OPCODES_OUTPUT_BITPAT   := $(OPCODES_DST_DIR)/output.bitpat
OPCODES_OUTPUT_TRACER	:= $(OPCODES_DST_DIR)/instructions.py

opcodes-clean:
	@rm -fr $(OPCODES_DST_DIR)

opcodes-source:
	@make -s opcodes-clean
	@mkdir -p $(OPCODES_DST_DIR)
	@ln --symbolic --target-directory=$(OPCODES_DST_DIR) $(OPCODES_SRC_DIR)/*

opcodes-qemu:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@rm -f $(OPCODES_OUTPUT_QEMU)
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --decode $(OPCODES_OUTPUT_QEMU)

opcodes-binutils:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@rm -f $(OPCODES_OUTPUT_BIN)
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --opc $(OPCODES_OUTPUT_BIN)

opcodes-disassemble:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@rm -f $(OPCODES_OUTPUT_DIS)
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --disassemble $(OPCODES_OUTPUT_DIS)

opcodes-encoding:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@rm -f $(OPCODES_OUTPUT_ENV)
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --encoding $(OPCODES_OUTPUT_ENV)

opcodes-bitpat:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@rm -f $(OPCODES_OUTPUT_BITPAT)
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --bitpat $(OPCODES_OUTPUT_BITPAT)

opcodes-tracer:
	@test -d $(OPCODES_DST_DIR) || make -s opcodes-source
	@$(PYTHON) $(OPCODES_PARSE) --input $(OPCODES_INPUT) --tracer $(OPCODES_OUTPUT_TRACER)

opcodes-highfive:
	@make -s opcodes-clean
	@make -s opcodes-source
	@make -s opcodes-qemu
	@make -s opcodes-binutils
	@make -s opcodes-disassemble
	@make -s opcodes-encoding
	@make -s opcodes-bitpat
#	@make -s opcodes-tracer

