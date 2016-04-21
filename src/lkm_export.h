/**
 * @file lkm_export.h
 * @brief Contains utility functions for exporting a processes contents
 *
 * @details
 *		This file defines the function that are used for exporting all
 *		the contents of a process within the linux kernel.
 *
 * @version 1.0
 * @date 04/18/2016
 */
#ifndef _LKM_EXPORT_H
#define _LKM_EXPORT_H

// *************************************************************************
//                               INCLUDES
// *************************************************************************
#include "types.h"
#include "lkm_utils.h"

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/slab.h>

#ifdef CONFIG_CPUSETS
int lkm_export_cpusets( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

int lkm_export_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_task_struct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_sigpending( struct sigpending signal, LKM_FILE file, unsigned long long *p_offset );
int lkm_export_sigset_t( sigset_t set, LKM_FILE file, unsigned long long *p_offset );

#ifdef CONFIG_CGROUPS
int lkm_export_cgroups( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_MEMCG
int lkm_export_cgroup_memory( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_PERF_EVENTS
int lkm_export_perf_events( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
int lkm_export_function_graph_tracer( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_TRACING
int lkm_export_tracing( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_TASK_XACCT
int lkm_export_task_xacct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

int lkm_export_vm_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );

#ifdef CONFIG_TRACE_IRQFLAGS
int lkm_export_trace_irqflags( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_LOCKDEP
int lkm_export_lockdep( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_DEBUG_MUTEXES
int lkm_export_debug_mutexes( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#ifdef CONFIG_RT_MUTEXES
int lkm_export_rt_mutexes( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset );
#endif

#endif
