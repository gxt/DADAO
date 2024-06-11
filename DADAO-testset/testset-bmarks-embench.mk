#
# Makefile for embench
#
TESTSET_EMBENCH_GITHUB		:= https://github.com/embench/embench-iot.git
TESTSET_EMBENCH_NEWFILES	:= $(DIR_DADAO_TOP)/DADAO-testset/embench-iot
TESTSET_EMBENCH_SOURCE		:= $(DIR_DADAO_SOURCE)/embench-iot
TESTSET_EMBENCH_BUILD		:= $(DIR_DADAO_BUILD)/embench-build
TESTSET_EMBENCH_LOG			:= $(DIR_DADAO_LOG)/testset-embench-qemu-log
TESTSET_EMBENCH_QEMU_LOG	:= $(DIR_DADAO_LOG)/testset-embench-qemu.log

testset-embench-qemu-clean:
	# Remove old embench source dir ...
	@rm -fr $(TESTSET_EMBENCH_SOURCE)
	# Remove old build dir ...
	@rm -fr $(TESTSET_EMBENCH_BUILD)
	# Remove old log dir ...
	@rm -fr $(TESTSET_EMBENCH_LOG)

testset-embench-qemu-source:
	# Clone official repo
	@$(__VAR_LOCAL_GIT__) git clone -q $(TESTSET_EMBENCH_GITHUB) -- $(TESTSET_EMBENCH_SOURCE)
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

testset-embench-qemu-build:
	$(TESTSET_EMBENCH_SOURCE)/build_all.py		\
		--builddir $(TESTSET_EMBENCH_BUILD)		\
		--logdir $(TESTSET_EMBENCH_LOG)			\
		--arch dadao							\
		--cc $(DADAO_ELF_GCC)					\
		--cflags="-c"							\
		--user-libs="-lm"

testset-embench-qemu-runtest:
	$(TESTSET_EMBENCH_SOURCE)/benchmark_size.py	\
		--builddir $(TESTSET_EMBENCH_BUILD)		\
		--logdir $(TESTSET_EMBENCH_LOG)
	# SOMETHING ERROR for benchmark_speed
	-$(TESTSET_EMBENCH_SOURCE)/benchmark_speed.py	\
		--builddir $(TESTSET_EMBENCH_BUILD)			\
		--logdir $(TESTSET_EMBENCH_LOG)				\
		--target-module run_qemu

testset-embench-qemu-highfive:	dadao-before-highfive
	@test ! -f $(TESTSET_EMBENCH_QEMU_LOG) || mv --force $(TESTSET_EMBENCH_QEMU_LOG) $(TESTSET_EMBENCH_QEMU_LOG).last
	@echo "=== testset-embench-qemu-highfive log file: $(TESTSET_EMBENCH_QEMU_LOG)"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
	@make testset-embench-qemu-clean						>> $(TESTSET_EMBENCH_QEMU_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
	@make testset-embench-qemu-source						>> $(TESTSET_EMBENCH_QEMU_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
#	@make testset-embench-qemu-prepare						>> $(TESTSET_EMBENCH_QEMU_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
	@make testset-embench-qemu-build						>> $(TESTSET_EMBENCH_QEMU_LOG) 2>&1
	@echo "--- 5. Runtest                                   at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
	@make testset-embench-qemu-runtest						>> $(TESTSET_EMBENCH_QEMU_LOG) 2>&1
	@echo "--- testset-embench-qemu-highfive DONE! ===      at `date +%T`"	| tee -a $(TESTSET_EMBENCH_QEMU_LOG)
