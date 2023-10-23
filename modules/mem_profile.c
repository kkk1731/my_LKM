#include "mem_profile.h"
#include "v2p.h"
#define BUF_SZ 1024
#define REGPAGE_SZ 0x1000
#define HPAGE_SZ 0x200000

//@*paddr:return value of vaddr->paddr translation
int is_huge_get_page(struct mm_struct *mm_str, unsigned long addr, unsigned long *paddr, struct page **page){
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd = NULL;
        pte_t *pte = NULL;
        int ret = 0;

        unsigned long pfn, phys_page_addr;

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
                *page = pmd_page(*pmd);
                pfn = pmd_pfn(*pmd);
                phys_page_addr = ((phys_addr_t)(pfn) << 21);
		*paddr = phys_page_addr;
                //printk("it is hugepage");
                return 1;
        }
        
        pte = pte_offset_map(pmd, addr);
	if (!pte || !pte_present(*pte) || pte_none(*pte) || is_zero_pfn(pte_pfn(*pte)))
	{
            //printk("is huge err");
            return 0;
        }

	*paddr = pte_pfn(*pte);
        *page = pte_page(*pte);
        //printk("page=%p", page);

        pte_unmap(pte);
        //printk("it is regular page");
        return 0;
}

/** @brief write file and fsync when "hsl_debug_alloc==1"
 * 
*/
int file_write_log(struct file *file, char *buf, char *msg, loff_t *pos){
	int ret;
	memset(buf, '\0', BUF_SZ);
        strcpy(buf, msg);
        ret = kernel_write(file, buf, strlen(buf), pos);
        //printk("msg=%c, write ret=%d, strlen(buf)=%ld", msg[0], ret, strlen(buf));
        return ret;
}

int show_vma(struct vm_area_struct *vma, struct file *file, loff_t *pos){
        unsigned long vm_flags = vma->vm_flags;
        char *buf;
        unsigned long i;
	bool mapping_existed = true;
        buf = kmalloc(BUF_SZ, GFP_KERNEL);
        if(buf){
                memset(buf, '\0', BUF_SZ);
        }else{
                printk("buf alloc err");
                return -1;
        }

        //first sentence
        sprintf(buf, "vma=%p\npermission=", vma);
        //printk("strlen(buf)=%ld, pos=%lld", strlen(buf), *pos);
        kernel_write(file, buf, strlen(buf), pos);
        //printk("*pos=%lld", *pos);

        //permissions
        memset(buf, '\0', BUF_SZ);
        if (vm_flags & VM_READ)  file_write_log(file, buf, "r", pos);
        //printk("*pos=%lld", *pos);
        if (vm_flags & VM_WRITE) file_write_log(file, buf, "w", pos);
        if(vm_flags&VM_EXEC)     file_write_log(file, buf, "x", pos);
        if(vm_flags&VM_SHARED)   file_write_log(file, buf, "s", pos);
        memset(buf, '\0', BUF_SZ);
        file_write_log(file, buf, "\n", pos);
        //printk("after kaigyo *pos=%lld", *pos);
        //huge permission
        memset(buf, '\0', BUF_SZ);
        if(vm_flags&VM_HUGEPAGE) file_write_log(file, buf, "huge\n", pos);
        if(vm_flags&VM_NOHUGEPAGE) file_write_log(file, buf, "nohuge\n", pos);
        //printk("after huge or not *pos=%lld", *pos);
        //per page profile
        for (i = vma->vm_start; i < vma->vm_end; i += REGPAGE_SZ){
                unsigned long paddr = 0;
                struct page *page;
                memset(buf, '\0', BUF_SZ);
                //huge
                //printk("is_huge");
                //printk("%p, %ld, %ld, %p", vma->vm_mm, i, paddr, page);
                if (is_huge_get_page(vma->vm_mm, i, &paddr, &page))
                {
                        //printk("%lx, %lx, %p", i, paddr, page);
                        //printk("huge");
                        if(paddr==0) sprintf(buf, "huge page, vaddr=%lx, paddr=%lx\n", i, paddr);
                        else sprintf(buf, "huge page, vaddr=%lx, paddr=%lx, page_ref_count=%d\n", i, paddr, page_ref_count(page));
                        i += (HPAGE_SZ - REGPAGE_SZ);
                }
                //regular
                else{
                        //printk("%lx, %lx, %p, %p", i, paddr, page, buf);
                        //printk("regular");
                        if(paddr==0) {
				if(mapping_existed){
					sprintf(buf, "regular page, vaddr=%lx, paddr=%lx\n", i, paddr);
					mapping_existed = false;
				}
				//if mapping not existed, go next loop
				else
					continue;
			}
                        else {
				sprintf(buf, "regular page, vaddr=%lx, paddr=%lx, page_ref_count=%d\n", i, paddr, page_ref_count(page));
				mapping_existed = true;
			}
                }
                //printk("kernel write");
                kernel_write(file, buf, strlen(buf), pos);
                //printk("end");
        }
        kfree(buf);
        return 0;
}

int show_vmas(struct task_struct *tsk)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma_head, *vma;
        //file
        loff_t pos = 0;
        struct file *file;
        char *filename = "/home/kk1731/LKM/test/show_vma_log.txt";
        
        mm = tsk->mm;
        vma_head = mm->mmap;
        vma = vma_head;

        //file init
        file = filp_open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
        if (IS_ERR(file))
        {
                printk("pre_file open err=%ld", PTR_ERR(file));
                return 0;
        }
        else
        {
                //init success
        }

        do{
            show_vma(vma, file, &pos);
            vma = vma->vm_next;
            //printk("show_vma called, vma=%p, vma_head=%p", vma, vma_head);
            if (!vma) break;
        } while (vma != vma_head);

        filp_close(file, NULL);

        return 0;
}
