#include <linux/fs.h> // vfs_write
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/pid.h>
#include <linux/sched.h>  // tasklist_lock
#include <linux/memory.h> // virt_to_phys

#include "types.h"
#include "lkm_utils.h"

// Derived from 'getconf PAGESIZE' on raspberry pi
// #define PAGE_SIZE (4096)

struct task_struct* lkm_get_task_struct( int pid )
{

    struct pid *real_pid_ptr = 0;
    struct task_struct* pid_task_ptr = 0;

    printk( KERN_DEBUG "lkm_get_task_struct->pid=%d\n", pid );

    // Convert from virtual to real PID
    real_pid_ptr = find_vpid(pid);
    if (real_pid_ptr == NULL)
    {
        printk(KERN_WARNING "lkm_get_task_struct->VPID translation failed; pid=%d\n", pid );
        return NULL;
    }

    // Get the task struct associated with the provided PID
    // pid_task - RCU used internally for synchronization with kernel
    pid_task_ptr = pid_task(real_pid_ptr, PIDTYPE_PID);
    if (pid_task_ptr == NULL)
    {
        printk(KERN_WARNING "lkm_get_task_struct->PID task locating failed; pid=%d\n", pid );
        return NULL;
    }
    
    // return
    return pid_task_ptr;
    
}

unsigned long lkm_virtual_to_physical( struct mm_struct *mm, unsigned long virtual_address )
{
    
    pgd_t *pgd = 0;
    pud_t *pud = 0;
    pmd_t *pmd = 0;
    pte_t *pte = 0;
    struct page *page = 0;
    unsigned long phys = 0;
    
    if( mm == 0 )
    {
        printk( KERN_WARNING "lkm_virtual_to_physical->invalid memory map pointer\n" );
        return INVALID_ARG;
    }
    
    pgd = pgd_offset(mm, virtual_address);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return 0;
    pud = pud_offset(pgd, virtual_address);
    if (pud_none(*pud) || pud_bad(*pud))
        return 0;
    pmd = pmd_offset(pud, virtual_address);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        return 0;
    if (!(pte = pte_offset_map(pmd, virtual_address)))
        return 0;
    if (!(page = pte_page(*pte)))
        return 0;
    phys = page_to_phys(page);
    pte_unmap(pte);
    
    return phys; 
}

int lkm_for_each_vma_in_task( struct task_struct* task_ptr, vmaCallback handler )
{
    struct vm_area_struct *vma = 0;
    int sv = 0;
    int rv = 0;
    
    // process arguments
    if( task_ptr == 0 || handler == 0 )
    {
        printk( KERN_WARNING "lkm_for_each_vma_page_in_task->Invalid Argument; task=0x%08x, handler=0x%08x\n", (unsigned int)task_ptr, (unsigned int)handler );
        return INVALID_ARG;
    }
    
    if (task_ptr->mm && task_ptr->mm->mmap)
    {
        
        // for each vma region in the process
        for (vma = task_ptr->mm->mmap; vma; vma = vma->vm_next)
        {
            
            // call the provided handler for each of the vma pages and regions
            sv = (*handler)( task_ptr, vma->vm_start, vma->vm_end, PAGE_SIZE );
            if( sv < 0 )
            {
                printk( KERN_WARNING "lkm_for_each_vma_in_task->handler returned error; error_code=0x%08x\n", sv );
                rv = sv;
            }
            
        }
    }
    
    return rv;
}

int lkm_for_each_vma_page_in_task( struct task_struct* task_ptr, vmaCallback handler )
{
    
    struct vm_area_struct *vma = 0;
    unsigned long vpage = 0;
    unsigned long phys = 0;
    int sv = 0;
    int rv = 0;
    
    // process arguments
    if( task_ptr == 0 || handler == 0 )
    {
        printk( KERN_WARNING "lkm_for_each_vma_page_in_task->Invalid Argument; task=0x%08x, handler=0x%08x\n", (unsigned int)task_ptr, (unsigned int)handler );
        return INVALID_ARG;
    }
    
    if (task_ptr->mm && task_ptr->mm->mmap)
    {
        
        // for each vma region in the process
        for (vma = task_ptr->mm->mmap; vma; vma = vma->vm_next)
        {
            
            // for each page in the memory region
            for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE)
            {
                phys = lkm_virtual_to_physical(task_ptr->mm, vpage);
                if( phys < 0 )
                {
                    printk( KERN_WARNING "lkm_for_each_vma_page_in_task->Physical Address Not Found\n" );
                    continue;
                }
                
                // call the provided handler for each of the vma pages and regions
                sv = (*handler)( task_ptr, vpage, phys, PAGE_SIZE );
                if( sv < 0 )
                {
                    printk( KERN_WARNING "lkm_for_each_vma_page_in_task->handler returned error; error_code=0x%08x\n", sv );
                    rv = sv;
                }
            }
        }
    }
    
    return rv;
    
}

