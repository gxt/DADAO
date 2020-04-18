TEMPLATE_NAME=elf32
GENERATE_SHLIB_SCRIPT=yes
ELFSIZE=64
SCRIPT_NAME=elf
OUTPUT_FORMAT="elf64-dadao"
NO_REL_RELOCS=yes
ENTRY=_start.

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

# The existence of a symbol __start (or _start) should overrule Main, so
# it can be a user symbol without the associated dadaoal magic.  We
# also want to provide Main as a synonym for _start, if Main wasn't
# defined but is referred to, and _start was defined.
#
# The reason we use a symbol "_start." as a mediator is to avoid
# causing ld to force the type of _start to object rather than no
# type, which confuses disassembly; we also make it alphanumerically
# a successor of _start for similar reasons.  Perhaps it's a linker
# bug that linker-defined symbols set the symbol-type.
#
# Note that we smuggle this into OTHER_TEXT_SECTIONS (at the end
# of .text) rather than TEXT_START_SYMBOLS.  This is necessary, as
# DEFINED wouldn't find the symbol if it was at the top; presumably
# before the definition, if the definition is not in the first file.
# FIXME: Arguably a linker bug.
# Only do this for a final link, or else we'll mess up e.g. error
# messages.
OTHER_TEXT_SECTIONS="
${RELOCATING+
 _start. = (DEFINED (_start) ? _start
	    : (DEFINED (Main) ? Main : (DEFINED (.text) ? .text : 0)));
 PROVIDE (Main = DEFINED (Main) ? Main : (DEFINED (_start) ? _start : _start.));
}"

OTHER_SECTIONS='
 .DADAO.reg_contents :
 {
   /* Note that this section always has a fixed VMA - that of its
      first register * 8.  */
   *(.DADAO.reg_contents.linker_allocated);
   *(.DADAO.reg_contents);
 }
'

# FIXME: Also bit by the PROVIDE bug?  If not, this could be
# EXECUTABLE_SYMBOLS.
# By default, put the high end of the stack where the register stack
# begins.  They grow in opposite directions.  */
OTHER_SYMBOLS="PROVIDE (__Stack_start = 0x8000000);"
