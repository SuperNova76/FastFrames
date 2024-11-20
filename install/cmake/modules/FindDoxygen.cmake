# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(Doxygen) calls, and extend the
# environment setup file of the project with the correct Doxygen paths.
#

# The LCG include(s):
include( LCGFunctions )

# Use the helper macro to do most of the work:
lcg_wrap_find_module( Doxygen )

# Set some extra variable(s), to make the environment configuration easier:
if( DOXYGEN_FOUND AND TARGET Doxygen::doxygen )
   get_property( _doxygenExecutable TARGET Doxygen::doxygen
      PROPERTY IMPORTED_LOCATION )
   get_filename_component( DOXYGEN_BINARY_PATH ${_doxygenExecutable} PATH )
   unset( _doxygenExecutable )
endif()

# Look for Graphviz explicitly. But do not require its finding for Doxygen
# to be considered found.
if( DOXYGEN_FOUND )
   if( DOXYGEN_FIND_QUIETLY )
      find_package( graphviz QUIET )
   else()
      find_package( graphviz )
   endif()
endif()

# Set up the RPM dependency:
lcg_need_rpm( doxygen )
