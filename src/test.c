#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define DEFAULT_COUNT 60
#define DEFAULT_DELAY 1
#define MALLOC_SIZE (0x100)
#define STACK_SIZE (0x100)

void sigint_handler( int signum )
{
	printf( "Interrupt Triggered\n" );
	exit(0);
	return;
}

int main(int argc, char** args)
{

	struct sigaction ss;

	int fd = -1;
	char buffer[STACK_SIZE];
	int pid = getpid();
	char *buf_malloc = 0;
	int count = DEFAULT_COUNT;
	int delay = DEFAULT_DELAY;
	int index = 0;
	int enable_mem = 0;
	int enable_sig = 0;
	int enable_file = 0;

	// process arguments
	while( index < argc )
	{

		if( strcmp( args[index], "-c") == 0  && (argc > index) )
		{
			count = atoi( args[++index] );
			// printf( "repeat_count: %d\n", config.repeat_count );
		}
		else if( strcmp( args[index], "-d") == 0  && (argc > index) )
		{
			delay = atoi( args[++index] );
			// printf( "delay: %d\n", config.delay );
		}
		else if( strcmp( args[index], "--mem") == 0  && (argc > index) )
		{
			enable_mem = 1;
		}
		else if( strcmp( args[index], "--signal") == 0  && (argc > index) )
		{
			enable_sig = 1;
		}
		else if( strcmp( args[index], "--files") == 0  && (argc > index) )
		{
			enable_file = 1;
		}
		
		index++;

	}

	if( enable_mem )
	{

		// initialize all variables initial state
		memset( buffer, 0xde, STACK_SIZE );
		memset( &ss, 0, sizeof(struct sigaction));

		// update the stack with known pattern for verification purposes
		memset( buffer, 0xde, STACK_SIZE / 2 );
		memset( buffer + (STACK_SIZE / 2), 0xab, STACK_SIZE / 2 );

		// allocate a buffer
		buf_malloc = malloc( MALLOC_SIZE );
		if( buf_malloc == NULL )
		{
			printf( "Unable to acquire required amount of memory\n" );
			return 0;
		}
	

		// fill malloc'd buffer with known pattern for verification purposes.
		memset( buf_malloc, 0x12, MALLOC_SIZE );
	}

	if( enable_sig )
	{
		// setup signal handler for testing purposes
		ss.sa_handler = sigint_handler;
		ss.sa_flags = SA_RESTART;
		sigaction( SIGINT, &ss, NULL );
	}

	if( enable_file )
	{
		fd = open( "bashrc", O_RDONLY );
		if( fd < 0 )
		{
			printf( "Unable to open bashrc\n ");
		}
	}

	while( count > 0 )
	{
		char tmp_buf;

		if( enable_file )
			read(fd, &tmp_buf, 1);

		printf("pid: %d, iterations remaining: %d\n", pid, count );
		sleep(delay);
		count -= 1;
	}

	if( enable_file )
	{
		if( fd >= 0 )
		{
			close(fd);
		}
	}

	if( enable_mem )
		free( buf_malloc );

	return 0;
	
}
