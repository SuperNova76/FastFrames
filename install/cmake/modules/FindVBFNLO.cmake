# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Module finding VBFNLO in the LCG release. Defines:
#  - VBFNLO_FOUND
#  - VBFNLO_INCLUDE_DIR
#  - VBFNLO_INCLUDE_DIRS
#  - VBFNLO_LIBRARIES
#  - VBFNLO_LIBRARY_DIRS
#
# Can be steered by VBFNLO_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME VBFNLO
   INCLUDE_SUFFIXES include INCLUDE_NAMES VBFNLO/utilities/VBFNLOConfig.h
   LIBRARY_SUFFIXES lib/VBFNLO
   COMPULSORY_COMPONENTS VBFNLO )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( VBFNLO DEFAULT_MSG VBFNLO_INCLUDE_DIR
   VBFNLO_LIBRARIES )
mark_as_advanced( VBFNLO_FOUND VBFNLO_INCLUDE_DIR VBFNLO_INCLUDE_DIRS
   VBFNLO_LIBRARIES VBFNLO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( vbfnlo )



