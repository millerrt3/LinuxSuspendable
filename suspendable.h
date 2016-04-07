/*
 * suspendable.h
 *
 * Author: Robert Miller
 * Last Edit: 4/1/16
 *
 * Including this API will 'opt in' to the kernel functionality for suspending
 *   a running process.
 */
#ifndef _SUSPENDABLE_H
#define _SUSPENDABLE_H

#include "types.h"

#include <signal.h>
#include <unistd.h>


// Return codes
const int SIG_INSTALL_FAILED = -1;
const int SUCCESS = 0;

SuspensionMode currSuspendMode = DEFERRED;

// User-defined portions of the signal handlers
static int (*suspend)(void) = NULL;
static int (*resume)(void) = NULL;


void setSuspensionMode(SuspensionMode mode)
{
  currSuspendMode = mode;
  // TODO handle deferred suspensions
}

/**
 * @brief User API internal signal handler that calls the application
 *	    provided handler
 */
static void uapi_suspendSigHandler(int signo)
{
  if (signo != SIGUSR1)
    return;

  if (suspend != NULL && (*suspend)() == SUCCESS)
  {
    // TODO call out to kernel module
  }
  else
  {
    // TODO handle situation in which the process failed to save state
  }
}

/**
 * @brief User API internal signal handler that calls the application
 *	    provided handler
 */
static void uapi_resumeSigHandler(int signo)
{
  if (signo != SIGUSR2)
    return;

  if (resume != NULL && (*resume)() == SUCCESS)
  {
    // TODO call out to kernel module
  }
  else
  {
    // TODO handle situation in which the process failed to restore state
  }
}


int initSuspendableSystem(int (*suspendSig)(), int (*resumeSig)())
{

  printf( "sigusr1=0x%08x, sigusr2=0x%08x\n", SIGUSR1, SIGUSR2 );

  // Install signal handlers
  if (signal(SIGUSR1, uapi_suspendSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAILED;
  if (signal(SIGUSR2, uapi_resumeSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAILED;

  // Install the user-defined handlers
  suspend = suspendSig;
  resume = resumeSig;

  // Allow suspension to be enabled
  currSuspendMode = ENABLED;

  return SUCCESS;
}

#endif
