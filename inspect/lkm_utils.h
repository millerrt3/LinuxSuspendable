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
void lkm_print_buffer( void *buffer, int size );
int      lkm_save_to_file( const char *pathname, void *buffer, int size );
LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission );
int      lkm_file_write( LKM_FILE file, char *buffer, int size );
int      lkm_file_read( LKM_FILE file, char *buffer, int size );
void     lkm_file_close( LKM_FILE file );

/**
 * @brief Activates the process with the specified pid
 *
 * @details
 * 	This function will activate the specified process. The process will be
 *	added to the linked list and pid hash table. This function assumes that the
 *	provided task_struct has been allocated within the proper area of memory.
 *
 * @note
 *	This function will remove the task_struct from the pid_hash table
 *	and the linked listed. It will not free resources that were allocated
 *	for the task_struct.
 *
 * @param[in] task_ptr Pointer to the task_struct to activate
 *
 * @return OK if successful, <0 if error occurs
 */
Status lkm_activate_pid( struct task_struct *task_ptr );

/**
 * @brief Deactivates the provided process
 *
 * @details
 * 	This function will deactivate the specified process. The process will
 *	be deactivated by removing it from the run queue; thus, the systems
 *	scheduler will not be able to provide it time on the cpu. This function
 *	is not responsible for cleaning up the memory associated with the task_struct.
 *
 * @note
 *	This function will remove the task_struct from the pid_hash table
 *	and the linked listed. It will not free resources that were allocated
 *	for the task_struct.
 *
 * @param[in] task_ptr Pointer to the task_struct to deactivate
 *
 * @return OK if successful, <0 if error occurs
 */
Status lkm_deactivate_pid( struct task_struct *task_ptr );


// TBD - Should these be internal?
Status lkm_remove_from_pidhash( struct task_struct *task_ptr );
Status lkm_remove_from_list( struct task_struct *task_ptr );

#endif
