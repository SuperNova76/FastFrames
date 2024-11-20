# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - CAIRO_FOUND
#  - CAIRO_BINARY_PATH
#  - CAIRO_INCLUDE_DIRS
#  - CAIRO_LIBRARY_DIRS
#  - CAIRO_<component>_FOUND
#  - CAIRO_<component>_LIBRARY
#  - CAIRO_LIBRARIES
#  - CAIRO_TRACE_EXECUTABLE
#
# Can be steered by CAIRO_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME Cairo
   INCLUDE_SUFFIXES include INCLUDE_NAMES cairo/cairo.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS cairo )

# Look for the dot executable:
find_program( CAIRO_TRACE_EXECUTABLE
   NAME cairo-trace
   PATHS ${CAIRO_LCGROOT}
   PATH_SUFFIXES bin )
if( CAIRO_TRACE_EXECUTABLE )
   get_filename_component( CAIRO_BINARY_PATH ${CAIRO_TRACE_EXECUTABLE} PATH )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Cairo DEFAULT_MSG CAIRO_INCLUDE_DIR
   CAIRO_LIBRARIES )
mark_as_advanced( CAIRO_FOUND CAIRO_INCLUDE_DIR CAIRO_INCLUDE_DIRS
   CAIRO_LIBRARY_DIRS )

# Set up the RPM dependency:
if( "${LCG_VERSION_NUMBER}" VERSION_LESS 92 )
   lcg_need_rpm( Cairo )
else()
   lcg_need_rpm( cairo )
endif()
