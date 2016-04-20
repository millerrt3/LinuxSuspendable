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

static void lkm_export_print_flags( unsigned int flags, LKM_FILE file, unsigned long long *p_offset  )
{
	int writeAmt = 0;

	writeAmt = lkm_file_ascii_write( file, (char*)&(flags), sizeof(unsigned int), p_offset );

	// check to see if any flag at all was set, if so then adjust pointer in file
	if( flags & 0xfffffff8 )
		writeAmt = lkm_file_write( file,"\n\t", strlen("\n\t"), p_offset );

	if( flags & PF_EXITING )
		writeAmt = lkm_file_write( file,"PF_EXITING | ", strlen("PF_EXITING | "), p_offset );

	if( flags & PF_EXITPIDONE )
		writeAmt = lkm_file_write( file,"PF_EXITPIDONE | ", strlen("PF_EXITPIDONE | "), p_offset );

	if( flags & PF_VCPU )
		writeAmt = lkm_file_write( file,"PF_VCPU | ", strlen("PF_VCPU | "), p_offset );

	if( flags & PF_WQ_WORKER )
		writeAmt = lkm_file_write( file,"PF_WQ_WORKER | ", strlen("PF_WQ_WORKER | "), p_offset );

	if( flags & PF_FORKNOEXEC )
		writeAmt = lkm_file_write( file,"PF_FORKNOEXEC | ", strlen("PF_FORKNOEXEC | "), p_offset );

	if( flags & PF_MCE_PROCESS )
		writeAmt = lkm_file_write( file,"PF_MCE_PROCESS | ", strlen("PF_MCE_PROCESS | "), p_offset );

	if( flags & PF_SUPERPRIV )
		writeAmt = lkm_file_write( file,"PF_SUPERPRIV | ", strlen("PF_SUPERPRIV | "), p_offset );

	if( flags & PF_DUMPCORE )
		writeAmt = lkm_file_write( file,"PF_DUMPCORE | ", strlen("PF_DUMPCORE | "), p_offset );

	if( flags & PF_SIGNALED )
		writeAmt = lkm_file_write( file,"PF_SIGNALED | ", strlen("PF_SIGNALED | "), p_offset );

	if( flags & PF_MEMALLOC )
		writeAmt = lkm_file_write( file,"PF_MEMALLOC | ", strlen("PF_MEMALLOC | "), p_offset );

	if( flags & PF_NPROC_EXCEEDED )
		writeAmt = lkm_file_write( file,"PF_NPROC_EXCEEDED | ", strlen("PF_NPROC_EXCEEDED | "), p_offset );

	if( flags & PF_USED_MATH )
		writeAmt = lkm_file_write( file,"PF_USED_MATH | ", strlen("PF_USED_MATH | "), p_offset );

	if( flags & PF_USED_ASYNC )
		writeAmt = lkm_file_write( file,"PF_USED_ASYNC | ", strlen("PF_USED_ASYNC | "), p_offset );

	if( flags & PF_NOFREEZE )
		writeAmt = lkm_file_write( file,"PF_NOFREEZE | ", strlen("PF_NOFREEZE | "), p_offset );

	if( flags & PF_FROZEN )
		writeAmt = lkm_file_write( file,"PF_FROZEN | ", strlen("PF_FROZEN | "), p_offset );

	if( flags & PF_FSTRANS )
		writeAmt = lkm_file_write( file,"PF_FSTRANS | ", strlen("PF_FSTRANS | "), p_offset );

	if( flags & PF_KSWAPD )
		writeAmt = lkm_file_write( file,"PF_KSWAPD | ", strlen("PF_KSWAPD | "), p_offset );

	if( flags & PF_MEMALLOC_NOIO )
		writeAmt = lkm_file_write( file,"PF_MEMALLOC_NOIO | ", strlen("PF_MEMALLOC_NOIO | "), p_offset );

	if( flags & PF_LESS_THROTTLE )
		writeAmt = lkm_file_write( file,"PF_LESS_THROTTLE | ", strlen("PF_LESS_THROTTLE | "), p_offset );

	if( flags & PF_KTHREAD )
		writeAmt = lkm_file_write( file,"PF_KTHREAD | ", strlen("PF_KTHREAD | "), p_offset );

	if( flags & PF_RANDOMIZE )
		writeAmt = lkm_file_write( file,"PF_RANDOMIZE | ", strlen("PF_RANDOMIZE | "), p_offset );

	if( flags & PF_SWAPWRITE )
		writeAmt = lkm_file_write( file,"PF_SNAPWRITE | ", strlen("PF_SNAPWRITE | "), p_offset );

	if( flags & PF_NO_SETAFFINITY )
		writeAmt = lkm_file_write( file,"PF_NO_SETAFFINITY | ", strlen("PF_NO_SETAFFINITY | "), p_offset );

	if( flags & PF_MCE_EARLY )
		writeAmt = lkm_file_write( file,"PF_MCE_EARLY | ", strlen("PF_MCE_EARLY | "), p_offset );

	if( flags & PF_MUTEX_TESTER )
		writeAmt = lkm_file_write( file,"PF_MUTEX_TESTER | ", strlen("PF_MUTEX_TESTER | "), p_offset );

	if( flags & PF_FREEZER_SKIP )
		writeAmt = lkm_file_write( file,"PF_FREEZER_SKIP | ", strlen("PF_FREEZER_SKIP | "), p_offset );

	if( flags & PF_SUSPEND_TASK )
		writeAmt = lkm_file_write( file,"PF_SUSPEND_TASK | ", strlen("PF_SUSPEND_TASK | "), p_offset );

}

