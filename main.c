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

  while (1)
  {
  }

  return 0;
}
