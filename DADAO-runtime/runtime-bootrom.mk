
RUNTIME_BOOTROM_SOURCE		:= $(DIR_DADAO_TOP)/DADAO-runtime/bootrom/
RUNTIME_BOOTROM_TARGET		:= $(DIR_DADAO_TARGET)/bootrom/
RUNTIME_BOOTROM_LOG			:= $(DIR_DADAO_LOG)/runtime-bootrom.log

runtime-bootrom-clean:
	@rm -fr $(RUNTIME_BOOTROM_TARGET)

runtime-bootrom-prepare:
	# TARGET DIR: $(RUNTIME_BOOTROM_TARGET)
	@mkdir -p $(RUNTIME_BOOTROM_TARGET)
	@ln -s -t $(RUNTIME_BOOTROM_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(RUNTIME_BOOTROM_TARGET) $(RUNTIME_BOOTROM_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>> $(RUNTIME_BOOTROM_TARGET)/Makefile
	@echo "include common.mk"									>> $(RUNTIME_BOOTROM_TARGET)/Makefile
	@echo "include bootrom.mk"									>> $(RUNTIME_BOOTROM_TARGET)/Makefile

runtime-bootrom-build:
	@make $(__VAR_DADAO_PATH__) -C $(RUNTIME_BOOTROM_TARGET) default

runtime-bootrom-highfive:	dadao-before-highfive
	@test ! -f $(RUNTIME_BOOTROM_LOG) || mv --force $(RUNTIME_BOOTROM_LOG) $(RUNTIME_BOOTROM_LOG).last
	@echo "=== runtime-bootrom-highfive log file: $(RUNTIME_BOOTROM_LOG)"	| tee -a $(RUNTIME_BOOTROM_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
	@make runtime-bootrom-clean								>> $(RUNTIME_BOOTROM_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
#	@make runtime-bootrom-source							>> $(RUNTIME_BOOTROM_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
	@make runtime-bootrom-prepare							>> $(RUNTIME_BOOTROM_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
	@make runtime-bootrom-build								>> $(RUNTIME_BOOTROM_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
#	@make runtime-bootrom-install							>> $(RUNTIME_BOOTROM_LOG) 2>&1
	@echo "--- runtime-bootrom-highfive DONE! ===           at `date +%T`"	| tee -a $(RUNTIME_BOOTROM_LOG)
