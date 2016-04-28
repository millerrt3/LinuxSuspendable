/**
 * @file types.h
 * @brief Contains all the type definitions used throughout the User API and the kernel module
 *
 * @version 1.0
 * @date 4/6/2016
 */

#ifndef _TYPES_H
#define _TYPES_H

#define MAX_FILENAME_SIZE 40
#define MAX_DIR_SIZE 40
#define MAX_PATH_SIZE 100

typedef enum
{
	LKM_TASK_STRUCT                      = 0x00000001,
	LKM_TASK_MEMORY                      = 0x00000002,
} LKM_InspectCmd;

typedef struct
{
	LKM_InspectCmd cmd;
	int proc_id;
	char dir_name[MAX_DIR_SIZE];
} LKM_Operation_t;

/**
 * @brief Represents possible return codes from functions within the module
 *
 * @details
 *    Represents the possible return code values from the functions within the module.
 */
typedef enum
{

  OK               =  0, /**< Function was successful */
  ERROR            = -1, /**< An unknown error occurred */
  INVALID_PID      = -2, /**< Invalid PID was provided or the PID is not currently active */
  INVALID_ARG      = -3, /**< Invalid argument provided to the function */
  SIG_INSTALL_FAIL = -4, /**< Installation of a singal handler failed */
  FILE_NOT_FOUND   = -5, 
  NO_MM_ELEMENT    = -6,
} Status;

#endif
