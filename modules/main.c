//main source of dump meminfo module.

#include "helper.h"

MODULE_LICENSE("GPL");

static int opcode = 0;
module_param(opcode, int, S_IRUGO);
static int pid = 0;
module_param(pid, int, S_IRUGO);
static unsigned long vaddr_head = 0x0;
module_param(vaddr_head, ulong, S_IRUGO);
static unsigned long vaddr_end = 0x0;
module_param(vaddr_end, ulong, S_IRUGO);


static struct task_struct *search_task_struct_by_pid(int pid)
{
	struct task_struct *tsk	= NULL;
	struct pid *pid_s	= NULL;

	pid_s = find_get_pid(pid);
	if (!pid_s)
		return NULL;

	tsk = get_pid_task(pid_s, PIDTYPE_PID);
	if (!tsk)
		return NULL;

	return tsk;
}

static int __init dump_meminfo_init(void){
    struct task_struct *tsk;
    int ret = 0;
    printk("dump_meminfo module is loaded, pid=%d", pid);

    tsk = search_task_struct_by_pid(pid);

    if(opcode == op_v2p){
        //later implement in v2p.c
        ret = v2p_range(tsk, vaddr_head, vaddr_end);

    }
    else if(opcode == op_search_huge){
        ret = search_huge(tsk, vaddr_head, vaddr_end);
    }
    else if (opcode == op_show_vma)
    {
        ret = show_vmas(tsk);
    }

    return 0;
}

static void __exit dump_meminfo_exit(void){
    printk("dump_meminfo module is unloaded");
}

module_init(dump_meminfo_init);
module_exit(dump_meminfo_exit);