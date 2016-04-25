#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

int main()
{

	char buffer[200];
	int pid = getpid();

	memset( buffer, 0xde, 200 );


	while( 1 )
	{
		printf("Process %d running\n", pid );
		sleep(1);
	}

	return 0;
	
}