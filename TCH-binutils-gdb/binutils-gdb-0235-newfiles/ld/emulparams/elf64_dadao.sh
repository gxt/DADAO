TEMPLATE_NAME=elf
GENERATE_SHLIB_SCRIPT=yes
ELFSIZE=64
SCRIPT_NAME=elf
OUTPUT_FORMAT="elf64-dadao"
NO_REL_RELOCS=yes
ENTRY=_start

# Default to 0
TEXT_START_ADDR='DEFINED (__.DADAO.start..text) ? __.DADAO.start..text : 0x400000'
# Don't add SIZEOF_HEADERS.
# Don't set EMBEDDED, that would be misleading; it's not that kind of system.
TEXT_BASE_ADDRESS=$TEXT_START_ADDR
DATA_ADDR='DEFINED (__.DADAO.start..data) ? __.DADAO.start..data : 0x1000000'

MAXPAGESIZE="CONSTANT (MAXPAGESIZE)"
ARCH=dadao
MACHINE=
COMPILE_IN=yes
EXTRA_EM_FILE=dadaoelf

# FIXME: Also bit by the PROVIDE bug?  If not, this could be
# EXECUTABLE_SYMBOLS.
# By default, put the high end of the stack where the register stack
# begins.  They grow in opposite directions.  */
OTHER_SYMBOLS="PROVIDE (__Stack_start = 0x8000000);"
