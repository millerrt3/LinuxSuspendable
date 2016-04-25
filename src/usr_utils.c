/**
 * @file usr_utils.c
 * @brief 
 *
 * @details 
 *
 * @version 1.0
 * @date 04/06/2016
 */
#include "types.h"
#include "usr_utils.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


char usr_itoa( char nibble )
{

    if( nibble >= 0 && nibble <= 9 )
    {
        return '0' + nibble;
    }
    else if( nibble >= 10 && nibble <= 15 )
    {
        return 'a' + (nibble-10);
    }
    else
    {
        return -1;
    }

}

// ensure destination buffer is twice the size of size
void usr_binary_to_ascii( char *dest, char *ptr, int size )
{
    int index = 0;
    int dest_index = 0;

    for( index = 0; index < size; index++ )
    {

        // grab first nibble
        dest[dest_index++] = usr_itoa((ptr[index] & 0xf0) >> 4);

        // grab second nibble
        dest[dest_index++] = usr_itoa(ptr[index] & 0x0f);
    }

}
 
int usr_dump_process_memory( int fd, int pid, unsigned long virtual_start, int size )
{

    int in_fd = 0;
    int out_fd = fd;
    int bytesToRead = 0;
    int recvAmt = 0;
    int writeAmt = 0;
    int rv = 0;
    unsigned int index = 0;
    unsigned int carry_index = 0;

    char buffer[ 100 ];
    char binary_page_buffer[4096];
    char ascii_page_buffer[9192];

    // generate filename
    snprintf( buffer, 100, "/proc/%d/mem", pid );
    printf( "Filename: %s, virtual=%lu, size=%d\n", buffer, virtual_start, size );

    // open the input file
    in_fd = open( buffer, O_RDONLY );
    if( in_fd < 0 )
    {
        printf( "ERROR: usr_dump_process_memory, in_fd=0x%08x\n", in_fd );
        return INVALID_PID;
    }

    // move to the specified address in file
    if( lseek( in_fd, virtual_start, SEEK_SET ) == -1 )
    {
        printf( "ERROR: Failed to perform the seek operation\n" );
        rv = -1;
    }

    // iterate over the memory and export to the provided file descriptor
    while( size > 0 && rv == 0 )
    {

        if( size < 4096 )
        {
            bytesToRead = size;
        }
        else
        {
            bytesToRead = 4096;
        }

        // clear memory
        memset( binary_page_buffer, 0, 4096);

        // read content from processes virtual memory
        recvAmt = read( in_fd, binary_page_buffer, bytesToRead );
        if( recvAmt == 0 )
        {
            printf( "End of file reached\n" );
        }
        else if( recvAmt < bytesToRead )
        {
            printf( "ERROR: Failed to receive all bytes; recv'd=%d, requested=%d, errno=0x%08x, %s\n", recvAmt, bytesToRead, errno, strerror(errno) );
            break;
        }

        // convert binary to ascii
        usr_binary_to_ascii( ascii_page_buffer, binary_page_buffer, recvAmt );

        for( index = 0; index < recvAmt; index++, carry_index++ )
        {

            if( (carry_index % 16) == 0 && carry_index != 0 )
            {
                // insert new line
                writeAmt = write( out_fd, "\n", 1 );
            }
            else if( (carry_index % 8) == 0 && carry_index != 0 )
            {
                // insert extra space
                writeAmt = write( out_fd, " ", 1 );
            }

            // export next byte
            writeAmt = write( out_fd, &(ascii_page_buffer[index * 2]), 2 );
            writeAmt = write( out_fd, " ", 1 );

        }

        size -= recvAmt;
        
    }

    // close the file descriptor for memory
    close( in_fd );

    return 0;

}
