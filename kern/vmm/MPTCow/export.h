#ifndef _INIT_MM_MPTCOW_H_
#define _INIT_MM_MPTCOW_H_

void paging_init(unsigned int);
void map_cow( uint32_t , uint32_t);
void map_decow( uint32_t, uint32_t);


#endif

