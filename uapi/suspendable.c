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
  if (signo != SUSPEND_SIGNAL)
    return;

  if (suspend != NULL && (*suspend)() == OK)
  {
    // TODO call out to kernel module
    //   Write pid to virtual file system (for suspend)
  }
  else
  {
    // TODO handle situation in which the process failed to save state
  }
}


static void resumeSigHandler(int signo)
{
  if (signo != RESTORE_SIGNAL)
    return;

  if (resume != NULL && (*resume)() == OK)
  {
    // TODO call out to kernel module
    //   Write pid to virtual file system (for restore)
  }
  else
  {
    // TODO handle situation in which the process failed to restore state
  }
}


int initSuspendableSystem(int (*suspendSig)(), int (*resumeSig)())
{
  // Install signal handlers
  if (signal(SUSPEND_SIGNAL, suspendSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAIL;
  if (signal(RESTORE_SIGNAL, resumeSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAIL;

  // Install the user-defined handlers
  suspend = suspendSig;
  resume = resumeSig;

  // Allow suspension to be enabled
  currSuspendMode = ENABLED;

  return OK;
}