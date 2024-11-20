#!/bin/bash
#
# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# Script used to merge CLID files during the build procedure.
#
# Usage: mergeClids.sh <output> <text file with inputs>
#

# Propagate errors:
set -e

# Make sure that we received the right number of arguments:
if [ "$#" -ne 2 ]; then
    echo "mergeClids.sh: Incorrect number of arguments received"
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
            cat ${fileName} > ${ofile}.tmp
        else
            cat ${fileName} >> ${ofile}.tmp
        fi
        firstFile=0
    fi
done < "${ifile}"

# If no file was created, exit already:
if [ ! -f ${ofile}.tmp ]; then
    exit 0
fi

# Remove the duplicate lines from the merged file:
if type awk >/dev/null 2>&1; then
    # I took this command from
    # http://unix.stackexchange.com/questions/171091/remove-lines-based-on-duplicates-within-one-column-without-sort
    # in order to remove duplicate lines that have the same CLID number and
    # class name. (The same first two columns.)
    awk '!seen[$1$2]++' ${ofile}.tmp > ${ofile}
    rm ${ofile}.tmp
else
    # In case AWK is not available, let's just use the unprocessed file:
    mv ${ofile}.tmp ${ofile}
fi
