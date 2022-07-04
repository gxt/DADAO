## Da Dao Zhi Jian: The Greatest TAO is Simplest

### Overview
DADAO-SimRISC project aims at teaching and scientific research as a whole. It is trying to design a RISC type instruction set system and hopes the system can offer guidance from the perspective of software development to the architecture design. The main content of the project is to develop a series of system-softwares supporting SimRISC, involving toolchain (compiler, binary tools, libraries), simulation environment, operating system kernel and other components.
### Prerequisites
Before the installation starts, we need to make sure that the operating system has enough support for the software system. Here goes the details:
##### 1. Source repository control
It will be good for you to modify the Makefile files in the DADAO directory to individualize your installation. But it is not bad if you want to use the default installation plan, except that you will be required to create a directory `/pub/GIT-ORIGIN`.

`/pub/GIT-ORIGIN` is created to store the source of the softwares needed by this project. In this directory, the source files should be organised into code repositories, and maintained by the famous version control utility: `Git` . As a matter of fact, OSS (Open Source Software) is the basis of this project, each SimRISC related developments are made on OSS frameworks. By using `Git`, it is possible to fetch the source of the software we need from OSS repository managing platform, and by using the `Git` command line option `'--bare'`, constructing a temperary OSS mirror becomes feasible.

Initialize the keeper directory for the OSS source repositories. If you don't have the authorization, creating it elsewhere is feasible too. No matter where it is going to be set, in order to facilitate the project to work smoothly, all of the bare git repos will be **DOWNLOADED** here, and it is a must that the top `Makefile` maintain the changes.
```shell
$ mkdir -p /pub/GIT-ORIGIN
$ cd /pub/GIT-ORIGIN
```

Downloaded git repos for Toolchain and system softwares, including `gcc`, `binutils`, `glibc`, `newlib` ... Remeber to use the git option `'--bare'` to download the git repos.

**`binutils` for binary tools:** linker, loader, assembler, readelf, objdump...
```shell
$ git clone https://github.com/bminor/binutils-gdb.git --bare
```

**`gcc` for compiler:** preprocessor, frontend, backend, runtime, crts...
```shell
$ git clone https://github.com/gcc-mirror/gcc.git  --bare
```

**`newlib` and `glibc` for libc:** standard libraries
```shell
$ git clone https://github.com/mirror/newlib-cygwin.git --bare
$ git clone https://github.com/lattera/glibc.git --bare
```

**`linux` for headers:** linux headers

```shell
$ git clone https://github.com/torvalds/linux.git --bare
```

**`qemu` and `qemu-project` for simulator:** qemu and other qemu-project tools
Except for the Qemu6.0 simulator itself, we still need to download other tools for its running. The required tools can be found at [qemu-project](https://gitlab.com/qemu-project), we need 7 of them here: `capstone`, `dtc`, `meson`, `libslirp`, `keycodemapdb`, `berkeley-softfloat-3`, `berkeley-testfloat-3`. The recommended way to download these tools is the same as how we download other OSS source.
```shell
$ git clone https://gitlab.com/qemu-project/qemu.git --bare
$ mkdir /pub/GIT-ORIGIN/qemu-project
$ cd /pub/GIT-ORIGIN/qemu-project
$ git clone ... --bare
```

##### 2. Software dependency for Toolchain
You can use `apt-get`  to install some necessary softwares if you are using a Ubuntu Linux OS:
```shell
$ sudo apt-get install build-essential flex bison m4 libtool texinfo
```
Expect for these, `autoconf2.64` is required by `Newlib` and `autoconf2.69` is required by `GCC` and `Binutils`. In addition, GCC requires `gmp`, `mpc`, `mpfr`:
```shell
$ sudo apt-get install autoconf2.64 autoconf2.69
$ sudo apt-get install libgmp-dev libmpc-dev libmpfr-dev
```

##### 3. Software dependency for Simulator
`Qemu6.0` is a simulator to simulate SimRISC hardware, some necessary softwares are needed: 
```shell
$ sudo apt-get install pkg-config libglib2.0-dev ninja-build libpixman-1-dev libncurses-dev
```

### Install
The installation of the DADAO-SimRISC project v0.2 now basically covers two part:
1. DADAO-SimRISC ELF/GNU Toolchain
2. DADAO-SimRISC Simulator
#### Install DADAO-SimRISC ELF/GNU Toolchain
DADAO-SimRISC ELF/GNU Toolchain includes:
1. **GCC** (GNU Compiler Collection) as Compiler.
2. **Binutils-gdb** (GDB not included) as Binary-Analysis Tools.
3. **Newlib-cygwin** (for ELF) as OS Standard Function Library.
4. **Glibc** (for GNU) as OS Standard Function Library.

To ensure the DADAO-SimRISC toolchain works just fine, we need to install each part of the toolchain in a specific order. The Compiler part has dependency on the Binary part for producing the internal function library **libgcc**, which is required by the compiler collection (`GCC` not `cc1`) itself. So we will have to Install the Binary part **Binutils-gdb** first, then the Compiler part: **GCC**, then the Library part: **Newlib-cygwin**.

##### General install commands
`dadao-linux-elf` : support static-linking objects 
```shell
$ make tch-elf-highfive
```

`dadao-linux-gnu` : support shared libraries and dynamic linking 
```shell
$ make tch-gnu-highfive
```

To be frank, the composition of GCC spans all aspects of toolchain softwares. It not only includes Linker (`ld`) and Assembler (`as`), which from the Binary part, but Internals (`libgcc`), Translator (`cc1`) and Preprocessor, which from the Compiler part. To some certain extent, the standard libraries are involved either, since some of the runtime libraries will use standard library functions to implement certain features. 

**Therefore, to build the toolchain smoothly, two options are considered here: **

1. **dadao-linux-elf** : **Binutils-gdb**, **GCC** on bootstrap method, **Newlib-Cygwin**.

	In that order, we only need to build GCC for once, since this toolchain is implemented for embedded systems, and requires less extra support from the compiler-builtin-functions.

```shell
$ make binutils-gdb-0235-highfive BINUTILS_GDB_0235_TARGET=dadao-linux-elf
$ make gcc-1003-highfive GCC_1003_TARGET=dadao-linux-elf
$ make newlib-cygwin-0303-highfive
```

2. **dadao-linux-gnu** :  **Binutils-gdb**, **GCC** on bootstrap method, **Glibc**, **GCC** with dependency. 

	in that order, we will build GCC for twice, because some internal function libraries like libatomic and libquadmaths define a series of compiler-builtin-functions, and these builtins use functions from the standard library (Glibc)

```shell
$ make binutils-gdb-0235-highfive
$ make gcc-1003-highfive
$ make glibc-0231-highfive
$ make gcc-1003-highfive-again
```

##### Problems encountered ?
It is normal to have problems during the installing procedure of the toolchain. For sure you may run the make commands one by one according to the Makefile and receive feedbacks from `cmd`,  but if you want to record the warning or error msgs, feel free to check the docs in `(pwd)/__log`  after any make command for installing is executed.
### Install DADAO-SimRISC Simulator
```shell
$ make qemu-0600-highfive
```
### Run your project
The end product is listed in `(pwd)/__install/bin` . Normally we will use the files in `(pwd)/DADAO-bench`  to test and make use of the DADAO-SimRISC system software.

Feel free to read and modify the `Makefile` files in DADAO-bench, because it will be helpful if you want to individualize your test against the toolchain or other system-softwares. Here goes a case:

```shell
$ make bench-helloworld-elf bench-helloworld-qemu

	# the make command and detail operations have been leave
	# out from the command line output, check that elsewhere

hello world!
```

