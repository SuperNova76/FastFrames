#!/bin/bash
#
# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# Script used to merge text files during the build procedure.
#
# Usage: mergeFiles.sh <output> <text file with inputs>
#

# Propagate errors:
set -e

# Make sure that we received the right number of arguments:
if [ "$#" -ne 2 ]; then
    echo "mergeFiles.sh: Incorrect number of arguments received"
    exit 1
fi

# Get the input/output file names:
ofile=$1
ifile=$2

# Merge the files listed in the input text file:
firstFile=1
while IFS='' read -r fileName || [[ -n "$fileName" ]]; do
    # Only consider files that really exist:
    if [ -f ${fileName} ]; then
        # Overwrite, or append the file:
        if [ ${firstFile} = 1 ]; then
            cat ${fileName} > ${ofile}
        else
            cat ${fileName} >> ${ofile}
        fi
        firstFile=0
    fi
done < "${ifile}"
