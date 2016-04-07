/*
 * suspendable.c
 *
 * Author: Robert Miller
 * Last Edit: 4/7/16
 *
 * Implementation of the suspension code defined in suspendable.h
 */


#include <signal.h>
#include <unistd.h>

#include "suspendable.h"


static SuspensionMode currSuspendMode = DEFERRED;

// User-defined portions of the signal handlers
static int (*suspend)(void) = NULL;
static int (*resume)(void) = NULL;


void setSuspensionMode(SuspensionMode mode)
{
  currSuspendMode = mode;
  // TODO handle deferred suspensions
}


static void suspendSigHandler(int signo)
{
  if (signo != SIGUSR1)
    return;

  if (suspend != NULL && (*suspend)() == OK)
  {
    // TODO call out to kernel module
  }
  else
  {
    // TODO handle situation in which the process failed to save state
  }
}


static void resumeSigHandler(int signo)
{
  if (signo != SIGUSR2)
    return;

  if (resume != NULL && (*resume)() == OK)
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
  // Install signal handlers
  if (signal(SIGUSR1, suspendSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAIL;
  if (signal(SIGUSR2, resumeSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAIL;

  // Install the user-defined handlers
  suspend = suspendSig;
  resume = resumeSig;

  // Allow suspension to be enabled
  currSuspendMode = ENABLED;

  return OK;
}