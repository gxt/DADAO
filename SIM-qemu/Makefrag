#
# Makefile for simulator
#
QEMU_0600_LOCAL		:= /pub/GITLAB/qemu-project/qemu.git
QEMU_0600_GITLAB	:= https://gitlab.com/qemu-project/qemu.git
QEMU_0600_VERSION	:= v6.0.1
QEMU_0600_BRANCH	:= dadao-0600
QEMU_0600_TARGETS	:= dadao-softmmu,dadao-linux-user

QEMU_0600_NEWFILES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0600-newfiles
QEMU_0600_PATCHES	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0600-patches
QEMU_0600_FIXUPS	:= $(DIR_DADAO_TOP)/SIM-qemu/qemu-0600-fixups

QEMU_0600_SOURCE	:= $(DIR_DADAO_SOURCE)/qemu-0600
QEMU_0600_DECODETREE	:= $(QEMU_0600_SOURCE)/scripts/decodetree.py
QEMU_0600_DECODEFILE	:= $(QEMU_0600_SOURCE)/target/dadao/decode-dadao.inc.c
QEMU_0600_DISASFILE	:= $(QEMU_0600_SOURCE)/disas/dadao.h
QEMU_0600_BUILD		:= $(DIR_DADAO_BUILD)/qemu-0600
QEMU_0600_INSTALL	?= $(DIR_DADAO_INSTALL)
#QEMU_0600_INSTALL	?= $(DIR_DADAO_BUILD)/__qemu-0600
QEMU_0600_LOG_STDOUT	:= $(DIR_DADAO_LOG)/qemu-0600.out
QEMU_0600_LOG_STDERR	:= $(DIR_DADAO_LOG)/qemu-0600.err

qemu-0600-clean:
	@echo "Remove old qemu source dir ..."
	@rm -fr $(QEMU_0600_SOURCE)
	@echo "Remove old qemu build dir ..."
	@rm -fr $(QEMU_0600_BUILD)
ifneq ($(QEMU_0600_INSTALL), $(DIR_DADAO_INSTALL))
	@echo "Remove old qemu install dir ..."
	@rm -fr $(QEMU_0600_INSTALL)
endif

qemu-0600-source:
	@test -d $(DIR_DADAO_SOURCE) || mkdir -p $(DIR_DADAO_SOURCE)
	@rm -fr $(QEMU_0600_SOURCE)
ifeq ($(wildcard $(QEMU_0600_LOCAL)),)
	@echo "Clone remote repo"
	@git clone -q $(QEMU_0600_GITLAB) -- $(QEMU_0600_SOURCE)
else
	@echo "Clone local repo"
	@git clone -q $(QEMU_0600_LOCAL) -- $(QEMU_0600_SOURCE)
