/*
 * main.c
 *
 * Author: Robert Miller
 * Last Edit: 4/1/16
 *
 * This program constitutes a test of the suspension mechanic implemented as a
 *   final project for CSE 522 by Robert Miller & Cameron Whipple
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "suspendable.h"


int suspendSigHandler()
{
  printf("Suspension handler called\n");
  return SUCCESS;
}


int resumeSigHandler()
{
  printf("Resume handler called\n");
  return SUCCESS;
}


int main(int argc, char** argv)
{

  int pid = 0;

  pid = getpid();
  printf( "pid=%d\n", pid );

  int retCode = initSuspendableSystem(&suspendSigHandler, &resumeSigHandler);
  if (retCode != SUCCESS)
  {
    printf("Setup of the suspension utility failed.\n");
    return retCode;
  }

  while( 1 )
  {

  int fd = open( "/sys/kernel/linux_suspendable/pid_to_suspend", O_RDWR );
  if( fd < 0 )
    return 0;

  char buffer[100];
  memset(buffer, 0, 100);
  sprintf( buffer, "%d", pid );
  printf( "Writing: %s\n", buffer );
  if( write( fd, buffer, strlen(buffer) ) < strlen(buffer) )
  {
    printf( "Failed to write\n" );
  }

  close( fd );

  sleep( 2 );

  }

  while (1)
  {
  }

  return 0;
}
