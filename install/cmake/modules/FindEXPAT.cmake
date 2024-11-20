# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# File intercepting find_package(EXPAT) calls, and making the created
# paths relocatable.
#

# The LCG include(s):
include( LCGFunctions )

# Use the helper macro to do most of the work:
lcg_wrap_find_module( EXPAT NO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( expat )
