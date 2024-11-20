#!/bin/bash
#
# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Script constructing a readable "nightly name" for the project being
# built. Either printing the exact tag that the build was made from,
# or a descriptive name of the branch.
#

set -o pipefail

git symbolic-ref HEAD 2> /dev/null | cut -d/ -f3 || \
    git reflog show -n1 --pretty=format:'%gs' 2> /dev/null | sed -E 's/checkout: moving from \w+ to (\w+)/\1/'
