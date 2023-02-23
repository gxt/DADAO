#include <asm/vmlinux.lds.h>
#include <asm/page.h>
#include <asm/cache.h>
#include <asm/thread_info.h>

OUTPUT_ARCH(dadao)
ENTRY(_start)

jiffies = jiffies_64;

SECTIONS
{
	. = PAGE_OFFSET;

	_start = .;

	__init_begin = .;
	HEAD_TEXT_SECTION
	INIT_TEXT_SECTION(PAGE_SIZE)
	INIT_DATA_SECTION(16)
	PERCPU_SECTION(L1_CACHE_BYTES)
	__init_end = .;

	.text : {
		_text = .;
		_stext = .;
		TEXT_TEXT
		SCHED_TEXT
		CPUIDLE_TEXT
		LOCK_TEXT
		*(.fixup)
		_etext = .;
	}

	_sdata = .;
	RO_DATA_SECTION(PAGE_SIZE)
	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
	_edata = .;

	EXCEPTION_TABLE(L1_CACHE_BYTES)
	NOTES

	BSS_SECTION(0, 0, 0)

	_end = .;

	STABS_DEBUG
	DWARF_DEBUG

	DISCARDS
}