endif
	@cd $(QEMU_0600_SOURCE); git checkout -qb $(QEMU_0600_BRANCH) $(QEMU_0600_VERSION)
	# include and arch-init
	@echo "INCLUDE and arch-init patches"
	@cd $(QEMU_0600_SOURCE); git am $(QEMU_0600_PATCHES)/0*.patch
	# linux-user
	@echo "LINUX-USER: Copy linux-user/dadao dir"
	@cp -a $(QEMU_0600_NEWFILES)/linux-user/* $(QEMU_0600_SOURCE)/linux-user/
	@cd $(QEMU_0600_SOURCE);					\
		git add linux-user/dadao;				\
		git commit -sm"DADAO-linux-user: add dadao support"
	@echo "LINUX-USER: apply patches"
	@cd $(QEMU_0600_SOURCE); git am $(QEMU_0600_PATCHES)/1*.patch
	# target
	@echo "TARGET: Copy target/dadao dir"
	@cp -a $(QEMU_0600_NEWFILES)/target/* $(QEMU_0600_SOURCE)/target/
	@echo "TARGET-DECODE: Generate dadao decodetree file"
	@make -s -C $(DIR_DADAO_TOP) opcodes-qemu
	@$(PYTHON) $(QEMU_0600_DECODETREE) --decode disas_dadao -o $(QEMU_0600_DECODEFILE) $(OPCODES_OUTPUT_QEMU)
	@cd $(QEMU_0600_SOURCE);					\
		git add target/dadao;					\
		git commit -sm"DADAO-target: add dadao support"
	# disas
	@cp -a $(QEMU_0600_NEWFILES)/disas/* $(QEMU_0600_SOURCE)/disas/
	@make -s -C $(DIR_DADAO_TOP) opcodes-disassemble
	@cp $(OPCODES_OUTPUT_DIS) $(QEMU_0600_DISASFILE)
	@cd $(QEMU_0600_SOURCE);					\
		git add disas;						\
		git commit -sm"DADAO: add disas support"
	# hw
	@echo "HW: Copy hw/dadao dir"
	@cp -a $(QEMU_0600_NEWFILES)/hw/* $(QEMU_0600_SOURCE)/hw/
	@cd $(QEMU_0600_SOURCE);					\
		git add hw/dadao;						\
		git commit -sm"DADAO-hw: add dadao support"
	# default-configs
	@echo "DEFAULT-CONFIGS: Copy config files"
	@cp -a $(QEMU_0600_NEWFILES)/default-configs/devices/* $(QEMU_0600_SOURCE)/default-configs/devices/
	@cp -a $(QEMU_0600_NEWFILES)/default-configs/targets/* $(QEMU_0600_SOURCE)/default-configs/targets/
	@cd $(QEMU_0600_SOURCE);					\
		git add default-configs;				\
		git commit -sm"DADAO-default-configs: add dadao support"
	# root config
	@echo "CONFIG: Apply patches"
	@cd $(QEMU_0600_SOURCE); git am $(QEMU_0600_PATCHES)/9*.patch
	# FIXUP
	#@cd $(QEMU_0600_SOURCE); git am $(QEMU_0600_FIXUPS)/*.patch

qemu-0600-build-new:
	@rm -fr $(QEMU_0600_BUILD)
	@mkdir -p $(QEMU_0600_BUILD)
	@cd $(QEMU_0600_BUILD) &&					\
		$(QEMU_0600_SOURCE)/configure				\
			--target-list=$(QEMU_0600_TARGETS)		\
			--prefix=$(QEMU_0600_INSTALL)			\
			--disable-sdl					\
			--disable-capstone				\
			--disable-slirp					\
			--disable-fdt					\
			--enable-debug					\
			--enable-curses

qemu-0600-build:
	@make -C $(QEMU_0600_BUILD) -j8

qemu-0600-install:
	@echo "Make install ..."
	@make -C $(QEMU_0600_BUILD) install

qemu-0600-highfive:
	@echo "BEGIN TO BUILD qemu-0600                         at `date +%T`"
	@echo "0. Remove old logfiles                           at `date +%T`"
	@test -d $(DIR_DADAO_LOG) || mkdir -p $(DIR_DADAO_LOG)
	@rm -fr $(QEMU_0600_LOG_STDOUT) $(QEMU_0600_LOG_STDERR)
	@echo "1. Clean old dirs                                at `date +%T`"
	@make -s qemu-0600-clean				1>> $(QEMU_0600_LOG_STDOUT) 2>> $(QEMU_0600_LOG_STDERR)
	@echo "2. New source                                    at `date +%T`"
	@make -s qemu-0600-source				1>> $(QEMU_0600_LOG_STDOUT) 2>> $(QEMU_0600_LOG_STDERR)
	@echo "3. Configure                                     at `date +%T`"
	@make -s qemu-0600-build-new				1>> $(QEMU_0600_LOG_STDOUT) 2>> $(QEMU_0600_LOG_STDERR)
	@echo "4. Build                                         at `date +%T`"
	@make -s qemu-0600-build				1>> $(QEMU_0600_LOG_STDOUT) 2>> $(QEMU_0600_LOG_STDERR)
	@echo "5. Install                                       at `date +%T`"
	@make -s qemu-0600-install				1>> $(QEMU_0600_LOG_STDOUT) 2>> $(QEMU_0600_LOG_STDERR)
	@echo "BUILD qemu-0600 DONE!                            at `date +%T`"

qemu-0600-tags:
	@make -C $(QEMU_0600_SOURCE) ctags

qemu-dejagnu-prepare:
	@mkdir -p $(DIR_DADAO_INSTALL)/usr/bin
	@mkdir -p $(DIR_DADAO_INSTALL)/usr/share/dejagnu/baseboards
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/bin/ /usr/bin/runtest
	@ln -s $(DIR_DADAO_INSTALL)/bin/qemu-dadao $(DIR_DADAO_INSTALL)/usr/bin/dadao-unknown-elf-run
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/share/dejagnu/			\
		/usr/share/dejagnu/*.c						\
		/usr/share/dejagnu/*.exp					\
		/usr/share/dejagnu/libexec					\
		/usr/share/dejagnu/config
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/share/dejagnu/baseboards/		\
		/usr/share/dejagnu/baseboards/basic-sim.exp			\
		$(DIR_DADAO_TOP)/SIM-qemu/qemu-dadao.exp
