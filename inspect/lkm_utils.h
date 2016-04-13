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

typedef enum 
{
	LKM_Read  = 0,
	LKM_Write = 1,
} LKM_FilePermission;

typedef struct file* LKM_FILE;

// *************************************************************************
//                          FUNCTION PROTOTYPES
// *************************************************************************

struct task_struct* lkm_get_task_struct( int pid );

int      lkm_create_directory( const char *pathname );
int      lkm_save_to_file( const char *pathname, void *buffer, int size );
LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission );
int      lkm_file_write( LKM_FILE file, char *buffer, int size );
int      lkm_file_read( LKM_FILE file, char *buffer, int size );
void     lkm_file_close( LKM_FILE file );

// TBD - Should these be internal?
Status lkm_remove_from_pidhash( struct task_struct *task_ptr );
Status lkm_remove_from_list( struct task_struct *task_ptr );

#endif
