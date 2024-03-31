#
# Makefile for simulator
#
QEMU_0801_GITLAB	:= https://gitlab.com/qemu-project/qemu.git
QEMU_0801_VERSION	:= v8.1.3
QEMU_0801_BRANCH	:= dadao-0801
QEMU_0801_TARGETS	:= dadao-softmmu,dadao-linux-user

QEMU_0801_NEWFILES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-newfiles
QEMU_0801_PATCHES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-patches
QEMU_0801_FIXUPS	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0801-fixups

QEMU_0801_SOURCE	:= $(DIR_DADAO_SOURCE)/qemu-0801
QEMU_0801_DECODEFILE	:= $(QEMU_0801_SOURCE)/target/dadao/insns.decode
QEMU_0801_DISASFILE	:= $(QEMU_0801_SOURCE)/disas/dadao.h
QEMU_0801_BUILD		:= $(DIR_DADAO_BUILD)/qemu-0801
QEMU_0801_INSTALL	?= $(DIR_DADAO_INSTALL)
#QEMU_0801_INSTALL	?= $(DIR_DADAO_BUILD)/__qemu-0801
QEMU_0801_LOG_STDOUT	:= $(DIR_DADAO_LOG)/qemu-0801.out
QEMU_0801_LOG_STDERR	:= $(DIR_DADAO_LOG)/qemu-0801.err

qemu-0801-clean:
	@echo "Remove old qemu source dir ..."
	@rm -fr $(QEMU_0801_SOURCE)
	@echo "Remove old qemu build dir ..."
	@rm -fr $(QEMU_0801_BUILD)
ifneq ($(QEMU_0801_INSTALL), $(DIR_DADAO_INSTALL))
	@echo "Remove old qemu install dir ..."
	@rm -fr $(QEMU_0801_INSTALL)
endif

qemu-0801-source:
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	@rm -fr $(QEMU_0801_SOURCE)
	# Clone source repo
	@git clone -q $(QEMU_0801_GITLAB) -- $(QEMU_0801_SOURCE)
	@cd $(QEMU_0801_SOURCE); git checkout -qb $(QEMU_0801_BRANCH) $(QEMU_0801_VERSION)
	# linux-user
	@cp -a $(QEMU_0801_NEWFILES)/linux-user/* $(QEMU_0801_SOURCE)/linux-user/
	@cd $(QEMU_0801_SOURCE);					\
		git add linux-user/dadao;				\
		git commit -sm"DADAO: add linux-user support"
	# target
	@cp -a $(QEMU_0801_NEWFILES)/target/* $(QEMU_0801_SOURCE)/target/
	@echo "TARGET-DECODE: Generate dadao decodetree file"
	@make -s -C $(DIR_DADAO_TOP) opcodes-qemu
	@cp $(OPCODES_OUTPUT_QEMU) $(QEMU_0801_DECODEFILE)
	@cd $(QEMU_0801_SOURCE);					\
		git add target/dadao;					\
		git commit -sm"DADAO: add target support"
	# disas
	@cp -a $(QEMU_0801_NEWFILES)/disas/* $(QEMU_0801_SOURCE)/disas/
	@make -s -C $(DIR_DADAO_TOP) opcodes-disassemble
	@cp $(OPCODES_OUTPUT_DIS) $(QEMU_0801_DISASFILE)
	@cd $(QEMU_0801_SOURCE);					\
		git add disas;						\
		git commit -sm"DADAO: add disas support"
	# hw
	@cp -a $(QEMU_0801_NEWFILES)/hw/* $(QEMU_0801_SOURCE)/hw/
	@cd $(QEMU_0801_SOURCE);					\
		git add hw/dadao;					\
		git commit -sm"DADAO: add hw support"
	# configs
	@cp -a $(QEMU_0801_NEWFILES)/configs/* $(QEMU_0801_SOURCE)/configs/
	@cd $(QEMU_0801_SOURCE);					\
		git add configs;					\
		git commit -sm"DADAO: add configs"
	# Patches
	@cd $(QEMU_0801_SOURCE); test ! -d $(QEMU_0801_PATCHES) || git am $(QEMU_0801_PATCHES)/*.patch
	# Fixups
	@cd $(QEMU_0801_SOURCE); test ! -d $(QEMU_0801_FIXUPS) || git am $(QEMU_0801_FIXUPS)/*.patch
	
qemu-0801-build-new:
	@rm -fr $(QEMU_0801_BUILD)
	@mkdir -p $(QEMU_0801_BUILD)
	@cd $(QEMU_0801_BUILD) &&					\
		$(QEMU_0801_SOURCE)/configure				\
			--target-list=$(QEMU_0801_TARGETS)		\
			--prefix=$(QEMU_0801_INSTALL)			\
			--disable-sdl					\
			--disable-capstone				\
			--disable-slirp					\
			--disable-fdt					\
			--enable-debug					\
			--enable-curses

qemu-0801-build:
	@make -C $(QEMU_0801_BUILD) -j8

qemu-0801-install:
	@echo "Make install ..."
	@make -C $(QEMU_0801_BUILD) install

qemu-0801-highfive:
	@echo "BEGIN TO BUILD qemu-0801                         at `date +%T`"
	@echo "0. Remove old logfiles                           at `date +%T`"
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@rm -fr $(QEMU_0801_LOG_STDOUT) $(QEMU_0801_LOG_STDERR)
	@echo "1. Clean old dirs                                at `date +%T`"
	@make -s qemu-0801-clean				1>> $(QEMU_0801_LOG_STDOUT) 2>> $(QEMU_0801_LOG_STDERR)
	@echo "2. New source                                    at `date +%T`"
	@make -s qemu-0801-source				1>> $(QEMU_0801_LOG_STDOUT) 2>> $(QEMU_0801_LOG_STDERR)
	@echo "3. Configure                                     at `date +%T`"
	@make -s qemu-0801-build-new				1>> $(QEMU_0801_LOG_STDOUT) 2>> $(QEMU_0801_LOG_STDERR)
	@echo "4. Build                                         at `date +%T`"
	@make -s qemu-0801-build				1>> $(QEMU_0801_LOG_STDOUT) 2>> $(QEMU_0801_LOG_STDERR)
	@echo "5. Install                                       at `date +%T`"
	@make -s qemu-0801-install				1>> $(QEMU_0801_LOG_STDOUT) 2>> $(QEMU_0801_LOG_STDERR)
	@echo "BUILD qemu-0801 DONE!                            at `date +%T`"

qemu-0801-tags:
	@make -C $(QEMU_0801_SOURCE) ctags
