#!/usr/bin/env python
#
# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Script used to merge confdb2 file during the build procedure. Based on
# Gaudi's merge_confdb2_parts script.
#
# Usage: mergeConfdb2.py <output> <text file with inputs>
#

# System import(s).
import sys
import os
import shelve

#The confdb2_part files rely on GaudiHandles. Importing them here so we
#can interpret these files later on
from GaudiKernel.GaudiHandles import *

# backwards-compatibility until Gaudi!1091 is merged in ATLAS
try:
    from GaudiKernel.DataHandle import DataHandle
except ModuleNotFoundError:
    from GaudiKernel.DataObjectHandleBase import DataObjectHandleBase  # old location

def main():
    '''Main function of the script

    Emulating the behaviour of a C/C++ application...
    '''

    # Parse the command line options.
    if len( sys.argv ) != 3:
        print( 'mergeConfdb2.py: Incorrect number of arguments received' )
        return 1
    ofile = sys.argv[ 1 ]
    ifile = sys.argv[ 2 ]

    # Open the text file with the input file names.
    input = open( ifile, 'r' )
    if not input:
        print( 'mergeConfdb2.py: Could not open "%s"' % ifile )
        return 1

    # Open the output shelve file.
    if os.path.exists( ofile ):
        os.remove( ofile )
        pass
    output = shelve.open( ofile )

    # Loop over the lines of the input file one by one, and process the files
    # mention by them.
    fname = input.readline()
    while fname:
        if not os.path.isfile( fname.strip() ):
            fname = input.readline()
            continue
        content = open( fname.strip(), 'rb' ).read()
        if content:
            # This call may very well fail. But instead of trying to handle
            # that failure ourselves, we let Python's default exception
            # handling deal with it.
            output.update( eval( content ) )
            pass
        fname = input.readline()
        pass

    # Close the output shelve file.
    output.close()

    # Return gracefully.
    return 0

# Execute the main function.
if __name__ == '__main__':
    sys.exit( main() )
    pass
