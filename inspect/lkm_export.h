
#ifndef _LKM_EXPORT_H
#define _LKM_EXPORT_H

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


int lkm_export_cpusets( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_task_struct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );

#endif
