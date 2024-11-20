# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - PANGO_FOUND
#  - PANGO_BINARY_PATH
#  - PANGO_INCLUDE_DIRS
#  - PANGO_LIBRARY_DIRS
#  - PANGO_<component>_FOUND
#  - PANGO_<component>_LIBRARY
#  - PANGO_LIBRARIES
#  - PANGO_VIEW_EXECUTABLE
#
# Can be steered by PANGO_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Pango
   INCLUDE_SUFFIXES include include/pango-1.0
   INCLUDE_NAMES pango/pango.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS pango pango-1.0 )

# Look for the dot executable:
find_program( PANGO_VIEW_EXECUTABLE
   NAME pango-view
   PATHS ${PANGO_LCGROOT}
   PATH_SUFFIXES bin )
if( PANGO_VIEW_EXECUTABLE )
   get_filename_component( PANGO_BINARY_PATH ${PANGO_VIEW_EXECUTABLE} PATH )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Pango DEFAULT_MSG PANGO_INCLUDE_DIR
   PANGO_LIBRARIES PANGO_VIEW_EXECUTABLE )
mark_as_advanced( PANGO_FOUND PANGO_INCLUDE_DIR PANGO_INCLUDE_DIRS
   PANGO_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( pango )
