# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Module finding Hto4l in the LCG release. Defines:
#   - HTO4L_BINARY_PATH
#   - HTO4L_Hto4l_EXECUTABLE
#

# The LCG include(s).
include( LCGFunctions )

# Declare the module.
lcg_external_module( NAME Hto4l
   BINARY_NAMES Hto4l
   BINARY_SUFFIXES bin bin32 bin64 )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Hto4l DEFAULT_MSG
   HTO4L_Hto4l_EXECUTABLE HTO4L_BINARY_PATH  )
mark_as_advanced( HTO4L_FOUND )

# Set up the RPM dependency.
lcg_need_rpm( hto4l )
