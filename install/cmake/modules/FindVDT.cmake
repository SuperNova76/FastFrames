# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Locate the VDT library.
#
# Defines:
#   - VDT_FOUND
#   - VDT_INCLUDE_DIR (cached)
#   - VDT_INCLUDE_DIRS
#   - VDT_vdt_LIBRARY (cached)
#   - VDT_LIBRARIES
#   - VDT_LIBRARY_DIRS
#
# Can be steered by VDT_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME VDT
   INCLUDE_SUFFIXES include INCLUDE_NAMES vdt/vdtcore_common.h
   LIBRARY_SUFFIXES lib COMPULSORY_COMPONENTS vdt )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( VDT DEFAULT_MSG VDT_INCLUDE_DIR
   VDT_LIBRARIES )
mark_as_advanced( VDT_FOUND VDT_INCLUDE_DIR VDT_INCLUDE_DIRS
   VDT_LIBRARIES VDT_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( vdt )
