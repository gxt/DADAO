
RUNTIME_BOOTROM_SOURCE		:= $(DIR_DADAO_TOP)/DADAO-runtime/bootrom/
RUNTIME_BOOTROM_TARGET		:= $(DIR_DADAO_TARGET)/bootrom/

runtime-bootrom-highfive:
	#
	# TARGET DIR: $(RUNTIME_BOOTROM_TARGET)
	#
	@rm -fr $(RUNTIME_BOOTROM_TARGET)
	@mkdir -p $(RUNTIME_BOOTROM_TARGET)
	@ln -s -t $(RUNTIME_BOOTROM_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(RUNTIME_BOOTROM_TARGET) $(RUNTIME_INCLUDE_DIR)
	@ln -s -t $(RUNTIME_BOOTROM_TARGET) $(RUNTIME_BOOTROM_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>> $(RUNTIME_BOOTROM_TARGET)/Makefile
	@echo "include common.mk"									>> $(RUNTIME_BOOTROM_TARGET)/Makefile
	@echo "include bootrom.mk"									>> $(RUNTIME_BOOTROM_TARGET)/Makefile
	@$(DADAO_MAKE) -C $(RUNTIME_BOOTROM_TARGET)

