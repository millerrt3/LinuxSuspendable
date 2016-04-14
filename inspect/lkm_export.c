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

int lkm_export_task_struct( struct task_struct *task_ptr, LKM_FILE file, unsigned long long *p_offset )
{
	int writeAmt = 0;

	writeAmt = lkm_file_write( file,"\nstate: ", strlen("\nstate: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->state), sizeof(long), p_offset );

	writeAmt = lkm_file_write( file,"\nstack: ", strlen("\nstack: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->stack), sizeof(void*), p_offset );

	writeAmt = lkm_file_write( file,"\nflags: ", strlen("\nflags: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->flags), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nptrace: ", strlen("\nptrace: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->ptrace), sizeof(unsigned int), p_offset );

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

	writeAmt = lkm_file_write( file,"\npolicy: ", strlen("\npolicy: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->policy), sizeof(unsigned int), p_offset );

	writeAmt = lkm_file_write( file,"\nnr_cpus_allowed: ", strlen("\nnr_cpus_allowed: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->nr_cpus_allowed), sizeof(int), p_offset );

	writeAmt = lkm_file_write( file,"\ncpus_allowed: ", strlen("\ncpus_allowed: "), p_offset );
	writeAmt = lkm_file_ascii_write( file, (char*)&(task_ptr->cpus_allowed), sizeof(cpumask_t), p_offset );

	if( lkm_export_cpusets( task_ptr, file, p_offset ) < 0 )
      printk( KERN_WARNING "linux_inspect->Failed to export cpusets\n" );

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