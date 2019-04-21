#include "lib/x86.h"

#include "import.h"

/** TASK 1:
  * * Initialize all the thread queues with tqueue_init_at_id.
  *  Hint 1:
  *  - Remember that there are NUM_IDS + 1 queues. The first NUM_IDS queues are the sleep queues and 
  *    the last queue with id NUM_IDS is the ready queue.
  */
void tqueue_init(unsigned int mbi_addr)
{
  // TODO: define your local variables here.
     unsigned int i;
  // TODO
     tcb_init(mbi_addr);
     for(i=0;i<NUM_IDS+1;i++)
     tqueue_init_at_id(i);
}

/** TASK 2:
  * * Insert the TCB #pid into the TAIL of the thread queue #chid.
  * 
  *  Hint 1: 
  *  - Recall that the doubly linked list is index based.
  *    So you only need to insert the index.
  *  Hint 2: Get the tail of the thread queue using tqueue_get_tail.
  *  Hint 3: There are multiple cases in this function.
  *  - a) If the tail of the current queue is empty, 
  *       you have to set both the head and the tail of the queue. 
  *  - b) Else, you only have to set the tail of the queue.
  *  Hint 4: Remember how the structures TQueue and TCB are setup. 
  *  - TQueue only tracks the head and the tail of the queue. 
  *  - TCB is a doubly linked list.
  *  - So, when you insert #pid into the TAIL of the thread queue, you have to:
  *    1. update the next pointer in the old tail to point to #pid.
  *    2. then the prev pointer for #pid should point to the old tail.
  *    3. and the next pointer for #pid should point to NULL (i.e. NUM_IDS)
  */
void tqueue_enqueue(unsigned int chid, unsigned int pid)
{
  // TODO
     unsigned int cur_tail_val = tqueue_get_tail(chid);
     if(cur_tail_val == NUM_IDS)
     {
        tqueue_set_head(chid,pid);
        tqueue_set_tail(chid,pid);
        tcb_set_prev(pid,NUM_IDS);
        tcb_set_next(pid,NUM_IDS);
      }
     else
     {
        tqueue_set_tail(chid,pid);
        tcb_set_next(cur_tail_val,pid);
        tcb_set_prev(pid,cur_tail_val);
        tcb_set_next(pid,NUM_IDS);      
     }
    
     
}

/** TASK 3:
  * * Reverse action of tqueue_enqueue, i.e., pops a TCB from the head of specified queue.
  *   It returns the poped thread's id, or NUM_IDS if the queue is empty.
  *
  *  Hint 1: Get the head of the thread queue using tqueue_get_head
  *  Hint 2: You will have to set the new head of the queue to the next pointer of current head.
  *  As in TASK2, you will also have to update the prev and next pointers of the new head. 
  *  There are multiple cases:
  *  - a) If the queue is now empty (i.e. next pointer of current head is NUM_IDS),
  *       empty the queue (i.e. set both head and tail to NUM_IDS)
  *  - b) Else, set prev pointer of the new head to NUM_IDS.
  *  Hint 3: Detach the popped head from other threads.
  *  - Reset the poped head's prev and next pointers to NUM_IDS.
  */
unsigned int tqueue_dequeue(unsigned int chid)
{
  // TODO
  unsigned int cur_head_val = tqueue_get_head(chid);
  unsigned int new_head_val = tcb_get_next(cur_head_val);
  if(cur_head_val == NUM_IDS)
  {
     tqueue_set_head(chid,NUM_IDS);
     tqueue_set_tail(chid,NUM_IDS);
     tcb_set_prev(cur_head_val,NUM_IDS);
     tcb_set_next(cur_head_val,NUM_IDS);
  }
  else
  {
     tqueue_set_head(chid,new_head_val);
     tcb_set_prev(new_head_val,NUM_IDS);
     tcb_set_prev(cur_head_val,NUM_IDS);
     tcb_set_next(cur_head_val,NUM_IDS);
  }
  return cur_head_val;  
  return 0;
}

/** TASK 4:
  * * Remove the TCB #pid from the queue #chid.
  *   You need to update the neighboring elements of #pid so that they reference each other (skip #pid). 
  *
  *  Hint: there are many cases in this function.
  *  - a) If #pid is the only thread in the queue, you will have to reset the queue as done in TASK 3.
  *  - b) If #pid is the head of the queue, set it's next thread as the head of the queue.
  *       Remember to update the new head's TCB pointers as in TASK 3.
  *  - c) If #pid is the tail of the queue, set it's prev thread as the tail of the queue.
  *       Remember to update the new tail's TCB pointers. (Where does the tail's next pointer point to?)
  *  Hint 2: Detach the removed #pid's TCB from other threads. (Same as TASK 3)
  */
void tqueue_remove(unsigned int chid, unsigned int pid)
{
  // TODO
     if(tcb_get_prev(pid) == NUM_IDS && tcb_get_next(pid) == NUM_IDS)
     {
       tqueue_set_head(chid,NUM_IDS);
       tqueue_set_tail(chid,NUM_IDS);
     }
     else if(tqueue_get_head(chid) == pid)
     {
       unsigned int nxt_thr = tcb_get_next(pid);
       tqueue_set_head(chid,nxt_thr);
       tcb_set_prev(nxt_thr,NUM_IDS);
       tcb_set_next(pid,NUM_IDS);
       tcb_set_prev(pid,NUM_IDS);
     }
     else if(tqueue_get_tail(chid) == pid)
     {
       unsigned int prev_thr = tcb_get_prev(pid);
       tqueue_set_tail(chid,prev_thr);
       tcb_set_next(prev_thr,NUM_IDS);
       tcb_set_prev(pid,NUM_IDS);
       tcb_set_next(pid,NUM_IDS);
     }
     else
     {
       unsigned int mid_prev = tcb_get_prev(pid);
       unsigned int mid_next = tcb_get_next(pid);
       tcb_set_prev(pid,NUM_IDS);
       tcb_set_next(pid,NUM_IDS);
       tcb_set_next(mid_prev,mid_next);
       tcb_set_prev(mid_next,mid_prev);
     }

       
}
