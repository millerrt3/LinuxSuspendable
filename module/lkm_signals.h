/**
 * @file lkm_signals.h
 * @brief Contains routines for interacting with signals within the kernel
 *
 * @details
 *    This services contains routines for interacting with process signals
 *    from within the kernel.
 *
 * @version 1.0
 * @date 04/06/2016
 */
#ifndef _LINUX_SIGNALS_H
#define _LINUX_SIGNALS_H

// *************************************************************************
//                               INCLUDES
// *************************************************************************
#include "../types.h" // Status

#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/pid.h>

// *************************************************************************
//                          FUNCTION PROTOTYPES
// *************************************************************************

/**
 * @brief Sends a signal to a user space process
 *
 * @details
 *    This function will send a signal to the specified user space process.
 *    This function can only be used within the kernel module. The kill()
 *    function is synonomous with this function for user space processes.
 *
 * @param[in] pid Process ID for the target process
 * @param[in] signal Signal values to send to the process
 *
 * @return See values in BStatus
 */
Status send_signal_from_kernel( int32_t pid, int32_t signal );

/**
 * @brief Sends a signal to a user space process
 *
 * @details
 *    This function will send a signal to the specified user space process.
 *    This function can only be used within the kernel module. The kill()
 *    function is synonomous with this function for user space processes.
 *
 * @param[in] pid Process ID for the target process
 * @param[in] signal Signal values to send to the process
 *
 * @return See values in BStatus
 */
Status kill_from_kernelspace( int32_t pid_id, int32_t signal );

#endif