int lkm_save_to_file_ascii( const char *pathname, void *buffer, int size )
{

    LKM_FILE file = 0;
    int writeAmt = 0;
    unsigned long long offset = 0;

    printk( KERN_DEBUG "lkm_save_to_file_ascii->path=%s, addr=0x%08x, size=%d\n", pathname, (int)buffer, size );

    // open file for write
    file = lkm_file_open( pathname, LKM_Write );
    if( file == NULL )
    {
        printk( KERN_WARNING "lkm_save_to_file_ascii->failed to open %s\n", pathname );
        file = NULL;
    }

    // write content into the file
    if( file != NULL )
    {
        writeAmt = lkm_file_ascii_write( file, buffer, size, &offset );
        if( writeAmt < size )
        {
            printk( KERN_WARNING "lkm_save_to_file_ascii->failed to write; actual=%d, expected=%d\n", writeAmt, size );
        }
    }

    if( file != NULL )
        lkm_file_close( file );

    return writeAmt;

}

int lkm_save_to_file( const char *pathname, void *buffer, int size )
{

    LKM_FILE file = 0;
    int writeAmt = 0;
    unsigned long long offset = 0;

    printk( KERN_DEBUG "lkm_save_to_file->path=%s, addr=0x%08x, size=%d\n", pathname, (int)buffer, size );

    // open file for write
    file = lkm_file_open( pathname, LKM_Write );
    if( file == NULL )
    {
        printk( KERN_WARNING "lkm_save_to_file->failed to open %s\n", pathname );
        file = NULL;
    }

    // write content into the file
    if( file != NULL )
    {
        writeAmt = lkm_file_write( file, buffer, size, &offset );
        if( writeAmt < size )
        {
            printk( KERN_WARNING "lkm_save_to_file->failed to write; actual=%d, expected=%d\n", writeAmt, size );
        }
    }

    if( file != NULL )
        lkm_file_close( file );

    return writeAmt;

}

int      lkm_create_directory( const char *pathname )
{
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    filp = filp_open(pathname, O_DIRECTORY|O_CREAT, 0777);
    if( filp == NULL )

    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return -1;
    }

    // close file
    filp_close(filp, NULL);
    
    return 0;
}

LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission )
{

	struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    if( permission == LKM_Write )
    {
    	filp = filp_open(pathname, O_WRONLY|O_CREAT|O_DIRECTORY, 0777);
    }
    else if( permission == LKM_Read )
    {
    	filp = filp_open(pathname, O_RDONLY, 0777);
    }

    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}


int lkm_file_write( LKM_FILE file, char *buffer, int size, unsigned long long *p_offset )
{
	mm_segment_t oldfs;
    int ret = 0;

    if( buffer == 0 || p_offset == 0 || file == 0 )
    {
    	printk( KERN_WARNING "lkm_file_write->invalid argument; buf=0x%08x, p_offset=0x%08x, file=0x%08x\n", (unsigned int)buffer, (unsigned int)p_offset, (unsigned int)file );
    	return INVALID_ARG;
    }

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, buffer, size, p_offset);

    set_fs(oldfs);
    return ret;

}

char lkm_itoa( char nibble )
{

    if( nibble >= 0 && nibble <= 9 )
    {
        return '0' + nibble;
    }
    else if( nibble >= 10 && nibble <= 15 )
    {
        return 'a' + (nibble-10);
    }
    else
    {
        return -1;
    }

}

// ensure destination buffer is twice the size of size
void lkm_binary_to_ascii( char *dest, char *ptr, int size )
{
    int index = 0;
    int dest_index = 0;

    for( index = 0; index < size; index++ )
    {

        // grab first nibble
        dest[dest_index++] = lkm_itoa((ptr[index] & 0xf0) >> 4);

        // grab second nibble
        dest[dest_index++] = lkm_itoa(ptr[index] & 0x0f);
    }

}

int      lkm_file_ascii_write( LKM_FILE file, char *buffer, int size, unsigned long long *p_offset )
{
    mm_segment_t oldfs;
    int ret = 0;
    char work_buf[100];
    int bytes_remain = size;
    int bytes_written = 0;
    int bytes_to_convert = 0;

    if( buffer == 0 || p_offset == 0 || file == 0 )
    {
    	printk( KERN_WARNING "lkm_file_ascii_write->invalid argument; buf=0x%08x, p_offset=0x%08x, file=0x%08x\n", (unsigned int)buffer, (unsigned int)p_offset, (unsigned int)file );
    	return INVALID_ARG;
    }

    oldfs = get_fs();
    set_fs(get_ds());

    while( bytes_remain > 0 )
    {

        memset( work_buf, 0, 100 );

        if( bytes_remain >= 50 )
        {
            bytes_to_convert = 50;
            
        }
        else
        {
            bytes_to_convert = bytes_remain;
        }


        // convert the next round into work_buf
        lkm_binary_to_ascii( work_buf, buffer + bytes_written, bytes_to_convert );

        // add content to file
        ret = vfs_write(file, work_buf, bytes_to_convert * 2, p_offset);
        if( ret < bytes_to_convert * 2)
        {
            printk( KERN_WARNING "lkm_file_ascii_write->ERROR Failed to write ascii content\n" );
            return -1;
        }

        bytes_written += bytes_to_convert;
        bytes_remain -= bytes_to_convert;
    }

    set_fs(oldfs);
    return size;

}


int      lkm_file_read( LKM_FILE file, char *buffer, int size )
{

	mm_segment_t oldfs;
    int ret = 0;
    unsigned long long offset = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, buffer, size, &offset);

    set_fs(oldfs);
    return ret;

}


void     lkm_file_close( LKM_FILE file )
{
	filp_close(file, NULL);
}
