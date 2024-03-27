#
# Makefile for embench
#
TESTSET_EMBENCH_GITHUB		:= https://github.com/embench/embench-iot.git
TESTSET_EMBENCH_NEWFILES	:= $(DIR_DADAO_TOP)/DADAO-testset/embench-iot
TESTSET_EMBENCH_SOURCE		:= $(DIR_DADAO_SOURCE)/embench-iot
TESTSET_EMBENCH_BUILD		:= $(DIR_DADAO_BUILD)/embench-build
TESTSET_EMBENCH_LOG			:= $(DIR_DADAO_LOG)/embench-log

testset-embench-qemu-clean:
	# Remove old embench source dir ...
	@rm -fr $(TESTSET_EMBENCH_SOURCE)
	# Remove old build dir ...
	@rm -fr $(TESTSET_EMBENCH_BUILD)
	# Remove old log dir ...
	@rm -fr $(TESTSET_EMBENCH_LOG)

testset-embench-qemu-source:
	# Clone official repo
	@git clone -q $(TESTSET_EMBENCH_GITHUB) -- $(TESTSET_EMBENCH_SOURCE)
	# Add dadao support
	@cp -a $(TESTSET_EMBENCH_NEWFILES)/config/dadao $(TESTSET_EMBENCH_SOURCE)/config
	@cd $(TESTSET_EMBENCH_SOURCE);				\
		git add .;								\
		git commit -sm"Add DADAO support"
	# Add run_qemu.py
	@cp -a $(TESTSET_EMBENCH_NEWFILES)/pylib/* $(TESTSET_EMBENCH_SOURCE)/pylib
	@cd $(TESTSET_EMBENCH_SOURCE);				\
		git add .;								\
		git commit -sm"Add run_qemu.py"

testset-embench-qemu-build-all:
	$(TESTSET_EMBENCH_SOURCE)/build_all.py		\
		--builddir $(TESTSET_EMBENCH_BUILD)		\
		--logdir $(TESTSET_EMBENCH_LOG)			\
		--arch dadao							\
		--cc $(DADAO_ELF_GCC)					\
		--cflags="-c"							\
		--user-libs="-lm"

testset-embench-qemu-benchmark-size:
	$(TESTSET_EMBENCH_SOURCE)/benchmark_size.py	\
		--builddir $(TESTSET_EMBENCH_BUILD)		\
		--logdir $(TESTSET_EMBENCH_LOG)

testset-embench-qemu-benchmark-speed:
	$(TESTSET_EMBENCH_SOURCE)/benchmark_speed.py	\
		--builddir $(TESTSET_EMBENCH_BUILD)			\
		--logdir $(TESTSET_EMBENCH_LOG)				\
		--target-module run_qemu

testset-embench-qemu-highfive:
	@make -s testset-embench-qemu-clean
	@make -s testset-embench-qemu-source
	@make -s testset-embench-qemu-build-all
	@make -s testset-embench-qemu-benchmark-size
	@make -s testset-embench-qemu-benchmark-speed
