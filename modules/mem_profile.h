#ifndef LKM_mem_profile_h
#define LKM_mem_profile_h

#include <linux/mm.h>
#include <linux/slab.h>
//file


int show_vmas(struct task_struct *tsk);

#endif