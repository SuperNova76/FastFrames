# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(Java) calls, and
# massage the paths produced by the system module, to make them relocatable.
#

# The LCG include(s):
include( LCGFunctions )

# Use the helper macro to do most of the work:
lcg_wrap_find_module( Java NO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( java FOUND_NAME Java VERSION_NAME JAVA )
