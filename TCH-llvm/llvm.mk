#
# Makefile for llvm-project
#
LLVM_1600_GITHUB	:= https://github.com/llvm/llvm-project.git
LLVM_1600_VERSION	:= llvmorg-16.0.0
LLVM_1600_BRANCH	:= dadao-1600

LLVM_1600_NEWFILES	:= $(DIR_DADAO_TOP)/TCH-llvm/llvm-1600-newfiles
LLVM_1600_PATCHES	:= $(DIR_DADAO_TOP)/TCH-llvm/llvm-1600-patches
LLVM_1600_FIXUPS	:= $(DIR_DADAO_TOP)/TCH-llvm/llvm-1600-fixups

LLVM_1600_SOURCE	:= $(DIR_DADAO_SOURCE)/llvm-1600
LLVM_1600_BUILD		:= $(DIR_DADAO_BUILD)/llvm-1600
LLVM_1600_INSTALL	?= $(DIR_DADAO_INSTALL)
LLVM_1600_LOG		:= $(DIR_DADAO_LOG)/llvm-1600.log

llvm-1600-clean:
	@echo "Remove old llvm source dir ..."
	@rm -fr $(LLVM_1600_SOURCE)
	@echo "Remove old llvm build dir ..."
	@rm -fr $(LLVM_1600_BUILD)

llvm-1600-source:
	@rm -fr $(LLVM_1600_SOURCE)
	# Clone remote repo
	@$(__VAR_L__) git clone -q $(LLVM_1600_GITHUB) -- $(LLVM_1600_SOURCE)
	# Specify version
	@cd $(LLVM_1600_SOURCE); git checkout -qb $(LLVM_1600_BRANCH) $(LLVM_1600_VERSION)
	# clang newfiles
	@cp -a $(LLVM_1600_NEWFILES)/clang/* $(LLVM_1600_SOURCE)/clang/
	@cd $(LLVM_1600_SOURCE);					\
		git add clang;						\
		git commit -sm"DADAO: add clang support"
	# llvm newfiles
	@cp -a $(LLVM_1600_NEWFILES)/llvm/* $(LLVM_1600_SOURCE)/llvm/
	@cd $(LLVM_1600_SOURCE);					\
		git add llvm;						\
		git commit -sm"DADAO: add llvm support"
	# Patches
	@cd $(LLVM_1600_SOURCE); test ! -d $(LLVM_1600_PATCHES) || git am $(LLVM_1600_PATCHES)/*.patch
	# Fixups
	@cd $(LLVM_1600_SOURCE); test ! -d $(LLVM_1600_FIXUPS) || git am $(LLVM_1600_FIXUPS)/*.patch

llvm-1600-prepare:
	@rm -fr $(LLVM_1600_BUILD)
	@mkdir -p $(LLVM_1600_BUILD)
	# NOTE: $(LLVM_1600_SOURCE)/build is softlink to $(LLVM_1600_BUILD)
	@ln -s $(LLVM_1600_BUILD) $(LLVM_1600_SOURCE)/build
	@cmake -S $(LLVM_1600_SOURCE)/llvm -B $(LLVM_1600_BUILD) -G Ninja	\
		-DCMAKE_BUILD_TYPE=Release					\
		-DCMAKE_INSTALL_PREFIX=$(LLVM_1600_INSTALL)			\
		-DLLVM_TARGETS_TO_BUILD="Dadao"					\
		-DLLVM_ENABLE_PROJECTS="clang;lld"				\
		-DLLVM_BUILD_RUNTIMES=OFF					\
		-DLLVM_INCLUDE_TESTS=ON						\
		-DLLVM_INCLUDE_EXAMPLES=OFF					\
		-DLLVM_ENABLE_BACKTRACES=OFF					\
		-DLLVM_ENABLE_ASSERTIONS=ON					\
		-DLLVM_OPTIMIZED_TABLEGEN=ON					\
		-DLLVM_USE_SPLIT_DWARF=ON					\
		-DLLVM_PARALLEL_COMPILE_JOBS=$(__VAR_C__)		\
		-DLLVM_USE_LINKER=mold

llvm-1600-build:
	@cd $(LLVM_1600_BUILD); cmake --build .

llvm-1600-install:
	@cd $(LLVM_1600_BUILD); cmake --install .

llvm-1600-highfive:	dadao-before-highfive
	@test ! -f $(LLVM_1600_LOG) || mv --force $(LLVM_1600_LOG) $(LLVM_1600_LOG).last
	@echo "=== llvm-1600-highfive log file: $(LLVM_1600_LOG)"				| tee -a $(LLVM_1600_LOG)
	@echo "--- 1. Clean                                     at `date +%T`"	| tee -a $(LLVM_1600_LOG)
	@make llvm-1600-clean									>> $(LLVM_1600_LOG) 2>&1
	@echo "--- 2. Source                                    at `date +%T`"	| tee -a $(LLVM_1600_LOG)
	@make llvm-1600-source									>> $(LLVM_1600_LOG) 2>&1
	@echo "--- 3. Prepare                                   at `date +%T`"	| tee -a $(LLVM_1600_LOG)
	@make llvm-1600-prepare									>> $(LLVM_1600_LOG) 2>&1
	@echo "--- 4. Build                                     at `date +%T`"	| tee -a $(LLVM_1600_LOG)
	@make llvm-1600-build									>> $(LLVM_1600_LOG) 2>&1
	@echo "--- 5. Install                                   at `date +%T`"	| tee -a $(LLVM_1600_LOG)
	@make llvm-1600-install									>> $(LLVM_1600_LOG) 2>&1
	@echo "--- llvm-1600-highfive DONE! ===                 at `date +%T`"	| tee -a $(LLVM_1600_LOG)

llvm-1600-check-llvm:
	@ninja -C $(LLVM_1600_BUILD) check-llvm

llvm-1600-check-lit:
	@ $(LLVM_1600_BUILD)/bin/llvm-lit $(LLVM_1600_SOURCE)/llvm/test/ -v --vg --vg-leak > $(LLVM_1600_SOURCE)/llvm/test/lit.sum

###
LLVM_1600_TABLEGEN	:= $(LLVM_1600_BUILD)/NATIVE/bin/llvm-tblgen			\
				-I $(LLVM_1600_SOURCE)/llvm/lib/Target/Dadao/		\
				-I $(LLVM_1600_BUILD)/include/				\
				-I $(LLVM_1600_SOURCE)/llvm/include/			\
				-I $(LLVM_1600_SOURCE)/llvm/lib/Target/			\
				$(LLVM_1600_SOURCE)/llvm/lib/Target/Dadao/Dadao.td

llvm-1600-tablegen:
	# -print-enum samples
	@$(LLVM_1600_TABLEGEN)	-print-enums -class=RegisterClass
	@$(LLVM_1600_TABLEGEN)	-print-enums -class=Register
	# output files in $(DIR_DADAO_TARGET)
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenAsmMatcher.inc		-gen-asm-matcher
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenAsmWriter.inc		-gen-asm-writer
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenCallingConv.inc		-gen-callingconv
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenDAGISel.inc		-gen-dag-isel
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenDisassemblerTables.inc	-gen-disassembler
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenInstrInfo.inc		-gen-instr-info
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenMCCodeEmitter.inc		-gen-emitter
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenRegisterInfo.inc		-gen-register-info
	@$(LLVM_1600_TABLEGEN) -o $(DIR_DADAO_TARGET)/DadaoGenSubtargetInfo.inc		-gen-subtarget

