#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../types.h"

int main( int argc, char **args )
{

	int pid_fd = 0;
	int pid = 0;
	int writeAmt = 0;
	int cmd = 0;

	LKM_Operation_t operation;

	if( argc <= 1 )
	{
		printf( "ERROR: No Command Provided\n" );
		return 0;
	}

	cmd = atoi( args[1] );

	// Retrieves pid
	pid = getpid();

	// populate operation struct
	operation.cmd = cmd;
	operation.proc_id = pid;

	// convert PID to ascii representation
	printf( "Process PID: %d, Command: 0x%08x\n", pid, cmd );

	// Open the pid file from sysfs in order to tell the
	// inspection module the pid of this process
	pid_fd = open( "/sys/kernel/linux_inspect/operation", O_RDWR );
	if( pid_fd < 0 )
	{
		printf( "ERROR: Unable to open lkm_pid file; 0x%08x\n", errno );
		return 0;
	}

	// tell the inspection module the pid of this process
	writeAmt = write( pid_fd, &operation, sizeof(LKM_Operation_t) );
	if( writeAmt < sizeof(LKM_Operation_t) )
	{
		printf( "ERROR: unable to write into lkm_pid; 0x%08x\n", errno );
	}

	// close file descriptor the pid
	close( pid_fd );

	int index = 0;
	for( index = 0; index < 5; index++ )
	{
		sleep(1);
	}

	return 0;

}
