/**
 * @file types.h
 * @brief Contains all the type definitions used throughout the User API and the kernel module
 *
 * @version 1.0
 * @date 4/6/2016
 */

#ifndef _TYPES_H
#define _TYPES_H


#define ARM_SIGUSR1 (0x0000000a)
#define ARM_SIGUSR2 (0x0000000c)


typedef enum
{
  SUSPEND_SIGNAL = ARM_SIGUSR1,
  RESTORE_SIGNAL = ARM_SIGUSR2,
} Signals;

/**
 * @brief Represents possible return codes from functions within the module
 *
 * @details
 * 		Represents the possible return code values from the functions within the module.
 */
typedef enum
{

  OK               =  0, /**< Function was successful */
  ERROR            = -1, /**< An unknown error occurred */
  INVALID_PID      = -2, /**< Invalid PID was provided or the PID is not currently active */
  INVALID_ARG      = -3, /**< Invalid argument provided to the function */
  SIG_INSTALL_FAIL = -4, /**< Installation of a singal handler failed */
} Status;

// Values to enable/disable suspension
typedef enum 
{
  ENABLED,
  DEFERRED,
  DISABLED,
} SuspensionMode;

typedef enum
{
  SUSPEND_PROCESS,
  RESTORE_PROCESS,
  SUSPEND_COMPLETE, /**< The processes suspend handler has completed */
  RESTORE_COMPLETE, /**< The processes restore handler has completed */
  MODE_UPDATE,      /**< The suspension mode for the process has changed */

} LKM_Command;

// ----- Inspection Module Defines

#define MAX_OPERATION_SIZE 8

typedef enum
{

	LKM_TASK_STRUCT                      = 0x00000001,
	LKM_TASK_MEMORY                      = 0x00000002,
	
} LKM_InspectCmd;

typedef struct
{
	LKM_InspectCmd cmd;
	int proc_id;
} LKM_Operation_t;

#endif
