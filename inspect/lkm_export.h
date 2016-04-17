
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
int lkm_export_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_task_struct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_cgroups( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_cgroup_memory( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_perf_events( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_function_graph_tracer( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_tracing( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_task_xacct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_vm_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );

#if 0
#ifdef CONFIG_TRACE_IRQFLAGS
int lkm_export_trace_irqflags( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif
#endif

#ifdef CONFIG_LOCKDEP
int lkm_export_lockdep( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#endif
