#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

#include "../types.h"

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