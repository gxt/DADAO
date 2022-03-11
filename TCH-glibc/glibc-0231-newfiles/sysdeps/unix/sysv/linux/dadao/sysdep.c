#include <sysdep.h>
#include <errno.h>

/* This routine is jumped to by all the syscall handlers, to stash
   an error number into errno.  */
long __syscall_error (long err_no)
{
  __set_errno (err_no);
  return -1;
}
