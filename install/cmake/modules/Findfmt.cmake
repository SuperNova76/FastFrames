# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(fmt) calls, and massage
# the paths produced by the auto-generated code, to make them relocatable.
#

# The LCG function(s).
include( LCGFunctions )

# Rely on the auto-generated CMake code's fmt::fmt imported target.
lcg_wrap_find_module( fmt
   IMPORTED_TARGETS fmt::fmt
   NO_LIBRARY_DIRS )

# Set up the RPM dependency.
lcg_need_rpm( fmt )
