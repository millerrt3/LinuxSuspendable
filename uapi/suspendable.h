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


#include "../types.h"


void setSuspensionMode(SuspensionMode mode);

/**
 * @brief User API internal signal handler that calls the application
 *	    provided handler
 */
static void suspendSigHandler(int signo);

/**
 * @brief User API internal signal handler that calls the application
 *	    provided handler
 */
static void resumeSigHandler(int signo);


int initSuspendableSystem(int (*suspendSig)(), int (*resumeSig)());


#endif
