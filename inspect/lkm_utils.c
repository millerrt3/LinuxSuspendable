#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/pid.h>

#if 0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#endif

#include "../types.h"
#include "lkm_utils.h"

struct task_struct* lkm_get_task_struct( int pid )
{

    struct pid *real_pid_ptr = 0;
    struct task_struct* pid_task_ptr = 0;

    printk( KERN_DEBUG "lkm_get_task_struct->pid=%d\n", pid );

    // acquire read lock for the task_struct area
    rcu_read_lock();

    // Convert from virtual to real PID
    real_pid_ptr = find_vpid(pid);
    if (real_pid_ptr == NULL)
    {
        printk(KERN_WARNING "lkm_get_task_struct->VPID translation failed; pid=%d\n", pid );
        return NULL;
    }

    // release lock over the task_struct listing
    rcu_read_unlock();

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


void lkm_print_buffer( void *buffer, int size )
{
    int index = 0;
    char *pBuf = (char*)buffer;

    for( index = 0; index < size; index++ )
    {
        printk( KERN_DEBUG "%02x ", pBuf[index] );
        if( index != 0 && (index % 20)==0 )
        {
            printk( KERN_DEBUG "\n" );
        }
    }
}


int lkm_save_to_file( const char *pathname, void *buffer, int size )
{

    LKM_FILE file = 0;
    int writeAmt = 0;

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
        writeAmt = lkm_file_write( file, buffer, size );
        if( writeAmt < size )
        {
            printk( KERN_WARNING "lkm_save_to_file->failed to write; actual=%d, expected=%d\n", writeAmt, size );
        }
    }

    if( file != NULL )
        lkm_file_close( file );

    return writeAmt;

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
    	filp = filp_open(pathname, O_WRONLY|O_CREAT, 0777);
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


int      lkm_file_write( LKM_FILE file, char *buffer, int size )
{
	mm_segment_t oldfs;
    int ret = 0;
    unsigned long long offset = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, buffer, size, &offset);

    set_fs(oldfs);
    return ret;

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


Status lkm_activate_pid( struct task_struct *task_ptr )
{

    printk( KERN_DEBUG "lkm_activate_pid\n" );

    return ERROR;
}


Status lkm_deactivate_pid( struct task_struct *task_ptr )
{

    Status status = OK;

    printk( KERN_DEBUG "lkm_deactivate_pid\n" );

    // detach_pid is not exported within kernel
    // detach_pid( task_ptr, PIDTYPE_PID );

    // TODO - acquire write lock for tasklist_lock
    // TODO - disable CPU interrupts
    // TODO - mark process as uninterruptible and unschedulable - removes from run queue?

    // TODO - remove process from run queue
    // del_from_runqueue();

    // remove from pid hash
    status = lkm_remove_from_pidhash( task_ptr );

    // remove from task list
    status = lkm_remove_from_list( task_ptr );

    // TODO - release write lock

    return ERROR;

}

Status lkm_remove_from_pidhash( struct task_struct *task_ptr )
{

    // Adapted from __change_pid(...) in linux kernel - http://lxr.free-electrons.com/source/kernel/pid.c#L395
    struct pid_link *link_ptr = &(task_ptr->pids[PIDTYPE_PID]);
    struct pid *pid_ptr = link_ptr->pid;
    int tmp = 0;

    hlist_del_rcu( &(link_ptr->node) );
    link->pid = NULL;

    for (tmp = PIDTYPE_MAX; --tmp >= 0; )
    {
        if (!hlist_empty(&pid_ptr->tasks[tmp]))
            return;
    }

    return OK;
}

/**
 * @brief Removes the provided process from the circular linked list
 *
 * @warning
 *  WRITE lock should be acquired for tasklist_lock prior to calling
 *  this function.
 */
Status lkm_remove_from_list( struct task_struct *task_ptr )
{

    if( task_ptr == NULL )
    {
        return INVALID_ARG;
    }

    // acquire pointers to surrounding items in list
    struct list_head *next = task_ptr->tasks.next;
    struct list_head *prev = task_ptr->tasks.prev;

    next->prev = prev;
    prev->next = next;

    return OK;

}
