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
#define MALLOC_SIZE (0x100000)
#define STACK_SIZE (0x10000)

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

	// accept count as argument if specified
	if( argc > 1 )
	{
		count = atoi( args[1] );
	}

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

	// setup signal handler for testing purposes
	ss.sa_handler = sigint_handler;
	ss.sa_flags = SA_RESTART;
	sigaction( SIGINT, &ss, NULL );

	fd = open( "bashrc", O_RDONLY );
	if( fd < 0 )
	{
		printf( "Unable to open bashrc\n ");
	}

	while( count > 0 )
	{
		char tmp_buf;
		read(fd, &tmp_buf, 1);

		printf("pid: %d, iterations remaining: %d\n\t%c\n", pid, count, tmp_buf );
		sleep(1);
		count -= 1;
	}

	if( fd >= 0 )
	{
		close(fd);
	}

	free( buf_malloc );

	return 0;
	
}
