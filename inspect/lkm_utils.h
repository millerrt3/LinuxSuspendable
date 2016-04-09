/**
 * @file lkm_utils.h
 * @brief 
 *
 * @details
 *
 * @version 1.0
 * @date 04/06/2016
 */
#ifndef _LKM_UTILS_H
#define _LKM_UTILS_H

// *************************************************************************
//                               INCLUDES
// *************************************************************************
#include "../types.h" // Status

// *************************************************************************
//                          FUNCTION PROTOTYPES
// *************************************************************************

/**
 * @brief Opens a file from within the kernel context
 *
 * @details
 *	This function will open a file while operating within the kernel. The
 *	open used in kernel space does not inherently work in the kernel so
 *	special process must be taken.
 *
 * @param[in] pathname Name of the file to open, max of 200 characters
 * @param[in] flags Open flags for the file
 *
 * @return file descriptor or -1 if an error occurred.
 */
int lkm_open( const char *pathname, int flags );

#endif
