# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# - Locate Xrootd library
# Defines:
#
#  XROOTD_FOUND
#  XROOTD_INCLUDE_DIR
#  XROOTD_INCLUDE_DIRS
#  XROOTD_<component>_LIBRARY
#  XROOTD_<component>_FOUND
#  XROOTD_LIBRARIES
#  XROOTD_LIBRARY_DIRS
#
# Can be steered by XROOTD_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Xrootd
   INCLUDE_SUFFIXES include INCLUDE_NAMES xrootd/XrdVersion.hh
   LIBRARY_SUFFIXES lib lib64 LIBRARY_PREFIX Xrd
   DEFAULT_COMPONENTS Utils )

# Ignore system paths when an LCG release was set up:
if( XROOTD_LCGROOT )
   set( _extraXRootDArgs NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH )
endif()
lcg_system_ignore_path_setup()

# Find the xrootd executable, and set up the binary path using it:
find_program( XROOTD_EXECUTABLE xrdcp
   PATH_SUFFIXES bin PATHS ${XROOTD_LCGROOT}
   ${_extraXRootDArgs} )
get_filename_component( XROOTD_BINARY_PATH ${XROOTD_EXECUTABLE} PATH )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Xrootd DEFAULT_MSG XROOTD_INCLUDE_DIR
   XROOTD_LIBRARIES )
mark_as_advanced( XROOTD_FOUND XROOTD_INCLUDE_DIR XROOTD_INCLUDE_DIRS
   XROOTD_LIBRARIES XROOTD_LIBRARY_DIRS XROOTD_EXECUTABLE XROOTD_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( xrootd )

# Clean up:
if( _extraXRootDArgs )
   unset( _extraXRootDArgs )
endif()
