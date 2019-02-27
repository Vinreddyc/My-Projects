#include <lib/debug.h>
#include "import.h"

#define PAGESIZE	4096
#define VM_USERLO	0x40000000
#define VM_USERHI	0xF0000000
#define VM_USERLO_PI	(VM_USERLO / PAGESIZE)   // VM_USERLO page index
#define VM_USERHI_PI	(VM_USERHI / PAGESIZE)   // VM_USERHI page index

/** ASSIGNMENT INFO:
  * * Implement the initialization function for the allocation table AT.
  * - It contains two major parts:
  *   1. Calculate the actual physical memory of the machine, and set the number of physical pages (NUM_PAGES).
  *   2. Initialize the physical allocation table (AT) implemented in MATIntro layer, based on the
  *      information available in the physical memory map table.  
  *
  *  - Review import.h in the current directory for the list of available getter and setter functions.
  */
void
pmem_init(unsigned int mbi_addr)
{
  unsigned int nps=0;

  // TODO: Define your local variables here.
  unsigned int i=0,lo_idx=0,hi_idx=0,k,j;
  unsigned int rowcnt;
  unsigned int phypgcnt;
  unsigned int pg_st_addr,pg_end_addr;
  unsigned int range_end,range_start;
  //Calls the lower layer initializatin primitives.
  //The parameter mbi_addr shell not be used in the further code.
	devinit(mbi_addr);

  /** TASK 1:
    * Calculate the number of actual number of available physical pages and store it into the local varaible nps.
    * 
    * Hint 1: Review the import.h file for available functions.
    * Hint 2: Think of it as the highest address possible in the ranges of the memory map table,
    *         divided by the page size plus 1.
    */
    rowcnt=get_size();
    //dprintf("getsize is:****************%d\n",get_size()); 
    while(i<rowcnt)
    {
      if(nps<get_mms(i)+get_mml(i))
      //dprintf("getmms and getmml are*************:%lu          %lu\n",get_mms(i),get_mml(i));
      nps=get_mms(i)+get_mml(i);     // maximum value
      //dprintf("nps is***************:%lu\n",nps);
      i++;                                                             //extracting the highest address of the last range.
    }
    //dprintf("highest address is is *******************%lu\n",(nps));
    phypgcnt=(nps/PAGESIZE)+1;//physical page count
    nps=phypgcnt;
    //dprintf("final1 nps is *******************%lu\n",(nps));      

  // TODO

	set_nps(nps); // Setting the value computed above to NUM_PAGES.
    
  /** TASK 2:
    * Initialization of the physical allocation table (AT).
    *
    * In CertiKOS, the entire addresses < VM_USERLO or >= VM_USERHI are reserved by the kernel.
    * That corresponds to the physical pages from 0 to VM_USERLO_PI-1, and from VM_USERHI_PI to NUM_PAGES-1.
    * The rest of pages that correspond to addresses [VM_USERLO, VM_USERHI), can be used freely ONLY IF
    * the entire page falls into one of the ranges in the memory map table with the permission marked as usable.
    *
    * Hints:
    * 1. You have to initialize AT for all the page indices from 0 to NPS - 1.
    * 2. For the pages that are reserved by the kernel, simply set its permission to 1.
    *    Recall that the setter at_set_perm also marks the page as unallocated. 
    *    Thus, you don't have to call another function to set the allocation flag.
    * 3. For the rest of the pages, explore the memory map table to find the information about the corresponding physical page
    *    and set its permission accordingly.
    *    The permission should be set to 2 only if there is a range containing the entire page that
    *    is marked as available in the memory map table. Otherwise, it should be set to 0.
    *    Note that the ranges in the memory map are not aligned by pages.
    *    So it may be possible that for some pages, only part of the addresses are in the ranges.
    *    Currently, we do not utilize partial pages, so in that case, you should consider those pages as unavailble.
    * 4. Every page in the allocation table should be initialized.
    *    But the ranges in the momory map table may not cover the entire available address space.
    *    That means there may be some gaps between the ranges.
    *    You should still set the permission of those pages in allocation table to 0.
    */
  // TODO
     for(i=0;i<VM_USERLO_PI;i++)
     {   
         at_set_perm(i,1);                                                    //setting permissions for the kernel portions
     }
     for(i=VM_USERHI_PI;i<nps;i++)
     {
         at_set_perm(i,1);
     }
     
     //for(i=VM_USERLO_PI;i<VM_USERHI_PI;i++)
     //nps_start=(0x40000000/PAGESIZE);             VM_USERLO
     //nps_end=(0x80000000/PAGESIZE)-1;             Hex value of usable range
     //dprintf("nps_start*************:%lu\n",nps_start);
     //dprintf("nps_start*************:%lu\n",nps_end);
     //for(i=nps_start+1;i<nps_end;i++)
    for(k=0;k<get_size();k++)
    {
         //dprintf("is_usable %d value********* %d\n",k,is_usable(k));
         if(VM_USERLO >= get_mms(k) && VM_USERLO <= get_mms(k)+get_mml(k))
         	lo_idx=k;
         if(VM_USERHI >= get_mms(k) && VM_USERHI <= get_mms(k)+get_mml(k))                   //fetching the range indexes for which VM_USERLO and VM_USERHI belong.
                hi_idx=k;
    }
     for(i=VM_USERLO_PI;i<VM_USERHI_PI;i++)
     {
       
	   pg_st_addr=i*4096;
           pg_end_addr=i*4096+4096;
           j=lo_idx;
           do
           {
            range_start=get_mms(j);
            range_end = get_mms(j)+get_mml(j);
            if(pg_st_addr >= range_start  &&  pg_end_addr <= range_end)                  //checking whether the page fits between the start and end ranges.
            {
               if(is_usable(j)==1)
               at_set_perm(i,2);                                                         //setting permission to normal usage.
               break;
            }
            else
            at_set_perm(i,0);                                                            //setting the bios permission for the gaps.
            j++;
           }
           while(j<=hi_idx);
         
       
     }
}


