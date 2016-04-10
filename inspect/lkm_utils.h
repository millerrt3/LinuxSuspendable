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

#include <linux/sched.h>

#if 0
typedef enum 
{
	LKM_Read  = 0,
	LKM_Write = 1,
} LKM_FilePermission;
#endif

// typedef struct file* LKM_FILE;

// *************************************************************************
//                          FUNCTION PROTOTYPES
// *************************************************************************

struct task_struct* lkm_get_task_struct( int pid );
void lkm_print_buffer( void *buffer, int size );

#if 0
LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission );
int      lkm_file_write( LKM_FILE file, char *buffer, int size );
int      lkm_file_read( LKM_FILE file, char *buffer, int size );
void     lkm_file_close( LKM_FILE file );
int      lkm_file_sync( LKM_FILE file )
#endif




#endif
