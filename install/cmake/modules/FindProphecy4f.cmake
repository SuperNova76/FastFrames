# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Module finding Prophecy4f in the LCG release. Defines:
#   - PROPHECY4F_BINARY_PATH
#   - PROPHECY4F_Prophecy4f_EXECUTABLE
#

# The LCG include(s).
include( LCGFunctions )

# Declare the module.
lcg_external_module( NAME Prophecy4f
   BINARY_NAMES Prophecy4f
   BINARY_SUFFIXES bin bin32 bin64 )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Prophecy4f DEFAULT_MSG
   PROPHECY4F_Prophecy4f_EXECUTABLE PROPHECY4F_BINARY_PATH  )
mark_as_advanced( PROPHECY4F_FOUND  )

# Set up the RPM dependency.
lcg_need_rpm( prophecy4f )
