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
#include "../types.h"


int custom_suspendSigHandler()
{
  printf("Suspension handler called\n");
  return OK;
}


int custom_resumeSigHandler()
{
  printf("Resume handler called\n");
  return OK;
}


int main(int argc, char** argv)
{
  int retCode = initSuspendableSystem(&custom_suspendSigHandler, &custom_resumeSigHandler);
  if (retCode != OK)
  {
    printf("Setup of the suspension utility failed.\n");
    return retCode;
  }

  while (1)
  {
  }

  return OK;
}
