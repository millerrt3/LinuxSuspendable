#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#include "../types.h"
#include "lkm_utils.h"

int lkm_open( const char *pathname, int flags )
{

	int fd = 0;
	int rv = 0;
	mm_segment_t old_fs = 0;

	printk( KERN_DEBUG "lkm_open->path=%s, flags=0x%08x\n", pathname, flags );

	if( flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR )
	{
		printk( KERN_WARNING "lkm_open->Invalid Argument; flags=0x%08x\n", flags );
		return -1;
	}

	// retrieve current file descriptor
	old_fs = get_fs();
	set_fs(KERNEL_DS);

	// opens the file
	fd = sys_open( pathname, flags, 0 );
	if( fd < 0 )
	{
		printk( KERN_WARNING "lkm_open->Open Failed; sv=0x%08x\n", fd );
	}

	// reinstall the old file system
	set_fs(old_fs);

	// return fd
	return fd;

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
    int ret;
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
    int ret;
    unsigned long long offset = 0;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;

}

void     lkm_file_close( LKM_FILE file )
{
	filp_close(file, NULL);
}

int lkm_file_sync( LKM_FILE file )
{
	vfs_fsync( file, 0 );
	return 0;
}