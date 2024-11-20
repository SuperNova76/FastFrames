# Copyright (C) 2002-2018 CERN for the benefit of the ATLAS collaboration
#
# Find the 'libxkbcommon' libraries, headers and tools.
# Sets:
#  LIBXKBCOMMON_FOUND
#  LIBXKBCOMMON_INCLUDE_DIR 
#  LIBXKBCOMMON_INCLUDE_DIRS
#  LIBXKBCOMMON_LIBRARIES 
#  LIBXKBCOMMON_LIBRARY_DIR

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME libxkbcommon
   INCLUDE_SUFFIXES include INCLUDE_NAMES xkbcommon/xkbcommon.h
   LIBRARY_SUFFIXES lib 
   DEFAULT_COMPONENTS xkbcommon )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )

find_package_handle_standard_args( libxkbcommon DEFAULT_MSG LIBXKBCOMMON_INCLUDE_DIR
   LIBXKBCOMMON_LIBRARIES )

mark_as_advanced( LIBXKBCOMMON_FOUND LIBXKBCOMMON_INCLUDE_DIR LIBXKBCOMMON_INCLUDE_DIRS
   LIBXKBCOMMON_LIBRARIES LIBXKBCOMMON_LIBRARY_DIRS )

# -- Set the QT_XKB_CONFIG_ROOT environment variable:
if( LIBXKBCOMMON_FOUND )
  set( LIBXKBCOMMON_ENVIRONMENT PREPEND QT_XKB_CONFIG_ROOT ${LIBXKBCOMMON_LCGROOT}/lib )
endif()

# Set up the RPM dependency:
lcg_need_rpm( libxkbcommon )
