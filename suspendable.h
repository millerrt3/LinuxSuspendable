/*
 * suspendable.h
 *
 * Author: Robert Miller
 * Last Edit: 4/1/16
 *
 * Including this API will 'opt in' to the kernel functionality for suspending
 *   a running process.
 */


#include <signal.h>
#include <unistd.h>


// Return codes
const int SIG_INSTALL_FAILED = -1;
const int SUCCESS = 0;


// Values to enable/disable suspension
enum SuspensionMode
{
  ENABLED,
  DEFERRED,
  DISABLED,
};
SuspensionMode currSuspendMode = SuspensionMode.DEFERRED;


// User-defined portions of the signal handlers
int (*suspend)(void) = NULL;
int (*resume)(void) = NULL;


void setSuspensionMode(SuspensionMode mode)
{
  currSuspendMode = mode;
  // TODO handle deferred suspensions
}


void suspendSigHandler(int signo)
{
  if (signo != SIGUSR1)
    return;

  if (suspend != NULL && (*suspend)() == SUCCESS)
  {
    // TODO call out to kernel module
  }
}


void resumeSigHandler(int signo)
{
  if (signo != SIGUSR2)
    return;

  if (resume != NULL && (*resume)() == SUCCESS)
  {
    // TODO call out to kernel module
  }
}


int initSuspendableSystem(int (*suspendSig)(), int (*resumeSig)())
{
  // Install signal handlers
  if (signal(SIGUSR1, suspendSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAILED;
  if (signal(SIGUSR2, resumeSigHandler) == SIG_ERR)
    return SIG_INSTALL_FAILED;

  // Install the user-defined handlers
  suspend = suspendSig;
  resume = resumeSig;

  // Allow suspension to be enabled
  currSuspendMode = ENABLED;

  return SUCCESS;
}