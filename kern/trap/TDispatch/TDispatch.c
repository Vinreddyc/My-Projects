#include <lib/syscall.h>

#include "import.h"

/** TASK 1:
  * * Dispatch the system call to appropriate functions based on syscall number.
  *   	- The function contracts are defined in import.h
  *   In case an invalid syscall number is provided, set the errno to: E_INVAL_CALLNR
  *   (error numbers defined in lib/syscall.h)
  *   
  */
void syscall_dispatch(void)
{
	// TODO
        unsigned int syscall_num = syscall_get_arg1();
        switch(syscall_num){
        case SYS_puts:
                      sys_puts();
                      break;
        case SYS_spawn:
                      sys_spawn();
                      break;
        case SYS_yield:
                      sys_yield();
                      break;
	case SYS_fork:
		      sys_fork();
		      break;
        default:
             syscall_set_errno(E_INVAL_CALLNR);
        }
}
