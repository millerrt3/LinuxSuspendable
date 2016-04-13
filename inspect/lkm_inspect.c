#include "../types.h"
#include "lkm_utils.h"

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/slab.h>

#define MAX_FILENAME_SIZE 40
#define MAX_DIR_SIZE 40
#define MAX_PATH_SIZE 100

#define AVAILABLE 0
#define BUSY 1

static int lkm_state = AVAILABLE;
static LKM_Operation_t lkm_opstruct;

static void __generate_path( char *dest, char *dir, char *filename )
{

  int index = 0;

  memset( dest, 0, MAX_PATH_SIZE );
  strncpy( dest, dir, strlen(dir) );
  dest[ strlen(dest) + 1 ] = '/';

  index = strlen( dest );
  strncpy( &(dest[index]), filename, MAX_PATH_SIZE - index );

  printk( KERN_DEBUG "__generate_path; dir=%s, filename=%s, dest=%s\n", dir, filename, dest );

}

static ssize_t operation_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return 0;
}

static ssize_t operation_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

  // TODO - Do we need to copy the buffer into a queue and have a separate kernel thread process the elements?

  int sv = 0;
  int rv = 0;
  int local = 0;
  struct task_struct *task_ptr = 0;
  int file_size = 0;
  Status status = 0;
  char dir_name[MAX_DIR_SIZE];
  char file_name[MAX_FILENAME_SIZE];
  char full_path[MAX_PATH_SIZE];
  // LKM_FILE file = 0;

  // copies the operation struct from userspace
  memcpy( &lkm_opstruct, buf, sizeof(LKM_Operation_t));
  memset( dir_name, 0, MAX_DIR_SIZE );
  memset( file_name, 0, MAX_FILENAME_SIZE );
  memset( full_path, 0, MAX_PATH_SIZE );

  printk(KERN_DEBUG "-------------------------------------------------\n");
  printk(KERN_DEBUG "linux_inspect->cmd=0x%08x\n", lkm_opstruct.cmd );
  printk(KERN_DEBUG "linux_inspect->pid=%d\n", lkm_opstruct.proc_id );

  if( lkm_state == BUSY )
    return 0;

  // acquire read lock for the task_struct area
  rcu_read_lock();

  // retrieve pointer to task_struct
  task_ptr = lkm_get_task_struct( lkm_opstruct.proc_id );
  if( task_ptr == NULL )
  {
    printk( KERN_WARNING "linux_inspect->invalid pid; %d\n", lkm_opstruct.proc_id );
    return;
  }

  // TODO - retrieve time element

  // generate directory name
  // TODO - integrate time element
  snprintf( dir_name, "%d-time-inspection", lkm_opstruct.proc_id );
  
  // create directory
  if( lkm_create_directory( dir_name ) < 0 )
  {
    printk( KERN_WARNING "linux_inspect->failed to create %s directory\n", dir_name );
    rv = 1;
  }

  // start dumping corresponding sections of the process
  if( (rv == 0) && (lkm_opstruct.cmd & LKM_TASK_STRUCT) )
  {

    // creates the full path
    __generate_path( full_path, dir_name, file_name );

    // export task struct contents to file
    file_size = lkm_save_to_file( full_path, task_ptr, sizeof(struct task_struct) );
    if( file_size < 0 )
    {
      printk( KERN_WARNING "linux_inspect->failed to export task_struct contents to %s\n", full_path );
      rv = 1;
    }

  }
#if 0
  if( (rv == 0) && (lkm_opstruct.cmd & LKM_TASK_MEMORY) )
  {

  }
#endif

  // release lock over the task_struct listing
  rcu_read_unlock();

  if( rv != 0 )
    return 0;

  return count;
}

static struct kobject *pid_kobj;

static struct kobj_attribute operation_attribute =
  __ATTR(operation, 0664, operation_show, operation_store );
  
static struct attribute *attrs[] = {
  &operation_attribute.attr,
  NULL, /* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
  .attrs = attrs,
};

// module installation point
static int lkm_init(void)
{
  int retval = 0;
	
  printk( KERN_DEBUG "linux_inspect->init\n" );

  // clear operation
  memset( &lkm_opstruct, 0, sizeof( LKM_Operation_t ) );
	
  // creates directory for interacting with module
  pid_kobj = kobject_create_and_add("linux_inspect", kernel_kobj);
  if (!pid_kobj)
    return -ENOMEM;

  /* Create the files associated with this kobject */
  retval = sysfs_create_group(pid_kobj, &attr_group);
  if (retval)
    kobject_put(pid_kobj);
	
  return 0;
}

// module removal point
static void lkm_exit(void)
{
  printk(KERN_DEBUG "linux_inspect->removed\n");
  kobject_put(pid_kobj);

  // kfree(lkm_task_struct_contents);
}

// Sets up callback functions
module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple & Robert Miller");
MODULE_DESCRIPTION("Linux module to allow for inspecting the task_struct for user space process.");
