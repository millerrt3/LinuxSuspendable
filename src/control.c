/**
 * @file control.c
 * @brief Userspace control application for the linux inspection module
 *
 * @details
 *		The linux inspection module provides the ability for a userspace
 *		application to get profiled by using a module that was loaded
 *		into the linux kernel. The module provides the ability to perform
 *		a deep inspection of a userspace process. Using this module,
 *		it becomes possible to perform an analysis of a process and how it
 *		is modeled within the linux kernel.
 *
 *		This linux inspection module makes use of the sysfs mechanism
 *		to allow a userspace application to initiate operations. This
 *		application makes use of that interface and provides a series
 *		of helpful operations.
 *
 * @date April 18, 2016
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#include "types.h"

/**
 * @brief Represents the format of an operation passed through the sysfs interface
 *
 * @details
 *		The linux inspection module provides an interface through the sysfs mechanism
 *		within the kernel. The sysfs allows a userspace application to push data to the
 *		kernel module. This structure represents an operation that is being requested
 *		of the kernel module.
 */
typedef struct
{

	int pid;           /**< PID of the process to be inspected */
	int cmd_mask;      /**< Mask specifying which of the process elements should be exported */
	int repeat_count;  /**< How many times should the operation be repeated */
	int delay;         /**< Lenth of the delay in between operations */
	
} CTRL_Config_t;

/**
 * @brief Prints the help menu to the console (similar to man page)
 */
void print_help()
{

	printf( "\n" );
	printf( "-------------------------------------------------------\n" );
	printf( "                Inspection Module Control           \n" );
	printf( "-------------------------------------------------------\n" );
	printf( "This is the userspace control application for the linux\n" );
	printf( "process inspection module. The module provides the ability\n" );
	printf( "capture a running processes state into a series of human\n" );
	printf( "readable files. These files can the be used to investigate\n" );
	printf( "what information the linux kernel currently knows about a\n" );
	printf( "running process.                                        \n" );
	printf( "\n");
	printf( "SYNOPSIS:\n");
	printf( "\ttskdmp [options] [targets]\n" );
	printf( "\n");
	printf( "OPTIONS:\n" );
	printf( "\t-c, Number of sequential times that the target processes\n" );
	printf( "\t    data should be exported. Default is 1.\n");
	printf( "\n");
	printf( "\t-p, PID of the process that should be targeted for export.\n");
	printf( "\t    The default action is to profile the \"tskdmp\" process.\n");
	printf( "\n");
	printf( "\t-d, Number of seconds between each call to module for an\n");
	printf( "\t    export to take place. Default is 1 second.");
	printf( "\n");
	printf( "TARGETS:\n" );
	printf( "\t--task, A high level export of the processes task_struct will\n");
	printf( "\t    be performed. Enabled by default.\n");
    printf( "\t--mem,  A detailed export of the processes memory will\n");
	printf( "\t    be performed.\n");
	printf( "\n");
	printf( "AUTHORS:\n" );
	printf( "\tCameron Whipple, Robert Miller\n" );
	printf( "\n");

}

int main( int argc, char **args )
{

	int pid_fd = 0;
	int writeAmt = 0;
	struct timespec tspec;
	int index = 1;
	LKM_Operation_t operation;
	CTRL_Config_t config;

	memset( &config, 0, sizeof(CTRL_Config_t) );

	// process arguments
	while( index < argc )
	{

		if( strcmp( args[index], "-h" ) == 0 && (argc > index) )
		{
			print_help();
			return 0;
		}
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
		else if( strcmp( args[index], "--mem") == 0  && (argc > index) )
		{
			config.cmd_mask |= LKM_TASK_MEMORY;
		}

		index++;

	}


	/*
	 * This section of the code will process the arguments. If an argument
	 * was not passed into the application then a series of default operations
	 * are assumed and configured.
	 */
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
		config.delay = 1;
	}

	if( config.cmd_mask == 0 )
	{
		config.cmd_mask |= LKM_TASK_STRUCT;
	}

	// opens the sysfs file that is provided by the linux module
	// as method for requesting operations.
	pid_fd = open( "/sys/kernel/linux_inspect/operation", O_RDWR );
	if( pid_fd < 0 )
	{
		printf( "ERROR: Unable to open interface to module; 0x%08x\n", errno );
		printf( "        Please ensure that module is installed and running\n" );
		return 0;
	}

	// populate operation struct
	operation.cmd = config.cmd_mask;
	operation.proc_id = config.pid;

	// this loop will perform a complete iteration for each
	// of the selected repeat_counts
	for( index = 0; index < config.repeat_count; index++ )
	{

		// retrieve current time
		clock_gettime( CLOCK_REALTIME, &tspec );

		// generate name of the directory that will contain the exported data
		memset( operation.dir_name, 0, MAX_DIR_SIZE );
		snprintf( operation.dir_name, MAX_DIR_SIZE, "%d-%d-inspection", operation.proc_id, (int)tspec.tv_sec );

		// print helpful state information
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

		// provide the linux inspection module with the requested operation
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

	// return
	return 0;

}
