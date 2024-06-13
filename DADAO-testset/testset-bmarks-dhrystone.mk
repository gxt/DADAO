TESTSET_DHRYSTONE_BARE_SOURCE	:= $(DIR_DADAO_TOP)/DADAO-testset/dhrystone-bare/
TESTSET_DHRYSTONE_BARE_TARGET	:= $(DIR_DADAO_TARGET)/testset-dhrystone-bare/
TESTSET_DHRYSTONE_BARE_LOG		:= $(DIR_DADAO_LOG)/testset-dhrystone-bare.log

testset-dhrystone-bare-clean:
	@rm -fr $(TESTSET_DHRYSTONE_BARE_TARGET)

testset-dhrystone-bare-source:
	@mkdir -p $(TESTSET_DHRYSTONE_BARE_TARGET)
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(TESTSET_DHRYSTONE_BARE_SOURCE)/*
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(CHIPYARD_0111_BUILD)/output
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile
	@echo "include dhrystone.mk"								>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile

testset-dhrystone-bare-build:
	@make $(__VAR_DADAO_PATH__) -C $(TESTSET_DHRYSTONE_BARE_TARGET) default

testset-dhrystone-bare-runtest-stage1:
	# dhrystone run on bare DO output error, could be ignored
	@-make $(__VAR_DADAO_PATH__) $(__VAR_BARE_STAGE1__) -C $(DIR_DADAO_TOP)						\
		RUNTIME_BARE_BINARY=$(TESTSET_DHRYSTONE_BARE_TARGET)/dhrystone.dadao		\
		runtime-bare-run-binary

testset-dhrystone-bare-runtest-stage5:
	# dhrystone run on bare DO output error, could be ignored
	@-make $(__VAR_DADAO_PATH__) $(__VAR_BARE_STAGE5__) -C $(DIR_DADAO_TOP)						\
		RUNTIME_BARE_BINARY=$(TESTSET_DHRYSTONE_BARE_TARGET)/dhrystone.dadao		\
		runtime-bare-run-binary

testset-dhrystone-bare-highfive:	dadao-before-highfive
	@test ! -f $(TESTSET_DHRYSTONE_BARE_LOG) || mv --force $(TESTSET_DHRYSTONE_BARE_LOG) $(TESTSET_DHRYSTONE_BARE_LOG).last
	@echo "=== testset-dhrystone-bare-highfive log file: $(TESTSET_DHRYSTONE_BARE_LOG)"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@make testset-dhrystone-bare-clean						>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@make testset-dhrystone-bare-source						>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
#	@make testset-dhrystone-bare-prepare					>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@make testset-dhrystone-bare-build						>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- 5.1. Runtest stage1                          at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@make testset-dhrystone-bare-runtest-stage1				>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- 5.2. Runtest stage5                          at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)
	@make testset-dhrystone-bare-runtest-stage5				>> $(TESTSET_DHRYSTONE_BARE_LOG) 2>&1
	@echo "--- testset-dhrystone-bare-highfive DONE! ===    at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_BARE_LOG)

TESTSET_DHRYSTONE_QEMU_SOURCE	:= $(DIR_DADAO_TOP)/DADAO-testset/dhrystone-qemu/
TESTSET_DHRYSTONE_QEMU_TARGET	:= $(DIR_DADAO_TARGET)/testset-dhrystone-qemu/
TESTSET_DHRYSTONE_QEMU_LOG		:= $(DIR_DADAO_LOG)/testset-dhrystone-qemu.log

testset-dhrystone-qemu-clean:
	@rm -fr $(TESTSET_DHRYSTONE_QEMU_TARGET)

testset-dhrystone-qemu-build:
	@mkdir -p $(TESTSET_DHRYSTONE_QEMU_TARGET)
	@cd $(TESTSET_DHRYSTONE_QEMU_TARGET) ;								\
		$(DADAO_ELF_GCC)												\
			-static														\
			-save-temps													\
			-DHZ=250													\
			-DTIME														\
			-o $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry					\
			$(TESTSET_DHRYSTONE_QEMU_SOURCE)/dhry_1.c					\
			$(TESTSET_DHRYSTONE_QEMU_SOURCE)/dhry_2.c
	@$(DADAO_ELF_READELF) -a $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry		> $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry.elf
	@$(DADAO_ELF_OBJDUMP) -lDS $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry	> $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry.dump

testset-dhrystone-qemu-runtest:
	@echo 10 | $(DADAO_QEMU_USER)										\
		-singlestep -strace												\
		-D $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhrystone.qemulog			\
		$(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry

testset-dhrystone-qemu-highfive:	dadao-before-highfive
	@test ! -f $(TESTSET_DHRYSTONE_QEMU_LOG) || mv --force $(TESTSET_DHRYSTONE_QEMU_LOG) $(TESTSET_DHRYSTONE_QEMU_LOG).last
	@echo "=== testset-dhrystone-qemu-highfive log file: $(TESTSET_DHRYSTONE_QEMU_LOG)"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
	@make testset-dhrystone-qemu-clean						>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
#	@make testset-dhrystone-qemu-source						>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
#	@make testset-dhrystone-qemu-prepare					>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
	@make testset-dhrystone-qemu-build						>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@echo "--- 5. Runtest                                   at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
	@make testset-dhrystone-qemu-runtest					>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@echo "--- testset-dhrystone-qemu-highfive DONE! ===    at `date +%T`"	| tee -a $(TESTSET_DHRYSTONE_QEMU_LOG)
