#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#include "../types.h"

typedef struct
{

	int pid;
	int cmd_mask;
	int repeat_count;
	int delay;
	
} CTRL_Config_t;

void print_help()
{

	printf( "\n" );
	printf( "-------------------------------------------------------\n" );
	printf( "                Inspection Module Controller           \n" );
	printf( "-------------------------------------------------------\n" );

}

int main( int argc, char **args )
{

	int pid_fd = 0;
	int writeAmt = 0;
	struct timespec tspec;
	int index = 1;
	LKM_Operation_t operation;
	CTRL_Config_t config;

	if( argc <= 1 )
	{
		print_help();
		return 0;
	}

	memset( &config, 0, sizeof(CTRL_Config_t) );

	// process arguments
	while( index < argc )
	{

		if( strcmp( args[index], "-p" ) == 0 && (argc > index) )
		{
			config.pid = atoi( args[++index] );
			printf( "PID: %d\n", config.pid );
		}
		else if( strcmp( args[index], "-c") == 0  && (argc > index) )
		{
			config.repeat_count = atoi( args[++index] );
			printf( "repeat_count: %d\n", config.repeat_count );
		}
		else if( strcmp( args[index], "-d") == 0  && (argc > index) )
		{
			config.delay = atoi( args[++index] );
			printf( "delay: %d\n", config.delay );
		}
		else if( strcmp( args[index], "--task") == 0  && (argc > index) )
		{
			config.cmd_mask |= LKM_TASK_STRUCT;
		}

		index++;

	}

	if( config.pid == 0 )
	{
		config.pid = getpid();
	}

	if( config.repeat_count == 0 )
	{
		config.repeat_count = 1;
	}

	if( config.delay == 0 )
	{
		config.delay = 2;
	}

	if( config.cmd_mask == 0 )
	{
		config.cmd_mask |= LKM_TASK_STRUCT;
	}

	// Open the pid file from sysfs in order to tell the
	// inspection module the pid of this process
	pid_fd = open( "/sys/kernel/linux_inspect/operation", O_RDWR );
	if( pid_fd < 0 )
	{
		printf( "ERROR: Unable to open lkm_pid file; 0x%08x\n", errno );
		return 0;
	}

	// populate operation struct
	operation.cmd = config.cmd_mask;
	operation.proc_id = config.pid;

	index = 0;
	for( index = 0; index < config.repeat_count; index++ )
	{

		// retrieve current time
		clock_gettime( CLOCK_REALTIME, &tspec );

		memset( operation.dir_name, 0, MAX_DIR_SIZE );
		snprintf( operation.dir_name, MAX_DIR_SIZE, "%d-%d-inspection", operation.proc_id, (int)tspec.tv_sec );

		// convert PID to ascii representation
		printf( "Process PID: %d, Command: 0x%08x, dir=%s\n", operation.proc_id, operation.cmd, operation.dir_name );

		// create output directory
		if( mkdir( operation.dir_name, 0777 ) < 0 )
		{
			if( errno == EACCES )
			{
				printf( "Invalid Permissions to create %s\n", operation.dir_name );
				return 1;
			}
			else if( errno == EEXIST )
			{
				printf( "%s already exists\n", operation.dir_name );
			}
			else
			{
				return 2;
			}

		}

		// tell the inspection module the pid of this process
		writeAmt = write( pid_fd, &operation, sizeof(LKM_Operation_t) );
		if( writeAmt < sizeof(LKM_Operation_t) )
		{
			printf( "ERROR: unable to write into lkm_pid; 0x%08x\n", errno );
		}

		// delay between iterations
		sleep( config.delay );

	}

	// close file descriptor the pid
	close( pid_fd );

	return 0;

}
