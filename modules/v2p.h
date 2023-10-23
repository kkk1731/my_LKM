#ifndef LKM_v2p_h
#define LKM_v2p_h
unsigned long v2p(struct task_struct *tsk, unsigned long addr);
int v2p_range(struct task_struct *tsk, unsigned long vaddr_top, unsigned long vaddr_end);
int search_huge(struct task_struct *tsk, unsigned long vaddr_top, unsigned long vaddr_end);
int is_hugepage(struct task_struct *tsk, unsigned long addr, unsigned long *paddr);
#endif