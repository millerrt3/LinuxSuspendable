#include "../types.h"
#include "lkm_export.h"
#include "lkm_utils.h"

#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <linux/cgroup.h>
#include <linux/atomic.h>

extern spinlock_t css_set_lock;

int lkm_export_task_struct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	/*
	 * General process information
	 */
	writeAmt = lkm_file_write( file,"\nstate: ", strlen("\nstate: "), p_offset );
	if( lkm_export_state( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export process state\n" );

	writeAmt = lkm_file_write( file,"\nstack: ", strlen("\nstack: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->stack), sizeof(void*), p_offset );

	// TODO -- Flags are listed @ line 1961; should we interpret them for the user?
	writeAmt = lkm_file_write( file,"\nflags: ", strlen("\nflags: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->flags), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nptrace: ", strlen("\nptrace: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ptrace), sizeof(unsigned int), p_offset );


	/*
	 * SMP waking
	 */
	writeAmt = lkm_file_write( file,"\nwake_entry: ", strlen("\nwake_entry: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->wake_entry), sizeof(struct llist_node), p_offset );

	writeAmt = lkm_file_write( file,"\non_cpu: ", strlen("\non_cpu: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->on_cpu), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nlast_wakee: ", strlen("\nlast_wakee: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->last_wakee), sizeof(struct task_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nwakee_flips: ", strlen("\nwakee_flips: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->wakee_flips), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nwakee_flip_decay_ts: ", strlen("\nwakee_flip_decay_ts: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->wakee_flip_decay_ts), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nwake_cpu: ", strlen("\nwake_cpu: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->wake_cpu), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\non_rq: ", strlen("\non_rq: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->on_rq), sizeof(int), p_offset );


	/*
	 * Scheduling priority
	 */
	writeAmt = lkm_file_write( file,"\nprio: ", strlen("\nprio: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->prio), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nstatic_prio: ", strlen("\nstatic_prio: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->static_prio), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nrt_priority: ", strlen("\nrt_priority: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rt_priority), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nsched_class: ", strlen("\nsched_class: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sched_class), sizeof(struct sched_class*), p_offset );

	writeAmt = lkm_file_write( file,"\nse: ", strlen("\nse: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->se), sizeof(struct sched_entity), p_offset );

	writeAmt = lkm_file_write( file,"\nrt: ", strlen("\nrt: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rt), sizeof(struct sched_rt_entity), p_offset );

	writeAmt = lkm_file_write( file,"\nsched_task_group: ", strlen("\nsched_task_group: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sched_task_group), sizeof(struct task_group*), p_offset );

	writeAmt = lkm_file_write( file,"\ndl: ", strlen("\ndl: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->dl), sizeof(struct sched_dl_entity), p_offset );

	writeAmt = lkm_file_write( file,"\nbtrace_seq: ", strlen("\nbtrace_seq: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->btrace_seq), sizeof(unsigned int), p_offset );


	/*
	 * Assigned CPUs
	 */
	writeAmt = lkm_file_write( file,"\npolicy: ", strlen("\npolicy: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->policy), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nnr_cpus_allowed: ", strlen("\nnr_cpus_allowed: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nr_cpus_allowed), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\ncpus_allowed: ", strlen("\ncpus_allowed: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpus_allowed), sizeof(cpumask_t), p_offset );


	/*
	 * Preemption
	 */
#if 0
	writeAmt = lkm_file_write( file,"\nrcu_read_lock_nesting: ", strlen("\nrcu_read_lock_nesting: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_nesting), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nrcu_read_lock_unlock->blocked: ", strlen("\nrcu_read_lock_unlock->blocked: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_unlock_special.b.blocked), sizeof(bool), p_offset );

	writeAmt = lkm_file_write( file,"\nrcu_read_lock_unlock->need_qs: ", strlen("\nrcu_read_lock_unlock->need_qs: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_unlock_special.b.need_qs), sizeof(bool), p_offset );

	// TODO -- task_ptr->rcu_blocked_node has a significant amount of stuff in it
	writeAmt = lkm_file_write( file,"\nrcu_tasks_nvcsw: ", strlen("\nrcu_tasks_nvcsw: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_tasks_nvcsw), sizeof(unsigned long), p_offset );
#endif

	/*
	 * cpusets
	 */
	if( lkm_export_cpusets( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export cpusets\n" );

  	/*
	 * cgroups
	 */
	if( lkm_export_cgroups( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export cgroups\n" );

  	/*
	 * futex
	 */
	writeAmt = lkm_file_write( file,"\nrobust_list: ", strlen("\nrobust_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->robust_list), sizeof(struct robust_list_head __user *), p_offset );

#if 0
	/*
	 * compat
	 */
	writeAmt = lkm_file_write( file,"\ncompat_robust_list: ", strlen("\ncompat_robust_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->compat_robust_list), sizeof(compat_robust_list_head), p_offset );
#endif

	/*
	 * pi
	 */
	writeAmt = lkm_file_write( file,"\npi_state_list: ", strlen("\npi_state_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pi_state_list), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\npi_state_cache: ", strlen("\npi_state_cache: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pi_state_cache), sizeof(struct futex_pi_state*), p_offset );

	/*
	 * perf events
	 */
	if( lkm_export_perf_events( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export performance events\n" );

    /*
     * debug preempt
     */
    writeAmt = lkm_file_write( file,"\npreempt_disable_ip: ", strlen("\npreempt_disable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->preempt_disable_ip), sizeof(unsigned long), p_offset );

#if 0
	/*
     * numa
     */
    writeAmt = lkm_file_write( file,"\nmempolicy: ", strlen("\nmempolicy: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->mempolicy), sizeof(struct mempolicy), p_offset );

#endif

	// TODO - may be interesting to dig deeper
	writeAmt = lkm_file_write( file,"\nrcu: ", strlen("\nrcu: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu), sizeof(struct rcu_head), p_offset );

	writeAmt = lkm_file_write( file,"\nsplice_pipe: ", strlen("\nsplice_pipe: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->splice_pipe), sizeof(struct pipe_inode_info*), p_offset );

	writeAmt = lkm_file_write( file,"\ntask_frag: ", strlen("\ntask_frag: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->task_frag), sizeof(struct page_frag), p_offset );

	writeAmt = lkm_file_write( file,"\ndelays: ", strlen("\ndelays: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->delays), sizeof(struct task_delay_info*), p_offset );

#if 0
	writeAmt = lkm_file_write( file,"\nmake_it_fail: ", strlen("\nmake_it_fail: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->make_it_fail), sizeof(int), p_offset );
#endif

	/*
	 * dirty records
	 */
	writeAmt = lkm_file_write( file,"\nnr_dirtied: ", strlen("\nnr_dirtied: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nr_dirtied), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nnr_dirtied_pause: ", strlen("\nnr_dirtied_pause: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nr_dirtied_pause), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\ndirty_paused_when: ", strlen("\ndirty_paused_when: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->dirty_paused_when), sizeof(unsigned long), p_offset );

	/*
	 * latencytop
	 */

	/*
	 * timer slack
	 */
	writeAmt = lkm_file_write( file,"\ntimer_slack_ns: ", strlen("\ntimer_slack_ns: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->timer_slack_ns), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\ndefault_timer_slack_ns: ", strlen("\ndefault_timer_slack_ns: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->default_timer_slack_ns), sizeof(unsigned long), p_offset );

	/*
	 * function graphing tracer
	 */
	if( lkm_export_function_graph_tracer( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export function grapher data\n" );

	return 0;
}

int lkm_export_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->state), sizeof(long), p_offset );
	switch(task_ptr->state)
	{
	case -1:
		writeAmt = lkm_file_write( file," (unrunnable)", strlen(" (unrunnable)"), p_offset );
		break;
	case 0:
		writeAmt = lkm_file_write( file," (runnable)", strlen(" (runnable)"), p_offset );
		break;
	default:
		writeAmt = lkm_file_write( file," (stopped)", strlen(" (stopped)"), p_offset );
		break;
	}

	return 0;
}

int lkm_export_cpusets( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;
	unsigned long flags = 0;

	// acquire alloc_lock
	spin_lock_irqsave( &(task_ptr->alloc_lock), flags );

	writeAmt = lkm_file_write( file,"\nmems_allowed: ", strlen("mems_allowed: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->mems_allowed), sizeof(nodemask_t), p_offset );

	writeAmt = lkm_file_write( file,"\nmems_allowed_seq: ", strlen("\nmems_allowed_seq: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->mems_allowed_seq), sizeof(seqcount_t), p_offset );

	writeAmt = lkm_file_write( file,"\ncpuset_mem_spread_rotor: ", strlen("\ncpuset_mem_spread_rotor: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpuset_mem_spread_rotor), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\ncpuset_slab_spread_rotor: ", strlen("\ncpuset_slab_spread_rotor: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpuset_slab_spread_rotor), sizeof(int), p_offset );

	// release alloc_lock
	spin_unlock_irqrestore( &(task_ptr->alloc_lock), flags );

	return 0;
}

int lkm_export_cgroups( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;
	unsigned long flags;

	// acquire lock
	// TODO - css_set_lock was not able to be found as a kernel module
	// spin_lock_irqsave(&css_set_lock, flags);

	writeAmt = lkm_file_write( file,"\ncgroups: ", strlen("\ncgroups: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cgroups), sizeof(struct css_set __rcu *), p_offset );

	spin_lock_irqsave( &(task_ptr->alloc_lock), flags );

	writeAmt = lkm_file_write( file,"\ncg_list: ", strlen("\ncg_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cg_list), sizeof(struct list_head), p_offset );

	spin_unlock_irqrestore( &(task_ptr->alloc_lock), flags );

	// release lock
	// TODO - css_set_lock was not able to be found as a kernel module
	// spin_unlock_irqrestore( &css_set_lock, flags );

	return 0;
}

int lkm_export_perf_events( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	int writeAmt = 0;
	int index = 0;
	char work_buf[100];

	// TODO - does the perf_event_mutex need to be acquired?

	for( index = 0; index < perf_nr_task_contexts; index++ )
	{

		memset( work_buf, 0, 100 );
		sprintf( work_buf, "\nperf_event_ctxp-%d: ", index );

		// print header
		writeAmt = lkm_file_write( file, work_buf, strlen(work_buf), p_offset );
		writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->perf_event_ctxp[index]), sizeof(struct perf_event_context*), p_offset );

	}

	writeAmt = lkm_file_write( file, "\nperf_event_mutex: ", strlen("\nperf_event_mutex: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->perf_event_mutex), sizeof(struct mutex), p_offset );

	writeAmt = lkm_file_write( file, "\nperf_event_list: ", strlen("\nperf_event_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->perf_event_list), sizeof(struct list_head), p_offset );

	return 0;

}

int lkm_export_function_graph_tracer( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	// TODO - Going more in depth for this section may be desirable as it could potentially reveal stacktrace

	int writeAmt = 0;
	unsigned int value = 0;

	writeAmt = lkm_file_write( file,"\ncurr_ret_stack: ", strlen("\ncurr_ret_stack: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->curr_ret_stack), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nret_stack: ", strlen("\nret_stack: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ret_stack), sizeof(struct ftrace_ret_stack*), p_offset );

	writeAmt = lkm_file_write( file,"\nftrace_timestamp: ", strlen("\nftrace_timestamp: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ftrace_timestamp), sizeof(unsigned long long), p_offset );

	value = atomic_read( &(task_ptr->trace_overrun) );
	writeAmt = lkm_file_write( file,"\ntrace_overrun: ", strlen("\ntrace_overrun: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(value), sizeof(unsigned int), p_offset );

	value = atomic_read( &(task_ptr->tracing_graph_pause) );
	writeAmt = lkm_file_write( file,"\ntracing_graph_pause: ", strlen("\ntracing_graph_pause: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(value), sizeof(unsigned int), p_offset );

	return 0;

}