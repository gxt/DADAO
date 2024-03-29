TESTSET_DHRYSTONE_BARE_SOURCE	:= $(DIR_DADAO_TOP)/DADAO-testset/dhrystone-bare/
TESTSET_DHRYSTONE_BARE_TARGET	:= $(DIR_DADAO_TARGET)/testset-dhrystone-bare/
TESTSET_DHRYSTONE_QEMU_SOURCE	:= $(DIR_DADAO_TOP)/DADAO-testset/dhrystone-qemu/
TESTSET_DHRYSTONE_QEMU_TARGET	:= $(DIR_DADAO_TARGET)/testset-dhrystone-qemu/
TESTSET_DHRYSTONE_BARE_LOG		:= $(DIR_DADAO_LOG)/testset-dhrystone-bare.log
TESTSET_DHRYSTONE_QEMU_LOG		:= $(DIR_DADAO_LOG)/testset-dhrystone-qemu.log

testset-dhrystone-bare-highfive:	runtime-common-headers
	#
	# TARGET DIR: $(TESTSET_DHRYSTONE_BARE_TARGET)
	# NOTE: ONLY dhrystone here
	#
	@rm -fr $(TESTSET_DHRYSTONE_BARE_TARGET)
	@mkdir -p $(TESTSET_DHRYSTONE_BARE_TARGET)
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(TESTSET_DHRYSTONE_BARE_SOURCE)/*
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(CHIPYARD_0107_BUILD)/output
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile
	@echo "include dhrystone.mk"								>> $(TESTSET_DHRYSTONE_BARE_TARGET)/Makefile
	@$(DADAO_MAKE) -C $(TESTSET_DHRYSTONE_BARE_TARGET) default	>  $(TESTSET_DHRYSTONE_BARE_LOG)
	@$(DADAO_MAKE) -C $(DIR_DADAO_TOP)											\
		RUNTIME_BARE_BINARY=$(TESTSET_DHRYSTONE_BARE_TARGET)/dhrystone.dadao	\
		runtime-bare-run-binary									>> $(TESTSET_DHRYSTONE_BARE_LOG)
	@ln -s -t $(TESTSET_DHRYSTONE_BARE_TARGET) $(TESTSET_DHRYSTONE_BARE_LOG)

testset-dhrystone-qemu-highfive:
	@rm -fr $(TESTSET_DHRYSTONE_QEMU_TARGET)
	@mkdir -p $(TESTSET_DHRYSTONE_QEMU_TARGET)
	@cd $(TESTSET_DHRYSTONE_QEMU_TARGET) ;								\
		$(DADAO_ELF_GCC)												\
			-static														\
			-save-temps													\
			-DHZ=250													\
			-DTIME														\
			-o $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry					\
			$(TESTSET_DHRYSTONE_QEMU_SOURCE)/dhry_1.c					\
			$(TESTSET_DHRYSTONE_QEMU_SOURCE)/dhry_2.c					> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@$(DADAO_ELF_READELF) -a $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry		> $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry.elf
	@$(DADAO_ELF_OBJDUMP) -lDS $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry	> $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry.dump
	@echo 10 | $(DADAO_QEMU_USER)										\
		-singlestep -strace												\
		-D $(TESTSET_DHRYSTONE_QEMU_TARGET)/dhrystone.qemulog			\
		$(TESTSET_DHRYSTONE_QEMU_TARGET)/dhry							>> $(TESTSET_DHRYSTONE_QEMU_LOG) 2>&1
	@ln -s -t $(TESTSET_DHRYSTONE_QEMU_TARGET) $(TESTSET_DHRYSTONE_QEMU_LOG)

