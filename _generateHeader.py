################################################################################
# file: _generateHeader.py
# date: 4/17/2016
# authors: Cameron Whipple, Robert Miller
#
# details:
#   This python script will handle the process
#   of converting a .config file from the menuconfig
#   build process of the linux kernel into a usable
#   header file. The header file can then be included
#   within a modules source as a method for determining
#   what options are built into the kernel.
#
# input:
#   The menuconfig system for the linux kernel will generate a .config file.
#   That file should be copied to the same directory as the python and renamed
#   to kernel_config
#
# use: "python _generateHeader.py"
#
################################################################################

# defines all file names
_INPUT_FILE = "kernel_config"
_OUTPUT_FILE = "kernel_config.h"
_DISABLED_FILE = "kernel_disabled"

# open all necessary files
in_file = open( _INPUT_FILE, 'r' )
out_file = open( _OUTPUT_FILE, 'w' )
disable_file = open( _DISABLED_FILE, 'w' )

# place the opening portion of the header guard
out_file.write( "#ifndef _KERNEL_CONFIG_H" + '\n' )
out_file.write( "#define _KERNEL_CONFIG_H" + '\n' )
out_file.write( '\n' )

# iterate over each line in the kernel_config file
for line in in_file.readlines():

    line = line.strip()
    
    if( line.startswith( '#', 0, len(line ) ) ):
        # print( "Commented Line: " + line )
        line = line + '\n'
        disable_file.write( line )
    elif(len(line) > 0 ):
        # print( "Line: " + line )
        line = line.replace('=', ' ')
        line = "#define " + line + '\n'
        out_file.write( line )

# place the closing portion of header guard
out_file.write( '\n' )
out_file.write( "#endif" + '\n' )

# close all open files
in_file.close()
out_file.close()
disable_file.close()

    
