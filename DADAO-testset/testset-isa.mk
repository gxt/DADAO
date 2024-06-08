TESTSET_ISA_SOURCE			:= $(DIR_DADAO_TOP)/DADAO-testset/isa
TESTSET_ISA_BARE_TARGET		:= $(DIR_DADAO_TARGET)/testset-isa-bare
TESTSET_ISA_BARE_LOG		:= $(DIR_DADAO_LOG)/testset-isa-bare.log

testset-isa-bare-clean:
	#
	# TARGET DIR: $(TESTSET_ISA_BARE_TARGET)
	#
	@rm -fr $(TESTSET_ISA_BARE_TARGET)

testset-isa-bare-source:
	@mkdir -p $(TESTSET_ISA_BARE_TARGET)
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(TESTSET_ISA_SOURCE)/*
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(CHIPYARD_0107_BUILD)/output
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_ISA_BARE_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_ISA_BARE_TARGET)/Makefile
	@echo "include isa.mk"										>> $(TESTSET_ISA_BARE_TARGET)/Makefile

testset-isa-bare-build:
	@make $(__VAR_P__) -C $(TESTSET_ISA_BARE_TARGET) ddrd-bare
	@make $(__VAR_P__) -C $(TESTSET_ISA_BARE_TARGET) ddrb-bare
#	@make $(__VAR_P__) -C $(TESTSET_ISA_BARE_TARGET) ddrf-bare

testset-isa-bare-runtest:
	@make $(__VAR_P__) -C $(TESTSET_ISA_BARE_TARGET) run-bare

testset-isa-bare-highfive:	dadao-before-highfive
	@test ! -f $(TESTSET_ISA_BARE_LOG) || mv --force $(TESTSET_ISA_BARE_LOG) $(TESTSET_ISA_BARE_LOG).last
	@echo "=== testset-isa-bare-highfive log file: $(TESTSET_ISA_BARE_LOG)"	| tee -a $(TESTSET_ISA_BARE_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)
	@make testset-isa-bare-clean							>> $(TESTSET_ISA_BARE_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)
	@make testset-isa-bare-source							>> $(TESTSET_ISA_BARE_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)
#	@make testset-isa-bare-prepare							>> $(TESTSET_ISA_BARE_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)
	@make testset-isa-bare-build							>> $(TESTSET_ISA_BARE_LOG) 2>&1
	@echo "--- 5. Runtest                                   at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)
	@make testset-isa-bare-runtest							>> $(TESTSET_ISA_BARE_LOG) 2>&1
	@echo "--- testset-isa-bare-highfive DONE! ===          at `date +%T`"	| tee -a $(TESTSET_ISA_BARE_LOG)

TESTSET_ISA_QEMU_TARGET		:= $(DIR_DADAO_TARGET)/testset-isa-qemu
TESTSET_ISA_QEMU_LOG		:= $(DIR_DADAO_LOG)/testset-isa-qemu.log

testset-isa-qemu-clean:
	#
	# TARGET DIR: $(TESTSET_ISA_QEMU_TARGET)
	#
	@rm -fr $(TESTSET_ISA_QEMU_TARGET)

testset-isa-qemu-source:
	@mkdir -p $(TESTSET_ISA_QEMU_TARGET)
	@ln -s -t $(TESTSET_ISA_QEMU_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_ISA_QEMU_TARGET) $(TESTSET_ISA_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_ISA_QEMU_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_ISA_QEMU_TARGET)/Makefile
	@echo "include isa.mk"										>> $(TESTSET_ISA_QEMU_TARGET)/Makefile

testset-isa-qemu-build:
	@make $(__VAR_P__) -C $(TESTSET_ISA_QEMU_TARGET) ddrd-qemu
	@make $(__VAR_P__) -C $(TESTSET_ISA_QEMU_TARGET) ddrb-qemu
	@make $(__VAR_P__) -C $(TESTSET_ISA_QEMU_TARGET) ddrf-qemu

testset-isa-qemu-runtest:
	@make $(__VAR_P__) -C $(TESTSET_ISA_QEMU_TARGET) run-qemu

testset-isa-qemu-highfive:	dadao-before-highfive
	@test ! -f $(TESTSET_ISA_QEMU_LOG) || mv --force $(TESTSET_ISA_QEMU_LOG) $(TESTSET_ISA_QEMU_LOG).last
	@echo "=== testset-isa-qemu-highfive log file: $(TESTSET_ISA_QEMU_LOG)"	| tee -a $(TESTSET_ISA_QEMU_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
	@make testset-isa-qemu-clean							>> $(TESTSET_ISA_QEMU_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
	@make testset-isa-qemu-source							>> $(TESTSET_ISA_QEMU_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
#	@make testset-isa-qemu-prepare							>> $(TESTSET_ISA_QEMU_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
	@make testset-isa-qemu-build							>> $(TESTSET_ISA_QEMU_LOG) 2>&1
	@echo "--- 5. Runtest                                   at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
	@make testset-isa-qemu-runtest							>> $(TESTSET_ISA_QEMU_LOG) 2>&1
	@echo "--- testset-isa-qemu-highfive DONE! ===          at `date +%T`"	| tee -a $(TESTSET_ISA_QEMU_LOG)
