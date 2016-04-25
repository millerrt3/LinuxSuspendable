/**
 * @file usr_utils.h
 * @brief Contains utility functions that are available within kernel space
 *
 * @details
 *		This file defines a series of routines that are available for use
 *		within a linux kernel module. Many operations must be hanlded differently
 *		when they are performed from within the linux kernel. 
 *
 * @version 1.0
 * @date 04/06/2016
 */
#ifndef _USR_UTILS_H
#define _USR_UTILS_H

// *************************************************************************
//                               INCLUDES
// *************************************************************************
#include <stdio.h>

#include "types.h"

int usr_dump_process_memory( int fd, int pid, unsigned long virtual_start, int size );

#endif