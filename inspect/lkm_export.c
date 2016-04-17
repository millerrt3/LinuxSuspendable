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
#ifdef CONFIG_SMP
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
#endif

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

#ifdef CONFIG_CGROUP_SCHED
	writeAmt = lkm_file_write( file,"\nsched_task_group: ", strlen("\nsched_task_group: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sched_task_group), sizeof(struct task_group*), p_offset );
#endif

	writeAmt = lkm_file_write( file,"\ndl: ", strlen("\ndl: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->dl), sizeof(struct sched_dl_entity), p_offset );

#ifdef CONFIG_PREEMPT_NOTIFIERS
	/* TODO */
#endif

#ifdef CONFIG_BLK_DEV_IO_TRACE
	writeAmt = lkm_file_write( file,"\nbtrace_seq: ", strlen("\nbtrace_seq: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->btrace_seq), sizeof(unsigned int), p_offset );
#endif

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
#ifdef CONFIG_PREEMPT_RCU
	writeAmt = lkm_file_write( file,"\nrcu_read_lock_nesting: ", strlen("\nrcu_read_lock_nesting: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_nesting), sizeof(int), p_offset );
#if 0
	writeAmt = lkm_file_write( file,"\nrcu_read_lock_unlock->blocked: ", strlen("\nrcu_read_lock_unlock->blocked: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_unlock_special.b.blocked), sizeof(bool), p_offset );

	writeAmt = lkm_file_write( file,"\nrcu_read_lock_unlock->need_qs: ", strlen("\nrcu_read_lock_unlock->need_qs: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_read_lock_unlock_special.b.need_qs), sizeof(bool), p_offset );
#endif

#endif

#ifdef CONFIG_TASKS_RCU
	// TODO -- task_ptr->rcu_blocked_node has a significant amount of stuff in it
	writeAmt = lkm_file_write( file,"\nrcu_tasks_nvcsw: ", strlen("\nrcu_tasks_nvcsw: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu_tasks_nvcsw), sizeof(unsigned long), p_offset );
#endif



   /*************************************************************************************************/
   /*                             ADD CONTENT HERE UNTIL TASK_STRUCT COMPLETED                      */
   /*************************************************************************************************/



	/*
	 * journaling filesystem info
	 */
	writeAmt = lkm_file_write( file,"\njournal_info: ", strlen("\njournal_info: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->journal_info), sizeof(void*), p_offset );

	/*
	 * stacked block device information
	 */
	// TODO - May be good to go further into as the bio structure is a major component in representing a driver in linux kernel (help with the story of projects importance)
	writeAmt = lkm_file_write( file,"\nbio_list: ", strlen("\nbio_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->bio_list), sizeof(struct bio_list*), p_offset );

	/*
	 * Enable the block layer within the linux kernel - CONFG_BLOCK
	 */
#ifdef CONFIG_BLOCK
	writeAmt = lkm_file_write( file,"\nplug: ", strlen("\nplug: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->plug), sizeof(struct blk_plug*), p_offset );
#endif

	/*
	 * vm state
	 */
	 // TODO - Definitely a look into for extension as most of the attributes are pointers to strucures
	if( lkm_export_vm_state( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export vm state\n" );

	/*
	 * task extended accounting over taskstats
	 */
#ifdef CONFIG_TASK_XACCT
	if( lkm_export_task_xacct( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export task xacct\n" );
#endif

	/*
	 * cpusets
	 */
#ifdef CONFIG_CPUSETS
	if( lkm_export_cpusets( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export cpusets\n" );
#endif

  	/*
	 * cgroups
	 */
#ifdef CONFIG_CGROUPS
	if( lkm_export_cgroups( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export cgroups\n" );
#endif

  	/*
	 * futex
	 */
#ifdef CONFIG_FUTEX

	writeAmt = lkm_file_write( file,"\nrobust_list: ", strlen("\nrobust_list: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->robust_list), sizeof(struct robust_list_head __user *), p_offset );

	/*
	 * compat
	 */
#ifdef CONFIG_COMPAT
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

#endif

	/*
	 * perf events
	 */
#ifdef CONFIG_PERF_EVENTS
	if( lkm_export_perf_events( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export performance events\n" );
#endif

    /*
     * debug preempt
     */
#ifdef CONFIG_DEBUG_PREEMPT
    writeAmt = lkm_file_write( file,"\npreempt_disable_ip: ", strlen("\npreempt_disable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->preempt_disable_ip), sizeof(unsigned long), p_offset );
#endif

	/*
     * numa
     */
#ifdef CONFIG_NUMA
	/* TODO - room for extension with kernel that has numa enabled */
    writeAmt = lkm_file_write( file,"\nmempolicy: ", strlen("\nmempolicy: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->mempolicy), sizeof(struct mempolicy*), p_offset );
#endif

#ifdef CONFIG_NUMA_BALANCING
	/* room for extension */
#endif

	// TODO - may be interesting to dig deeper
	writeAmt = lkm_file_write( file,"\nrcu: ", strlen("\nrcu: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rcu), sizeof(struct rcu_head), p_offset );

	writeAmt = lkm_file_write( file,"\nsplice_pipe: ", strlen("\nsplice_pipe: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->splice_pipe), sizeof(struct pipe_inode_info*), p_offset );

	writeAmt = lkm_file_write( file,"\ntask_frag: ", strlen("\ntask_frag: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->task_frag), sizeof(struct page_frag), p_offset );

#ifdef CONFIG_TASK_DELAY_ACCT
	writeAmt = lkm_file_write( file,"\ndelays: ", strlen("\ndelays: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->delays), sizeof(struct task_delay_info*), p_offset );
#endif

#ifdef CONFIG_FAULT_INJECTION
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
#ifdef CONFIG_LATENCYTOP
	/* room for extension */
#endif

	/*
	 * timer slack
	 */
	writeAmt = lkm_file_write( file,"\ntimer_slack_ns: ", strlen("\ntimer_slack_ns: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->timer_slack_ns), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\ndefault_timer_slack_ns: ", strlen("\ndefault_timer_slack_ns: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->default_timer_slack_ns), sizeof(unsigned long), p_offset );

	/*
  	 * Kernel Address Sanitizer (kasan)
  	 */
#ifdef CONFIG_KASAN
	writeAmt = lkm_file_write( file,"\nkasan_depth: ", strlen("\nkasan_depth: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->kasan_depth), sizeof(unsigned int), p_offset );
#endif

	/*
	 * function graphing tracer
	 */
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	if( lkm_export_function_graph_tracer( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export function grapher data\n" );
#endif

    /*
	 * function tracer
	 */
#ifdef CONFIG_TRACING
	if( lkm_export_tracing( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export the tracing data\n" );
#endif

    /*
     * memory allocation for cgroups
     */
#ifdef CONFIG_MEMCG
  	if( lkm_export_cgroup_memory( task_ptr, file, p_offset ) < 0 )
  		printk( KERN_WARNING "linux_inspect->Failed to export cgroup memory\n" );
#endif

  	/*
  	 * uprobes
  	 */
#ifdef CONFIG_UPROBES
  	 /* room for extension */
#endif

  	/*
  	 * bcache
  	 * NOTE: Not built into the kernel
  	 */
#if defined(CONFIG_BCACHE) || defined(CONFIG_BCACHE_MODULE)
	/* room for extension */
#endif

  	/*
  	 * debug atomic sleep
  	 * NOTE: Not built into the kernel
  	 */
#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
	/* room for extension */
#endif

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

#ifdef CONFIG_CPUSETS
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
#endif

#ifdef CONFIG_CGROUPS
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
#endif

#ifdef CONFIG_PERF_EVENTS
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
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
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
#endif

#ifdef CONFIG_TRACING
int lkm_export_tracing( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\ntrace: ", strlen("\ntrace: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->trace), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\ntrace_recursion: ", strlen("\ntrace_recursion: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->trace_recursion), sizeof(unsigned long), p_offset );

	return 0;
}
#endif

#ifdef CONFIG_MEMCG
int lkm_export_cgroup_memory( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nmemcg_oom->memcg: ", strlen("\nmemcg_oom->memcg: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->memcg_oom.memcg), sizeof(struct mem_cgroup*), p_offset );

	writeAmt = lkm_file_write( file,"\nmemcg_oom->gfp_mask: ", strlen("\nmemcg_oom->gfp_mask: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->memcg_oom.gfp_mask), sizeof(gfp_t), p_offset );

	writeAmt = lkm_file_write( file,"\nmemcg_oom->order: ", strlen("\nmemcg_oom->order: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->memcg_oom.order), sizeof(int), p_offset );

	// TODO - one last remaining field

	return 0;
}
#endif

#ifdef CONFIG_TASK_XACCT
int lkm_export_task_xacct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nacct_rss_mem1: ", strlen("\nacct_rss_mem1: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->acct_rss_mem1), sizeof(u64), p_offset );

	writeAmt = lkm_file_write( file,"\nacct_vm_mem1: ", strlen("\nacct_vm_mem1: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->acct_vm_mem1), sizeof(u64), p_offset );

	writeAmt = lkm_file_write( file,"\nacct_timexpd: ", strlen("\nacct_timexpd: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->acct_timexpd), sizeof(cputime_t), p_offset );

	return 0;
}
#endif

int lkm_export_vm_state( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nreclaim_state: ", strlen("\nreclaim_state: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->reclaim_state), sizeof(struct reclaim_state*), p_offset );

	writeAmt = lkm_file_write( file,"\nbacking_dev_info: ", strlen("\nbacking_dev_info: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->backing_dev_info), sizeof(struct backing_dev_info*), p_offset );

	writeAmt = lkm_file_write( file,"\nio_context: ", strlen("\nio_context: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->io_context), sizeof(struct io_context*), p_offset );

	writeAmt = lkm_file_write( file,"\nptrace_message: ", strlen("\nptrace_message: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ptrace_message), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nlast_siginfo: ", strlen("\nlast_siginfo: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->last_siginfo), sizeof(siginfo_t*), p_offset );

	writeAmt = lkm_file_write( file,"\nioac: ", strlen("\nioac: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ioac), sizeof(struct task_io_accounting), p_offset );

	return 0;
}

#ifdef CONFIG_LOCKDEP
int lkm_export_lockdep( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{

	int writeAmt = 0;
	int index = 0;
	char work_buf[100];

	writeAmt = lkm_file_write( file,"\ncurr_chain_key: ", strlen("\ncurr_chain_key: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->curr_chain_key), sizeof(u64), p_offset );

	writeAmt = lkm_file_write( file,"\nlockdep_depth: ", strlen("\nlockdep_depth: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->lockdep_depth), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nlockdep_recursion: ", strlen("\nlockdep_recursion: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->lockdep_recursion), sizeof(unsigned int), p_offset );

	for( index = 0; index < MAX_LOCK_DEPTH; index++ )
	{

		memset( work_buf, 0, 100 );
		sprintf( work_buf, "\nheld_locks-%d: ", index );

		// print header
		writeAmt = lkm_file_write( file, work_buf, strlen(work_buf), p_offset );
		writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->held_locks[index]), sizeof(struct held_lock), p_offset );

	}

	writeAmt = lkm_file_write( file,"\nlockdep_reclaim_gfp: ", strlen("\nlockdep_reclaim_gfp: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->lockdep_reclaim_gfp), sizeof(gfp_t), p_offset );

	return 0;
}
#endif