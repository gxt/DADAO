#
# Makefile for mibench
#
TESTSET_MIBENCH_GITHUB      := https://github.com/ekut-es/mibench.git
TESTSET_MIBENCH_BRANCH      := mibench
TESTSET_MIBENCH_TARGET      := $(DIR_DADAO_TARGET)/testset-mibench-qemu
TESTSET_MIBENCH_PATCHES     := $(DIR_DADAO_TOP)/DADAO-testset/mibench-patches

testset-mibench-qemu-clean:
	# Remove old mibench dir ...
	@rm -fr $(TESTSET_MIBENCH_TARGET)

testset-mibench-qemu-source:
	# Remove old mibench source dir ...
	@test -d $(TESTSET_MIBENCH_TARGET) || mkdir -p $(TESTSET_MIBENCH_TARGET)
	# Clone official repo
	@$(__VAR_L__) git clone -q $(TESTSET_MIBENCH_GITHUB) -- $(TESTSET_MIBENCH_TARGET)
	@cd $(TESTSET_MIBENCH_TARGET); git checkout -qb $(TESTSET_MIBENCH_BRANCH)
	# Apply patches
	@cd $(TESTSET_MIBENCH_TARGET); git am $(TESTSET_MIBENCH_PATCHES)/*.patch

testset-mibench-qemu-build:
	@cd $(TESTSET_MIBENCH_TARGET);      	\
        make CC=$(DADAO_ELF_GCC) PLATFORM=dadao

testset-mibench-qemu-qemu:
	# TODO

testset-mibench-qemu-highfive:
	@make -s testset-mibench-qemu-clean
	@make -s testset-mibench-qemu-source
	@make -s testset-mibench-qemu-build
	@make -s testset-mibench-qemu-qemu
