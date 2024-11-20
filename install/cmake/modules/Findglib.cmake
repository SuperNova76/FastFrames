# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Find the 'glib' libraries, headers and tools.
# Sets:
#  GLIB_FOUND
#  GLIB_INCLUDE_DIR
#  GLIB_INCLUDE_DIRS
#  GLIB_LIBRARIES
#  GLIB_LIBRARY_DIR

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME glib
   INCLUDE_SUFFIXES include include/glib-2.0
   INCLUDE_NAMES glib.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS glib glib-2.0 )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( glib DEFAULT_MSG GLIB_INCLUDE_DIR
   GLIB_LIBRARIES )
mark_as_advanced( GLIB_FOUND GLIB_INCLUDE_DIR GLIB_INCLUDE_DIRS
   GLIB_LIBRARIES GLIB_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( glib )
