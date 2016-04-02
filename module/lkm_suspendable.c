#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// module installation point
static int lkm_init(void)
{
	printk(KERN_ALERT "Module installed\n");
	return 0;
}

// module removal point
static void lkm_exit(void)
{
	printk(KERN_ALERT "Module removed\n");
}

// Sets up callback functions
module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cameron Whipple & Robert Miller");
MODULE_DESCRIPTION("Linux module that provides the ability to suspend an actively executing process to a file.");
