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

typedef enum 
{
	LKM_Read  = 0,
	LKM_Write = 1,
} LKM_FilePermission;

typedef struct file* LKM_FILE;

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
LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission );
int      lkm_file_write( LKM_FILE file, char *buffer, int size );
int      lkm_file_read( LKM_FILE file, char *buffer, int size );
void     lkm_file_close( LKM_FILE file );
int      lkm_file_sync( LKM_FILE file )


#endif
