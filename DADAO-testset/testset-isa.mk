TESTSET_ISA_SOURCE			:= $(DIR_DADAO_TOP)/DADAO-testset/isa
TESTSET_ISA_BARE_TARGET		:= $(DIR_DADAO_TARGET)/testset-isa-bare
TESTSET_ISA_QEMU_TARGET		:= $(DIR_DADAO_TARGET)/testset-isa-qemu

testset-isa-bare-highfive:	runtime-common-headers
	#
	# TARGET DIR: $(TESTSET_ISA_BARE_TARGET)
	# NOTE: CAN ONLY RUN ONE BY ONE, so add j1 option for make
	#
	@rm -fr $(TESTSET_ISA_BARE_TARGET)
	@mkdir -p $(TESTSET_ISA_BARE_TARGET)
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(TESTSET_ISA_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_ISA_BARE_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_ISA_BARE_TARGET)/Makefile
	@echo "include isa.mk"										>> $(TESTSET_ISA_BARE_TARGET)/Makefile
	@$(DADAO_MAKE) -j1 -C $(TESTSET_ISA_BARE_TARGET) dduii-bare
	@$(DADAO_MAKE) -j1 -C $(TESTSET_ISA_BARE_TARGET) run-bare

testset-isa-qemu-highfive:	runtime-common-headers
	#
	# TARGET DIR: $(TESTSET_ISA_QEMU_TARGET)
	# NOTE: CAN ONLY RUN ONE BY ONE, so add j1 option for make
	#
	@rm -fr $(TESTSET_ISA_QEMU_TARGET)
	@mkdir -p $(TESTSET_ISA_QEMU_TARGET)
	@ln -s -t $(TESTSET_ISA_QEMU_TARGET) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_ISA_BARE_TARGET) $(TESTSET_ISA_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_ISA_QEMU_TARGET)/Makefile
	@echo "include common.mk"									>> $(TESTSET_ISA_BARE_TARGET)/Makefile
	@echo "include isa.mk"										>> $(TESTSET_ISA_BARE_TARGET)/Makefile
	@$(DADAO_MAKE) -j1 -C $(TESTSET_ISA_QEMU_TARGET) dduii-qemu
	@$(DADAO_MAKE) -j1 -C $(TESTSET_ISA_QEMU_TARGET) run-qemu

