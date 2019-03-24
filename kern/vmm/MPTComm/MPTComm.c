#include <lib/x86.h>

#include "import.h"

/** TASK 1:
  * * For each process from id 0 to NUM_IDS -1,
  *   set the page directory entries so that the kernel portion of the map as identity map,
  *   and the rest of page directories are unmmaped.
  * 
  * Hint 1: Recall which portions are reserved for the kernel and calculate the pde_index.       
  * Hint 2: Recall which function in MPTIntro layer is used to set identity map. (See import.h) 
  * Hint 3: Remove the page directory entry to unmap it.
  */
void pdir_init(unsigned int mbi_adr)
{
    // TODO: define your local variables here.
    unsigned int proc_idx,pdir_idx;//addr
    unsigned int VM_USERLO = 0x40000000/PAGESIZE;
    unsigned int VM_USERHI = 0xF0000000/PAGESIZE;
    idptbl_init(mbi_adr);

    // TODO
    for(proc_idx=0;proc_idx<NUM_IDS;proc_idx++)
    {
      for(pdir_idx=0;pdir_idx<1024;pdir_idx++)
       {
        //addr=(i*1024+j);
        	if(pdir_idx<VM_USERLO>>10)                              //making the VM index compatible with array indexes of IDPTbl.
        	set_pdir_entry_identity(proc_idx,pdir_idx);

        	else if (pdir_idx>=VM_USERHI>>10)
        	set_pdir_entry_identity(proc_idx,pdir_idx);

        	else
        	rmv_pdir_entry(proc_idx,pdir_idx);
       }
    }
}

/** TASK 2:
  * * 1. Allocate a page (with container_alloc) for the page table,
  * * 2. Check if the page was allocated and register the allocated page in page directory for the given virtual address.
  * * 3. Clear (set to 0) all the page table entries for this newly mapped page table.
  * * 4. Return the page index of the newly allocated physical page.
  * 	 In the case when there's no physical page available, it returns 0.
  */
unsigned int alloc_ptbl(unsigned int proc_index, unsigned int vadr)
{
  // TODO
     unsigned int i;
     unsigned int DIR_MASK = 0xffc00000;
     unsigned int pdir_idx = (vadr & DIR_MASK) >> 22;
     unsigned int pg_idx = container_alloc(proc_index);
     if(pg_idx!=0)
     {
     	set_pdir_entry_by_va(proc_index,vadr,pg_idx);
     	for(i=0;i<1024;i++)
     	{
         	rmv_ptbl_entry(proc_index,pdir_idx,i);                       //removing the page table entries.
     	}
     
        return pg_idx;
     }
     else 
     return 0;
}

/** TASK 3:
  * * Reverse operation of alloc_ptbl.
  *   - Remove corresponding page directory entry
  *   - Free the page for the page table entries (with container_free).
  * Hint 1: Find the pde corresponding to vadr (MPTOp layer)
  * Hint 2: Remove the pde (MPTOp layer)
  * Hint 3: Use container free
  */
void free_ptbl(unsigned int proc_index, unsigned int vadr)
{
  // TODO
     unsigned int DIR_MASK = 0xffc00000;
     unsigned int dir_idx = (vadr & DIR_MASK) >> 12;            //finding pde to corresponding vadr
     rmv_pdir_entry_by_va(proc_index,vadr);
     container_free(proc_index,dir_idx);
}
