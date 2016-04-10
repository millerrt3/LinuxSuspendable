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
	int cmd_fd = 0;
	int pid = 0;
	int writeAmt = 0;
	int cmd = 0;
	char buffer[10];

	if( argc <= 1 )
	{
		printf( "ERROR: No Command Provided\n" );
		return 0;
	}

	cmd = atoi( args[1] );

	// Retrieves pid
	pid = getpid();

	// convert PID to ascii representation
	memset( buffer, 0, 10 );
	sprintf( buffer, "%d", pid );
	printf( "Process PID: %d, Command: 0x%08x\n", pid, cmd );

	// Open the pid file from sysfs in order to tell the
	// inspection module the pid of this process
	pid_fd = open( "/sys/kernel/linux_inspect/lkm_pid", O_RDWR );
	if( pid_fd < 0 )
	{
		printf( "ERROR: Unable to open lkm_pid file; 0x%08x\n", errno );
		return 0;
	}

	// tell the inspection module the pid of this process
	writeAmt = write( pid_fd, (void*)buffer, 4 );
	if( writeAmt < 4 )
	{
		printf( "ERROR: unable to write into lkm_pid; 0x%08x\n", errno );
		close(pid_fd);
		return 0;
	}

	// close file descriptor the pid
	close( pid_fd );

	// Process command locally
	switch( cmd )
	{

		case INS_TEST:
			break;

		default:
	   		break;

	}

	cmd_fd = open( "/sys/kernel/linux_inspect/lkm_cmd", O_RDWR );
	if( cmd_fd < 0 )
	{
		printf( "ERROR: Unable to open lkm_cmd file; 0x%08x\n", errno );
		return 0;
	}

	// tell the inspection module the pid of this process
	memset( buffer, 0, 10 );
	sprintf( buffer, "%d", cmd );
	writeAmt = write( cmd_fd, (void*)buffer, 4 );
	if( writeAmt < 4 )
	{
		printf( "ERROR: unable to write into lkm_cmd; 0x%08x\n", errno );
		close(pid_fd);
		return 0;
	}

	// close file descriptor the pid
	close( cmd_fd );

	sleep(5);

	return 0;

}
