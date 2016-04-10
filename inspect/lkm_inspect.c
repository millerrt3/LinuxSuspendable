#include "../types.h"
#include "lkm_utils.h"

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>

#define MAX_FILENAME_SIZE 200

// ----- Global Variables
static int lkm_pid = 0;
static int lkm_cmd = 0;
static int lkm_filename_set = 0;
static char lkm_filename[MAX_FILENAME_SIZE];

static ssize_t pid_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return 0;
}

static ssize_t pid_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

  int sv = 0;
  int local = 0;

  // convert string to virtual pid
  sv = kstrtoint(buf, 10, &local);
  if (sv < 0)
    return sv;
		
  // update local pid copy
  lkm_pid = local;
  printk(KERN_DEBUG "linux_inspect->PID=%d\n", lkm_pid );

  return count;
}

static ssize_t cmd_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return 0;
}

static ssize_t cmd_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
  int sv = 0;
  int local = 0;
  struct task_struct *task_struct_ptr = 0;

  // convert string to virtual pid
  sv = kstrtoint(buf, 10, &local);
  if (sv < 0)
    return sv;
    
  // update local pid copy
  lkm_cmd = local;
  printk(KERN_DEBUG "linux_inspect->cmd=%d\n", lkm_cmd );

  // TODO - Add processing based on the received command
  switch( lkm_cmd )
  {

    case INS_TEST:
      printk( KERN_DEBUG "linux_inspect->INS_TEST\n" );
      printk( KERN_DEBUG "linux_inspect->task_struct_size=%d\n", sizeof(struct task_struct));

      // retrieve pointer for user process
      task_struct_ptr = lkm_get_task_struct( lkm_pid );
      if( task_struct_ptr == NULL )
      {
        printk( KERN_WARNING "linux_inspect->invalid pid; %d\n", lkm_pid );
        break;
      }

      prink( KERN_DEBUG "linux_inspect(%d)->state=%d\n", lkm_pid, task_struct_ptr->state );
      // prink( KERN_DEBUG "linux_inspect(%d)->state=%d\n", lkm_pid, task_struct_ptr->state );

      break;
    case INS_SUSPEND:
      printk( KERN_DEBUG "linux_inspect->INS_SUSPEND\n" );
      break;
    case INS_DUMP:
      printk( KERN_DEBUG "linux_inspect->INS_DUMP\n" );
      break;
    default:
      printk( KERN_DEBUG "linux_inspect->default\n" );

  }

  return count;
}

static ssize_t filename_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  return 0;
}

static ssize_t filename_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{

  // determine number of bytes to copy
  int copy_amt = MAX_FILENAME_SIZE;
  if( count < MAX_FILENAME_SIZE )
  {
    copy_amt = count;
  }

  // copy filename that was passed
  // TODO - Look into the use of copy_from_user - necessary?
  memset( lkm_filename, 0, MAX_FILENAME_SIZE );
  memcpy( lkm_filename, buf, copy_amt );
  lkm_filename_set = 1;

  printk( KERN_DEBUG "linux_inspect->filename=%s\n", lkm_filename );

  return count;

}

static struct kobject *pid_kobj;

static struct kobj_attribute pid_attribute =
  __ATTR(lkm_pid, 0664, pid_show, pid_store);
  
static struct kobj_attribute cmd_attribute =
  __ATTR(lkm_cmd, 0664, cmd_show, cmd_store );

static struct kobj_attribute filename_attribute =
  __ATTR(lkm_filename, 0664, filename_show, filename_store );
  
static struct attribute *attrs[] = {
  &pid_attribute.attr,
  &cmd_attribute.attr,
  &filename_attribute.attr,
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

  // clear filename
  memset( lkm_filename, 0, MAX_FILENAME_SIZE );
	
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
}

// Sets up callback functions
module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple & Robert Miller");
MODULE_DESCRIPTION("Linux module to allow for inspecting the task_struct for user space process.");
