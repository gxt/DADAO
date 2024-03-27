#
# Makefile for coremark
#
TESTSET_COREMARK_GITHUB	:= https://github.com/eembc/coremark.git
TESTSET_COREMARK_TARGET	:= $(DIR_DADAO_TARGET)/testset-coremark-qemu

testset-coremark-qemu-clean:
	# Remove old coremark source dir ...
	@rm -fr $(TESTSET_COREMARK_TARGET)

testset-coremark-qemu-source:
	# Clone official repo
	@git clone -q $(TESTSET_COREMARK_GITHUB) -- $(TESTSET_COREMARK_TARGET)

testset-coremark-qemu-build:
	@cd $(TESTSET_COREMARK_TARGET);							\
		PORT_DIR=linux										\
		CC=$(DADAO_ELF_GCC)									\
		XCFLAGS="-static -O0"								\
		NO_LIBRT=true										\
		make compile

testset-coremark-qemu-run:
	@$(DADAO_QEMU_USER)										\
		-D $(TESTSET_COREMARK_TARGET)/coremark.qemulog		\
		$(TESTSET_COREMARK_TARGET)/coremark.exe

testset-coremark-qemu-highfive:
	@make -s testset-coremark-qemu-clean
	@make -s testset-coremark-qemu-source
	@make -s testset-coremark-qemu-build
	@make -s testset-coremark-qemu-run
