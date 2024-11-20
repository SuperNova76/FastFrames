# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#  - GRAPHVIZ_FOUND
#  - GRAPHVIZ_BINARY_PATH
#  - GRAPHVIZ_INCLUDE_DIRS
#  - GRAPHVIZ_LIBRARY_DIRS
#  - GRAPHVIZ_<component>_FOUND
#  - GRAPHVIZ_<component>_LIBRARY
#  - GRAPHVIZ_LIBRARIES
#
# Can be steered by GRAPHVIZ_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME graphviz
   INCLUDE_SUFFIXES include INCLUDE_NAMES graphviz/graphviz_version.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS gvc )

# Look for the dot executable:
find_program( GRAPHVIZ_DOT_EXECUTABLE
   NAME dot
   PATHS ${GRAPHVIZ_LCGROOT}
   PATH_SUFFIXES bin )
if( GRAPHVIZ_DOT_EXECUTABLE )
   get_filename_component( GRAPHVIZ_BINARY_PATH ${GRAPHVIZ_DOT_EXECUTABLE}
      PATH )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( graphviz DEFAULT_MSG GRAPHVIZ_INCLUDE_DIR
   GRAPHVIZ_LIBRARIES GRAPHVIZ_DOT_EXECUTABLE )
mark_as_advanced( GRAPHVIZ_FOUND GRAPHVIZ_INCLUDE_DIR GRAPHVIZ_INCLUDE_DIRS
   GRAPHVIZ_LIBRARY_DIRS )

# Set up the RPM dependency:
lcg_need_rpm( graphviz )
