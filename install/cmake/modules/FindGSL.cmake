# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(GSL) calls, and massage
# the paths produced by the system module, to make them relocatable.
#

# The LCG function(s):
include( LCGFunctions )

# Let the helper macro do most of the work:
lcg_wrap_find_module( GSL NO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( GSL )
