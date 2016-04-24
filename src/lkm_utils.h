/**
 * @file lkm_utils.h
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
#ifndef _LKM_UTILS_H
#define _LKM_UTILS_H

// *************************************************************************
//                               INCLUDES
// *************************************************************************
#include "types.h" // Status

#include <linux/sched.h>
#include <linux/mm.h>


/**
 * @brief Acceptable operations for opening a file in kernel space
 *
 * @details
 *	This enumeration reflects all the available ways in which a file can
 *	be opened from within kernel space.
 */
typedef enum 
{
	LKM_Read  = 0,  /**< Open a file for read access */
	LKM_Write = 1,  /**< Open a file with write access */
} LKM_FilePermission;

/**
 * @brief Represents an open file to the routines in this file
 */
typedef struct file* LKM_FILE;

typedef int (*vmaCallback)(struct task_struct* task_ptr, unsigned long page_virtual_address, unsigned long physical_address, unsigned int page_size );

// *************************************************************************
//                          FUNCTION PROTOTYPES
// *************************************************************************

/**
 * @brief Retrieves a pointer to the task_struct
 *
 * @details
 *		This function will provide a pointer to the
 *		task_struct that corresponds to the provided pid.
 *
 * @note
 *		It is assumed that calling code has previously acquired an
 *		RCU read lock for the task_struct list.
 *
 * @param[in] pid Process ID
 *
 * @return Pointer to the task_struct, NULL if an invalid PID is provided.
 */
struct task_struct* lkm_get_task_struct( int pid );

/**
 * @brief Converts a virtual address to the physical address within the linux kernel
 *
 * @param[in] virtual_address Virtual address that should be converted to the physical
 * @return Physical address
 */
unsigned long lkm_virtual_to_physical( struct mm_struct *mm, unsigned long virtual_address );

int lkm_for_each_vma_page_in_task( struct task_struct* task_ptr, vmaCallback handler ); 

/**
 * @brief Writes binary data into a file
 *
 * @details
 *		This function will write the provided binary data into a file.
 *
 * @param[in] pathname Name and location of the file
 * @param[in] buffer Pointer to the data to be written
 * @param[in] size Number of bytes to the written into the file
 *
 * @return Number of bytes written to the file.
 */
int      lkm_save_to_file( const char *pathname, void *buffer, int size );

/**
 * @brief Converts binary data into ascii and then saves into a file
 *
 * @details
 *		This function will write the provided data into a file. The data
 *		will be treated as binary and will thus be converted to it's ascii
 *		equivalent. As such, the size of the file will be twice the size of
 *		the provided source buffer
 *
 * @param[in] pathname Name and location of the file
 * @param[in] buffer Pointer to the data to be written
 * @param[in] size Number of bytes to the written into the file
 *
 * @return Number of bytes written to the file.
 */
int      lkm_save_to_file_ascii( const char *pathname, void *buffer, int size );

/**
 * @brief Opens a file from within kernelspace
 *
 * @details
 *		This file will open the specified file from within kernelspace.
 *
 * @param[in] pathname Name of the file to open
 * @param[in] permission Necessary permissions to be attributed to the file
 *
 * @return Pointer to the open file or NULL if failed
 */
LKM_FILE lkm_file_open( const char *pathname, LKM_FilePermission permission );

/**
 * @brief Writes binary data into an already open file
 *
 * @details
 *		This function will write the provided data into an already open file
 *
 * @param[in] file Pointer to the file
 * @param[in] buffer The data to be written into the file
 * @param[in] size Number of bytes to write
 * @param[in,out] Offset to write data on input and location of last bytes written for output
 *
 * @return Number of bytes written
 */ 
int      lkm_file_write( LKM_FILE file, char *buffer, int size, unsigned long long *p_offset );

/**
 * @brief Writes binary data into an already open file once it has been converted
 *
 * @details
 *		This function will write the provided data into an already open file. The data will be
 *		converted to it's ascii equivalent prior to being written. As such, the size of the output
 *		will be twice the size of the input buffer.
 *
 * @param[in] file Pointer to the file
 * @param[in] buffer The data to be written into the file
 * @param[in] size Number of bytes to write
 * @param[in,out] Offset to write data on input and location of last bytes written for output
 *
 * @return Number of bytes written
 */
int      lkm_file_ascii_write( LKM_FILE file, char *buffer, int size, unsigned long long *p_offset );

/**
 * @brief Reads data from an open file
 *
 * @details
 *	This function will read data from an already open file within the kernel space.
 *
 * @param[in] file The open file
 * @param[in] buffer Location to place the data
 * @param[in] size Number of bytes to read
 *
 * @return Number of bytes read
 */
int      lkm_file_read( LKM_FILE file, char *buffer, int size );

/**
 * @brief Closes a previously opened file
 *
 * @param[in] File to be closed
 */
void     lkm_file_close( LKM_FILE file );

/**
 * @brief Converts a binary nibble into an ascii character
 *
 * @details
 *	This function accepts a binary nibble as input and will return
 *	that same binary value in a ascii hex value.
 *
 * @param[in] nibble binary version of nibble to convert
 *
 * @return ascii version of the provided nibble
 */
char lkm_itoa( char nibble );

/**
 * @brief Converts the provided buffer to it's ascii equivalent
 *
 * @details
 *	This routine handles the process of converting the provided
 *	buffer into it's ascii equivalent. The destination buffer should
 *	be at least twice the size of the input buffer.
 *
 * @param[in] dest Destination for the ascii buffer
 * @param[in] ptr Location of the source buffer
 * @param[in] size Number of bytes that are in the source buffer
 *
 */
void lkm_binary_to_ascii( char *dest, char *ptr, int size );

#endif
