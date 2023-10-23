#ifndef LKM_helper_h
#define LKM_helper_h

#include <linux/module.h>
// #include <linux/sched.h>
#include <linux/pid.h>
// #include <linux/printk.h>
#include <linux/kernel.h>
// #include <linux/slab.h>
#include <linux/pgtable.h>
// #include <linux/fs.h>
// #include <asm/io.h>
// #include <linux/fdtable.h>
#include<linux/moduleparam.h>
#include "v2p.h"
#include "mem_profile.h"

typedef enum opcode_member{
    op_v2p         = 1,
    op_search_huge = 2,
    op_show_vma    = 3,
    op_non         = 0,
} opcode_member;

#endif