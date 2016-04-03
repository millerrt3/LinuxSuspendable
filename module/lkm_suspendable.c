#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>

#define FILENAME_MAX_SIZE 200

// ----- Function Prototypes
static ssize_t pid_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t pid_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static ssize_t restore_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t restore_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobject *pid_kobj;
static pid_t pid_to_suspend = 0;
static char restore_filename[ FILENAME_MAX_SIZE ];

static struct kobj_attribute suspend_attribute =
	__ATTR(pid_to_suspend, 0664, pid_show, pid_store);
	
static struct kobj_attribute restore_attribute =
	__ATTR(restore_filename, 0664, restore_show, restore_store );
	
static struct attribute *attrs[] = {
	&suspend_attribute.attr,
	&restore_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};



static ssize_t pid_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t pid_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int sv = 0;
	struct pid *real_pid = 0;
	struct task_struct* pidTask = 0;

	// convert string to virtual pid
	sv = kstrtoint(buf, 10, &pid_to_suspend);
	if (sv < 0)
		return sv;
		
	printk(KERN_DEBUG "linux_suspendable->PID=%d\n", pid_to_suspend );

#if 0
	// Convert from virtual to real PID
	real_pid = find_vpid(pid_to_suspend);
	if (real_pid == NULL)
	{
		printk(KERN_WARNING "VPID translation failed\n");
		return count;
	}

	// Get the task struct associated with the provided PID
	pidTask = pid_task(real_pid, PIDTYPE_PID);
	if (real_pid == NULL)
	{
		printk(KERN_WARNING "PID task locating failed\n");
		return count;
	}
#endif
	
	return count;

}

static ssize_t restore_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t restore_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk( KERN_DEBUG "linux_suspendable->filename: %s\n", buf );
	
	// clears buffer
	memset( restore_filename, 0, FILENAME_MAX_SIZE );
	
	// copies the filename
	strncpy( restore_filename, buf, FILENAME_MAX_SIZE );
	
	return count;
}

// module installation point
static int lkm_init(void)
{
	int retval = 0;
	
	printk( KERN_DEBUG "linux_suspendable->init\n" );
	
	// creates directory for interacting with module
	pid_kobj = kobject_create_and_add("linux_suspendable", kernel_kobj);
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
	printk(KERN_DEBUG "linux_suspendable->removed\n");
	kobject_put(pid_kobj);
}

// Sets up callback functions
module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple & Robert Miller");
MODULE_DESCRIPTION("Linux module that provides the ability to suspend an actively executing process to a file.");