static void lkm_export_task_cputime_struct( struct task_cputime cpu, LKM_FILE file, unsigned long long *p_offset  )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\n\tutime: ", strlen("\n\tutime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(cpu.utime), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\n\tstime: ", strlen("\n\tstime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(cpu.stime), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\n\tsum_exec_runtime: ", strlen("\n\tsum_exec_runtime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(cpu.sum_exec_runtime), sizeof(unsigned long long), p_offset );
}

static void lkm_export_cputime_struct( struct cputime cpu, LKM_FILE file, unsigned long long *p_offset  )
{

	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\n\tutime: ", strlen("\n\tutime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(cpu.utime), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\n\tstime: ", strlen("\n\tstime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(cpu.stime), sizeof(cputime_t), p_offset );

}

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

	writeAmt = lkm_file_write( file,"\nflags: ", strlen("\nflags: "), p_offset );
	lkm_export_print_flags( task_ptr->flags, file, p_offset );

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
	/* room for extension */
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

	// TODO - Need to finish the PREEMPT RCU section
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


	/*
	 * Configurable scheduling
	 */
#if defined(CONFIG_SCHEDSTATS) || defined(CONFIG_TASK_DELAY_ACCT)
	writeAmt = lkm_file_write( file,"\nsched_info: ", strlen("\nsched_info: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sched_info), sizeof(struct sched_info), p_offset );
#endif


	/*
	 * Tasks
	 */
	writeAmt = lkm_file_write( file,"\ntasks: ", strlen("\ntasks: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->tasks), sizeof(struct list_head), p_offset );
#ifdef CONFIG_SMP
	// TODO - Expand structs
	writeAmt = lkm_file_write( file,"\npushable_tasks: ", strlen("\npushable_tasks: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pushable_tasks), sizeof(struct plist_node), p_offset );

	writeAmt = lkm_file_write( file,"\npushable_dl_tasks: ", strlen("\npushable_dl_tasks: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pushable_dl_tasks), sizeof(struct rb_node), p_offset );
#endif


	// TODO - This is one of the big ones
	/*
	 * Memory
	 */
	writeAmt = lkm_file_write( file,"\nmm: ", strlen("\nmm: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->mm), sizeof(void*), p_offset );

	writeAmt = lkm_file_write( file,"\nactive_mm: ", strlen("\nactive_mm: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->active_mm), sizeof(void*), p_offset );

#ifdef CONFIG_COMPAT_BRK
	writeAmt = lkm_file_write( file,"\nbrk_randomized: ", strlen("\nbrk_randomized: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->brk_randomized), sizeof(unsigned), p_offset );
#endif

	writeAmt = lkm_file_write( file,"\nvmacache_seqnum: ", strlen("\nvmacache_seqnum: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->vmacache_seqnum), sizeof(u32), p_offset );

	writeAmt = lkm_file_write( file,"\nvmacache: ", strlen("\nvmacache: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->vmacache), sizeof(void*), p_offset );

#ifdef SPLIT_RSS_COUNTING
	writeAmt = lkm_file_write( file,"\nrss_stat: ", strlen("\nrss_stat: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->rss_stat), sizeof(struct task_rss_stat), p_offset );
#endif


	/*
	 * Task State
	 */
	writeAmt = lkm_file_write( file,"\nexit_state: ", strlen("\nexit_state: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->exit_state), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nexit_code: ", strlen("\nexit_code: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->exit_code), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nexit_signal: ", strlen("\nexit_signal: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->exit_signal), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\npdeath_signal: ", strlen("\nepdeath_signal "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pdeath_signal), sizeof(int), p_offset );

	// TODO - Possibly protected by siglock (probably safe)
	writeAmt = lkm_file_write( file,"\njobctl: ", strlen("\njobctl: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->jobctl), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\npersonality: ", strlen("\npersonality: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->personality), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nin_execve: ", strlen("\nin_execve: "), p_offset );
	if( task_ptr->in_execve )
		writeAmt = lkm_file_write( file, "true", strlen("true"), p_offset );
	else
		writeAmt = lkm_file_write( file, "false", strlen("false"), p_offset );

	writeAmt = lkm_file_write( file,"\nin_iowait: ", strlen("\nin_iowait: "), p_offset );
	if( task_ptr->in_iowait )
		writeAmt = lkm_file_write( file, "true", strlen("true"), p_offset );
	else
		writeAmt = lkm_file_write( file, "false", strlen("false"), p_offset );

	writeAmt = lkm_file_write( file,"\nsched_reset_on_fork: ", strlen("\nsched_reset_on_fork: "), p_offset );
	if( task_ptr->sched_reset_on_fork )
		writeAmt = lkm_file_write( file, "true", strlen("true"), p_offset );
	else
		writeAmt = lkm_file_write( file, "false", strlen("false"), p_offset );
		
	writeAmt = lkm_file_write( file,"\nsched_contributes_to_load: ", strlen("\nsched_contributes_to_load: "), p_offset );
	if( task_ptr->sched_contributes_to_load )
		writeAmt = lkm_file_write( file, "true", strlen("true"), p_offset );
	else
		writeAmt = lkm_file_write( file, "false", strlen("false"), p_offset );

#ifdef CONFIG_MEMCG_KMEM
	writeAmt = lkm_file_write( file,"\n: ", strlen("\n: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->, sizeof(int), p_offset );
#endif

	writeAmt = lkm_file_write( file,"\natomic_flags: ", strlen("\natomic_flags: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->atomic_flags), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nrestart_block: ", strlen("\nrestart_block: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->restart_block), sizeof(struct restart_block), p_offset );

	writeAmt = lkm_file_write( file,"\npid: ", strlen("\npid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pid), sizeof(pid_t), p_offset );
		
	writeAmt = lkm_file_write( file,"\ntgid: ", strlen("\ntgid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->tgid), sizeof(pid_t), p_offset );
	
#ifdef CONFIG_CC_STACKPROTECTOR
	writeAmt = lkm_file_write( file,"\nstack_canary: ", strlen("\nstack_canary: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->stack_canary), sizeof(unsigned long), p_offset );
#endif


	/*
	 * Related Processes
	 */
	writeAmt = lkm_file_write( file,"\nreal_parent: ", strlen("\nreal_parent: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->real_parent), sizeof(struct task_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nparent: ", strlen("\nparent: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->parent), sizeof(struct task_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nchildren: ", strlen("\nchildren: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->children), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\nsibling: ", strlen("\nsibling: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sibling), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\ngroup_leader: ", strlen("\ngroup_leader: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->group_leader), sizeof(struct task_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nptraced: ", strlen("\nptraced: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ptraced), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\nptrace_entry: ", strlen("\nptrace_entry: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ptrace_entry), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\npids: ", strlen("\npids: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pids), sizeof(struct pid_link), p_offset );

	writeAmt = lkm_file_write( file,"\nthread_group: ", strlen("\nthread_group: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->thread_group), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\nthread_node: ", strlen("\nthread_node: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->thread_node), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\nvfork_done: ", strlen("\nvfork_done: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->vfork_done), sizeof(struct completion*), p_offset );

	writeAmt = lkm_file_write( file,"\nset_child_tid: ", strlen("\nset_child_tid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->set_child_tid), sizeof(int*), p_offset );

	writeAmt = lkm_file_write( file,"\nclear_child_tid: ", strlen("\nclear_child_tid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->clear_child_tid), sizeof(int*), p_offset );


	/*
	 * Timing
	 */
	writeAmt = lkm_file_write( file,"\nutime: ", strlen("\nutime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->utime), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\nstime: ", strlen("\nstime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->stime), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\nutimescaled: ", strlen("\nutimescaled: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->utimescaled), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\nstimescaled: ", strlen("\nstimescaled: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->stimescaled), sizeof(cputime_t), p_offset );

	writeAmt = lkm_file_write( file,"\ngtime: ", strlen("\ngtime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->gtime), sizeof(cputime_t), p_offset );

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
	writeAmt = lkm_file_write( file,"\nprev_cputime: ", strlen("\nprev_cputime: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->prev_cputime), sizeof(struct cputime), p_offset );
	lkm_export_cputime_struct( file, task_ptr->prev_cputime, p_offset );
#endif
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
	writeAmt = lkm_file_write( file,"\nvtime_seqlock: ", strlen("\nvtime_seqlock: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->vtime_seqlock), sizeof(seqlock_t), p_offset );

	writeAmt = lkm_file_write( file,"\nvtime_snap: ", strlen("\nvtime_snap: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->vtime_snap), sizeof(unsigned long long), p_offset );

	// TODO - Research the usage of the vtime_snap_whence enum to potentitally interpret these fields
#endif

	writeAmt = lkm_file_write( file,"\nnvcsw: ", strlen("\nnvcsw: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nvcsw), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nnivcsw: ", strlen("\nnivcsw: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nivcsw), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nstart_time: ", strlen("\nstart_time: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->start_time), sizeof(u64), p_offset );

	writeAmt = lkm_file_write( file,"\nreal_start_time: ", strlen("\nreal_start_time: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->real_start_time), sizeof(u64), p_offset );


	/*
	 * Memory Map Fault Info
	 */
	writeAmt = lkm_file_write( file,"\nmin_flt: ", strlen("\nmin_flt: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->min_flt), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nmaj_flt: ", strlen("\nmaj_flt: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->maj_flt), sizeof(unsigned long), p_offset );

	/*
	 * cputime and timers
	 */
	writeAmt = lkm_file_write( file,"\ncputime_expires: ", strlen("\ncputime_expires: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cputime_expires), sizeof(struct task_cputime), p_offset );
	lkm_export_task_cputime_struct( task_ptr->cputime_expires, file, p_offset );

	writeAmt = lkm_file_write( file,"\ncpu_timers[0]: ", strlen("\ncpu_timers[0]: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpu_timers[0]), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\ncpu_timers[1]: ", strlen("\ncpu_timers[1]: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpu_timers[1]), sizeof(struct list_head), p_offset );

	writeAmt = lkm_file_write( file,"\ncpu_timers[2]: ", strlen("\ncpu_timers[2]: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpu_timers[2]), sizeof(struct list_head), p_offset );

	/*
	 * Credentials
	 */
	writeAmt = lkm_file_write( file,"\nreal_cred: ", strlen("\nreal_cred: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->real_cred), sizeof(struct cred*), p_offset );

	writeAmt = lkm_file_write( file,"\ncred: ", strlen("\ncred: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cred), sizeof(struct cred*), p_offset );

	writeAmt = lkm_file_write( file,"\ncomm: ", strlen("\ncomm: "), p_offset );
	writeAmt = lkm_file_write( file, task_ptr->comm, strlen(task_ptr->comm), p_offset );


	/*
	 * File System
	 */
	writeAmt = lkm_file_write( file,"\nlink_count: ", strlen("\nlink_count: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->link_count), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\ntotal_link_count: ", strlen("\ntotal_link_count: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->total_link_count), sizeof(int), p_offset );

#ifdef CONFIG_SYSVIPC
	writeAmt = lkm_file_write( file,"\nsysvsem: ", strlen("\nsysvsem: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sysvsem), sizeof(struct sysv_sem), p_offset );

	writeAmt = lkm_file_write( file,"\nsysvshm: ", strlen("\nsysvshm: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sysvshm), sizeof(struct sysv_shm), p_offset );
#endif
#ifdef CONFIG_DETECT_HUNG_TASK
	writeAmt = lkm_file_write( file,"\nlast_switch_count: ", strlen("\nlast_switch_count: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->last_switch_count), sizeof(unsigned long), p_offset );
#endif

	writeAmt = lkm_file_write( file,"\nthread: ", strlen("\nthread: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->thread), sizeof(struct thread_struct), p_offset );

	writeAmt = lkm_file_write( file,"\nfs: ", strlen("\nfs: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->fs), sizeof(struct fs_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nfiles: ", strlen("\nfiles: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->files), sizeof(struct files_struct*), p_offset );

	writeAmt = lkm_file_write( file,"\nnsproxy: ", strlen("\nnsproxy: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nsproxy), sizeof(struct nsproxy*), p_offset );


	/*
	 * Signal Handling
	 */
	writeAmt = lkm_file_write( file,"\nsignal: ", strlen("\nsignal: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->signal), sizeof(struct signal_*), p_offset );

	writeAmt = lkm_file_write( file,"\nsighand: ", strlen("\nsighand: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sighand), sizeof(struct sighand_*), p_offset );

	writeAmt = lkm_file_write( file,"\nblocked: ", strlen("\nblocked: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->blocked), sizeof(sigset_t), p_offset );

	writeAmt = lkm_file_write( file,"\nreal_blocked: ", strlen("\nreal_blocked: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->real_blocked), sizeof(sigset_t), p_offset );

	writeAmt = lkm_file_write( file,"\nsaved_sigmask: ", strlen("\nsaved_sigmask: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->saved_sigmask), sizeof(sigset_t), p_offset );

	writeAmt = lkm_file_write( file,"\npending: ", strlen("\npending: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pending), sizeof(struct sigpending), p_offset );

	writeAmt = lkm_file_write( file,"\nsas_ss_sp: ", strlen("\nsas_ss_sp: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sas_ss_sp), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nsas_ss_size: ", strlen("\nsas_ss_size: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sas_ss_size), sizeof(size_t), p_offset );

	writeAmt = lkm_file_write( file,"\nnotifier: ", strlen("\nnotifier: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->notifier), sizeof(void*), p_offset );

	writeAmt = lkm_file_write( file,"\nnotifier_data: ", strlen("\nnotifier_data: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->notifier_data), sizeof(void*), p_offset );

	writeAmt = lkm_file_write( file,"\nnotifier_mask: ", strlen("\nnotifier_mask: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->notifier_mask), sizeof(sigset_t*), p_offset );

	writeAmt = lkm_file_write( file,"\ntask_works: ", strlen("\ntask_works: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->task_works), sizeof(struct callback_head*), p_offset );

	writeAmt = lkm_file_write( file,"\naudit_context: ", strlen("\naudit_context: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->audit_context), sizeof(struct audit_context*), p_offset );


#ifdef CONFIG_AUDITSYSCALL
	writeAmt = lkm_file_write( file,"\nloginuid: ", strlen("\nloginuid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->loginuid), sizeof(kuid_t), p_offset );

	writeAmt = lkm_file_write( file,"\nsessionid: ", strlen("\nsessionid: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->sessionid), sizeof(unsigned int), p_offset );
#endif

	writeAmt = lkm_file_write( file,"\nseccomp: ", strlen("\nseccomp: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->seccomp), sizeof(struct seccomp), p_offset );

	writeAmt = lkm_file_write( file,"\nparent_exec_id: ", strlen("\nparent_exec_id: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->parent_exec_id), sizeof(u32), p_offset );

	writeAmt = lkm_file_write( file,"\nself_exec_id: ", strlen("\nself_exec_id: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->self_exec_id), sizeof(u32), p_offset );

	/*
	 * RT Mutexes
	 */
#ifdef CONFIG_RT_MUTEXES
	if( lkm_export_rt_mutexes( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export real time mutexes\n" );
#endif

	/*
	 * Debugging Mutexes
	 */
#ifdef CONFIG_DEBUG_MUTEXES
	if( lkm_export_debug_mutexes( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export debugging mutexes\n" );
#endif
	
	/*
	 * IRQ flags for tracing
	 */
#ifdef CONFIG_TRACE_IRQFLAGS
	if( lkm_export_trace_irqflags( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export tracing irq\n" );
#endif

	/*
	 * lockdep
	 */
#ifdef CONFIG_LOCKDEP
	if( lkm_export_lockdep( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export lockdep\n" );
#endif

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
    /* room for extension */
#endif

#ifdef CONFIG_NUMA_BALANCING
	/* room for extension */
#endif

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

	writeAmt = lkm_file_write( file,"\nmemcg_oom->may_oom: ", strlen("\nmemcg_oom->may_oom: "), p_offset );
	if( task_ptr->memcg_oom.may_oom )
		writeAmt = lkm_file_write( file,"true", strlen("true"), p_offset );
	else
		writeAmt = lkm_file_write( file,"false", strlen("false"), p_offset );

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

#ifdef CONFIG_TRACE_IRQFLAGS
int lkm_export_trace_irqflags( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nirq_events: ", strlen("\nirq_events: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->irq_events), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirq_enable_ip: ", strlen("\nhardirq_enable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirq_enable_ip), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirq_disable_ip: ", strlen("\nhardirq_disable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirq_disable_ip), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirq_enable_event: ", strlen("\nhardirq_enable_event: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirq_enable_event), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirq_disable_event: ", strlen("\nhardirq_disable_event: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirq_disable_event), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirqs_enabled: ", strlen("\nhardirqs_enabled: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirqs_enabled), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nhardirq_context: ", strlen("\nhardirq_context: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->hardirq_context), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirq_enable_ip: ", strlen("\nsoftirq_enable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirq_enable_ip), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirq_disable_ip: ", strlen("\nsoftirq_disable_ip: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirq_disable_ip), sizeof(unsigned long), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirq_enable_event: ", strlen("\nsoftirq_enable_event: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirq_enable_event), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirq_disable_event: ", strlen("\nsoftirq_disable_event: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirq_disable_event), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirqs_enabled: ", strlen("\nsoftirqs_enabled: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirqs_enabled), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\nsoftirq_context: ", strlen("\nsoftirq_context: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->softirq_context), sizeof(int), p_offset );

	return 0;
}
#endif

#ifdef CONFIG_DEBUG_MUTEXES
int lkm_export_debug_mutexes( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nblocked_on: ", strlen("\nblocked_on: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->blocked_on), sizeof(struct mutex_waiter*), p_offset );

	return 0;
}
#endif

#ifdef CONFIG_RT_MUTEXES
int lkm_export_rt_mutexes( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\npi_waiters: ", strlen("\npi_waiters: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pi_waiters), sizeof(struct rb_root), p_offset );

	writeAmt = lkm_file_write( file,"\npi_waiters_leftmost: ", strlen("\npi_waiters_leftmost: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pi_waiters_leftmost), sizeof(struct rb_root*), p_offset );

	writeAmt = lkm_file_write( file,"\npi_blocked_on: ", strlen("\npi_blocked_on: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->pi_blocked_on), sizeof(struct rt_mutex_waiter*), p_offset );

	return 0;
}
#endif
