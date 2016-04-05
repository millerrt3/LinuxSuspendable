#include "module_types.h" // Status

#include <linux/string.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>

// Pulled from RPi-Linux-4.1.13/linux/arch/arm/include/uapi/asm/signal.h
#define ARM_SIGNAL_MAX (32)
#define ARM_SIGNAL_MIN  (1)

Status send_signal_from_kernel( int32_t pid, int32_t signal )
{
	return kill_from_kernelspace( pid, signal );
}

Status kill_from_kernelspace( int32_t pid_id, int32_t signal )
{
	int32_t sv = 0;
	int32_t rv = 0;
	struct pid *real_pid_ptr = 0;
	struct task_struct* pid_task_ptr = 0;
	struct siginfo info;
	
	// Verify signal value
	if( signal < ARM_SIGNAL_MIN || signal > ARM_SIGNAL_MAX )
	{
		printk( KERN_WARNING "lkm_signals->send_signal_from_kernel->invalid signal; %d\n", signal );
		return INVALID_ARG;
	}
	
	// TODO - Determine if necessary - http://yongbingchen.github.io/blog/2013/03/11/sending-a-signal-from-linux-kernel/
	// rcu_read_lock();

	// Convert from virtual to real PID
	real_pid_ptr = find_vpid(pid_id);
	if (real_pid == NULL)
	{
		printk(KERN_WARNING "lkm_signals->send_signal_from_kernel->VPID translation failed; pid=0x%08x\n", pid_id );
		return INVALID_PID;
	}

	// Get the task struct associated with the provided PID
	pid_task_struct_ptr = pid_task(real_pid_ptr, PIDTYPE_PID);
	if (real_pid_ptr == NULL)
	{
		printk(KERN_WARNING "lkm_signals->send_signal_from_kernel->PID task locating failed; pid=0x%08x\n", pid_id );
		return INVALID_PID;
	}
	
	// TODO - Determine if necessary - http://yongbingchen.github.io/blog/2013/03/11/sending-a-signal-from-linux-kernel/
	// rcu_read_unlock();
	
	// TODO - determine whether targetted pid is in critical region; if yes then return appropriate error code
	
	// TODO - build siginfo structure
	memset( &info, 0, sizeof( info ) );
	info.si_signo = signal;
	info.si_code = 0;
	info.si_int = 1234;

	// send signal to process
	sv = send_sig_info( signal, &info, pid_task_ptr);
	if( sv < 0 )
	{
		printk(KERN_WARNING "lkm_signals->send_signal_from_kernel->sending signal %d to process %d failed\n", signal, pid_id );
		return ERROR;
	}
	
	return OK;
	
}
