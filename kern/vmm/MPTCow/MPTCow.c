#include <lib/gcc.h>
#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

#define PAGESIZE	4096
#define VM_USERLO	0x40000000
#define VM_USERHI	0xF0000000
#define VM_USERLO_PI	(VM_USERLO / PAGESIZE)   // VM_USERLO page index
#define VM_USERHI_PI	(VM_USERHI / PAGESIZE)   // VM_USERHI page index
#define DIR_MASK    0xffc00000
#define PAGE_MASK   0x003ff000
#define OFFSET_MASK 0x00000fff
#define DIR_SHIFT   22
#define PAGE_SHIFT  12
#define PT_PERM_CUP     (PTE_COW | PTE_U | PTE_P)
#define PT_PERM_PWU     (PTE_P | PTE_W | PTE_U)


void map_cow( uint32_t from_pid, uint32_t to_pid)
{
	//uint32_t pgtable_ent,pg_ind,i,j,vir_addr,from_pg_int,to_pg_int;
        uint32_t from_pdir_ent,to_pdir_ent,from_pdir_ent_idx,from_pgtable_ent,to_pgtable_ent,from_ptbl_id,to_ptbl_id;
	uint32_t i,j,vir_addr;
	for(i = 0; i < 1024; i++)
	{
 		if(i >= (VM_USERLO>>DIR_SHIFT) && i < (VM_USERHI>>DIR_SHIFT))
		{
		from_pdir_ent = get_pdir_entry(from_pid, i);
		//to_pdir_ent = get_pdir_entry(to_pid, i);
                //from_pdir_ent_idx = from_pdir_ent/PAGESIZE;
		if(from_pdir_ent!=0)
		{
			to_ptbl_id = container_alloc(to_pid);
			set_pdir_entry(to_pid,i,to_ptbl_id);
		for(j=0; j < 1024; j++)
		{
			vir_addr = ((i * 1024) + j) * PAGESIZE;
			if(vir_addr >= VM_USERLO && vir_addr < VM_USERHI)
			{
					
				from_pgtable_ent = get_ptbl_entry_by_va(from_pid, vir_addr);
				if(from_pgtable_ent!=0)
				{
				from_ptbl_id = from_pgtable_ent/PAGESIZE;
				set_ptbl_entry_by_va(to_pid,vir_addr,from_ptbl_id,PT_PERM_CUP);
				set_ptbl_entry_by_va(from_pid,vir_addr,from_ptbl_id,PT_PERM_CUP);
				}
				
			}
				
		}
		}
		}
	}
	
}



void map_decow( uint32_t pid, uint32_t vadr)
{
	uint32_t i,j;	
	//Old Page	
	uint32_t pgtable_ent = get_ptbl_entry_by_va(pid, vadr);
	uint32_t pgtable_ent_idx = pgtable_ent/PAGESIZE;
	uint32_t old_stphyaddr = pgtable_ent_idx*PAGESIZE;
        uint32_t old_ndphyaddr = (pgtable_ent_idx+1)*PAGESIZE;
	rmv_ptbl_entry_by_va(pid, vadr);

	//New Page	
	uint32_t allc_idx = alloc_page(pid, vadr, PT_PERM_PWU);
        uint32_t nphyaddr_st = allc_idx*PAGESIZE;
        uint32_t nphyaddr_nd = (allc_idx+1)*PAGESIZE;
	
	i = nphyaddr_st;
	j = old_stphyaddr;
	while(j < nphyaddr_nd)
	{
		if(old_stphyaddr < old_ndphyaddr)
		{
		uint32_t *map_src = (uint32_t *)i;
                uint32_t *map_des = (uint32_t *)j;
		*map_src = *map_des;
		}
		i++;
		j++;
	}
	
}
