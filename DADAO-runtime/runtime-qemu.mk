
runtime-qemu-dejagnu-prepare:
	@mkdir -p $(DIR_DADAO_INSTALL)/usr/bin
	@mkdir -p $(DIR_DADAO_INSTALL)/usr/share/dejagnu/baseboards
	@test -f $(DIR_DADAO_INSTALL)/bin/qemu-dadao || make -s -C $(DIR_DADAO_TOP) qemu-$(VER_QEMU)-highfive
	@ln -s $(DIR_DADAO_INSTALL)/bin/qemu-dadao $(DIR_DADAO_INSTALL)/usr/bin/dadao-unknown-elf-run
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/bin/ /usr/bin/runtest
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/share/dejagnu/					\
		/usr/share/dejagnu/*.c											\
		/usr/share/dejagnu/*.exp										\
		/usr/share/dejagnu/libexec										\
		/usr/share/dejagnu/config
	@ln -s -t $(DIR_DADAO_INSTALL)/usr/share/dejagnu/baseboards/		\
		/usr/share/dejagnu/baseboards/basic-sim.exp						\
		$(DIR_DADAO_TOP)/DADAO-runtime/qemu-dadao.exp

