#
# Makefile for tests
#

TESTSET_SIMPLE_SOURCE			:= $(DIR_DADAO_TOP)/DADAO-testset/simple
TESTSET_SIMPLE_TARGET_LLVM		:= $(DIR_DADAO_TARGET)/simple-llvm
TESTSET_SIMPLE_TARGET_GCC_ELF	:= $(DIR_DADAO_TARGET)/simple-gcc-elf

testset-simple-llvm-highfive:
	#
	# TARGET_LLVM DIR: $(TESTSET_SIMPLE_TARGET_LLVM)
	#
	@rm -fr $(TESTSET_SIMPLE_TARGET_LLVM)
	@mkdir -p $(TESTSET_SIMPLE_TARGET_LLVM)
	@ln -s -t $(TESTSET_SIMPLE_TARGET_LLVM) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_SIMPLE_TARGET_LLVM) $(TESTSET_SIMPLE_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_SIMPLE_TARGET_LLVM)/Makefile
	@echo "include common.mk"									>> $(TESTSET_SIMPLE_TARGET_LLVM)/Makefile
	@echo "include simple-llvm.mk"								>> $(TESTSET_SIMPLE_TARGET_LLVM)/Makefile
	@echo "include simple-qemu.mk"								>> $(TESTSET_SIMPLE_TARGET_LLVM)/Makefile
	@$(DADAO_MAKE) -C $(TESTSET_SIMPLE_TARGET_LLVM) default
	@$(DADAO_MAKE) -C $(TESTSET_SIMPLE_TARGET_LLVM) test.qemulog

testset-simple-gcc-elf-highfive:
	#
	# TARGET_GCC_ELF DIR: $(TESTSET_SIMPLE_TARGET_GCC_ELF)
	#
	@rm -fr $(TESTSET_SIMPLE_TARGET_GCC_ELF)
	@mkdir -p $(TESTSET_SIMPLE_TARGET_GCC_ELF)
	@ln -s -t $(TESTSET_SIMPLE_TARGET_GCC_ELF) $(RUNTIME_COMMON_MK)
	@ln -s -t $(TESTSET_SIMPLE_TARGET_GCC_ELF) $(TESTSET_SIMPLE_SOURCE)/*
	@echo "DIR_DADAO_TOP\t\t\t:= $(DIR_DADAO_TOP)"				>  $(TESTSET_SIMPLE_TARGET_GCC_ELF)/Makefile
	@echo "include common.mk"									>> $(TESTSET_SIMPLE_TARGET_GCC_ELF)/Makefile
	@echo "include simple-gcc-elf.mk"							>> $(TESTSET_SIMPLE_TARGET_GCC_ELF)/Makefile
	@echo "include simple-qemu.mk"								>> $(TESTSET_SIMPLE_TARGET_GCC_ELF)/Makefile
	@$(DADAO_MAKE) -C $(TESTSET_SIMPLE_TARGET_GCC_ELF) default
	@$(DADAO_MAKE) -C $(TESTSET_SIMPLE_TARGET_GCC_ELF) test.qemulog

