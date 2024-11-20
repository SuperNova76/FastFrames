# Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
#
# Wrapper around CMake's built-in FindPNG.cmake module. Making sure that
# the found include path and library directory are set up in a relocatable
# way.
#

# The LCG include(s):
include( LCGFunctions )

# Use the helper macro for the wrapping:
lcg_wrap_find_module( PNG NO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( png )
