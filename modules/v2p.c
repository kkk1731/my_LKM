// #include <linux/kernel.h>
// #include <linux/pgtable.h>
// #include <linux/pid.h>
#include <linux/pgtable.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/fdtable.h>

#define OBJ_MASK_PGD	0x01
#define OBJ_MASK_VMA	0x02
#define OBJ_MASK_MEMREG	0x04
#define OBJ_MASK_TLS	0x08
#define OBJ_MASK_FILE	0x10
#include "v2p.h"


#define PAGE_OFFSET_MASK (~(~0UL << PAGE_SHIFT))

unsigned long v2p(struct task_struct *tsk, unsigned long addr)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long pfn, phys_page_addr;
	struct mm_struct *mm_str;
	mm_str = tsk->mm;

	pgd = pgd_offset(mm_str, addr);
	if (pgd_none(*pgd))
		return 0;

	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d))
		return 0;

	pud = pud_offset(p4d, addr);
	if (pud_none(*pud))
		return 0;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd))
		return 0;
	if(pmd_trans_huge(*pmd) > 0){
		pfn = pmd_pfn(*pmd);
		phys_page_addr = ((phys_addr_t)(pfn) << 21);
		return phys_page_addr;
	}

	pte = pte_offset_map(pmd, addr);
	if (!pte || !pte_present(*pte) || pte_none(*pte) || is_zero_pfn(pte_pfn(*pte)))
	{
		return 0;
	}

	pfn = pte_pfn(*pte);
	phys_page_addr = PFN_PHYS(pfn);

	pte_unmap(pte);

	return phys_page_addr;
}



int v2p_range(struct task_struct *tsk, unsigned long vaddr_head, unsigned long vaddr_end){
        unsigned long i;
        for (i = vaddr_head; i <= vaddr_end; i += 0x1000) {
                unsigned long paddr = v2p(tsk, i);
                printk("vaddr:0x%lx, paddr:0x%lx", i, paddr);
        }
        return 0;
}

//@*paddr:return value of vaddr->paddr translation
int is_hugepage(struct task_struct *tsk, unsigned long addr, unsigned long *paddr){
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd = NULL;
	int ret = 0;

	unsigned long pfn, phys_page_addr;
	struct mm_struct *mm_str;
	mm_str = tsk->mm;

	pgd = pgd_offset(mm_str, addr);
	if (pgd_none(*pgd))
		return 0;

	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d))
		return 0;

	pud = pud_offset(p4d, addr);
	if (pud_none(*pud))
		return 0;

	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd))
		return 0;
	ret = pmd_trans_huge(*pmd);
	if(ret > 0){
		pfn = pmd_pfn(*pmd);
		phys_page_addr = ((phys_addr_t)(pfn) << 21);
		*paddr = phys_page_addr;
		return 1;
	}
	
	return 0;
}

int search_huge(struct task_struct *tsk, unsigned long vaddr_head, unsigned long vaddr_end){
    unsigned long i;
    for (i = vaddr_head; i <= vaddr_end; i += 0x200000)
    {
	    unsigned long paddr = 0;
	    if (is_hugepage(tsk, i, &paddr))
	    {
		    printk("hugepage, vaddr:0x%lx, paddr:0x%lx", i, paddr);
	    }
    }
	return 0;

}
/*
unsigned long v2p(struct task_struct *tsk, unsigned long addr)
{
    struct mm_struct *mm;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long ret;

    mm = current->mm;
    // pgtable walk
    pgd = pgd_offset(mm, addr);
    if (!pgd_present(*pgd)) return -1;
    p4d = p4d_offset(pgd, addr);
    if (!p4d_present(*p4d)) return -2;
    pud = pud_offset(p4d, addr);
    if (!pud_present(*pud)) return -3;
    pmd = pmd_offset(pud, addr);
    if (!pmd_present(*pmd)) return -4;

    if (pmd_trans_huge(*pmd)) return pmd_val(*pmd);
    if (!pmd_bad(*pmd)) {
        pte = pte_offset_map(pmd, addr);
        if (!pte_none(*pte))
            ret = pte_val(*pte);
        else
            ret = -5;
        pte_unmap(pte);
	}
	return ret;
}
*/