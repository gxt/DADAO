
#define HAVE_ARCH_PLTENTER
#define HAVE_ARCH_PLTEXIT

#include <elf/sotruss-lib.c>

ElfW(Addr)
la_dadao_gnu_pltenter (ElfW(Sym) *sym __attribute__ ((unused)),
			 unsigned int ndx __attribute__ ((unused)),
			 uintptr_t *refcook, uintptr_t *defcook,
			 La_dadao_regs *regs, unsigned int *flags,
			 const char *symname, long int *framesizep)
{
  print_enter (refcook, defcook, symname,
	       regs->lr_reg[0], regs->lr_reg[1], regs->lr_reg[2],
	       *flags);

  /* No need to copy anything, we will not need the parameters in any case.  */
  *framesizep = 0;

  return sym->st_value;
}

unsigned int
la_dadao_gnu_pltexit (ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
			uintptr_t *defcook,
			const struct La_dadao_regs *inregs,
			struct La_dadao_retval *outregs, const char *symname)
{
  print_exit (refcook, defcook, symname, outregs->lrv_reg[0]);

  return 0;
}
