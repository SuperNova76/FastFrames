# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# - Locate UUID library
# Defines:
#
#  UUID_FOUND
#  UUID_INCLUDE_DIR
#  UUID_INCLUDE_DIRS
#  UUID_<component>_LIBRARY
#  UUID_<component>_FOUND
#  UUID_LIBRARIES
#  UUID_LIBRARY_DIRS
#
# Can be steered by UUID_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME UUID
  INCLUDE_SUFFIXES include INCLUDE_NAMES uuid/uuid.h
  LIBRARY_SUFFIXES lib
  COMPULSORY_COMPONENTS uuid )

# On MacOS X the uuid functions are in libSystem.dylib. So no extra library
# needs to be linked against.
if( APPLE )
   set( uuid_extra_dep )
else()
   set( uuid_extra_dep UUID_LIBRARIES )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( UUID DEFAULT_MSG UUID_INCLUDE_DIR
   ${uuid_extra_dep} )
mark_as_advanced( UUID_FOUND UUID_INCLUDE_DIR UUID_INCLUDE_DIRS
   UUID_LIBRARIES UUID_LIBRARY_DIRS )

# Clean up:
unset( uuid_extra_dep )

# Set up the RPM dependency.
lcg_need_rpm( uuid )
