# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Try to find xz (lzma).
#
# Defines:
#  - XZ_FOUND
#  - XZ_INCLUDE_DIR
#  - XZ_LIBRARIES
#  - XZ_LIBRARY_DIRS
#
# Can be steered by XZ_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME xz
   INCLUDE_SUFFIXES include INCLUDE_NAMES lzma.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS lzma )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( xz DEFAULT_MSG XZ_INCLUDE_DIR
   XZ_LIBRARIES XZ_LIBRARY_DIRS )
mark_as_advanced( XZ_FOUND XZ_INCLUDE_DIR
   XZ_LIBRARIES XZ_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_os_id( _os _isValid )
if( _isValid AND "${_os}" STREQUAL "slc6" )
    lcg_need_rpm( xz )
endif()
unset( _os )
unset( _isValid )
