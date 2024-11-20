# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(Python) calls, and extend
# the environment setup file of the project with the correct Python paths.
#

# This module requires CMake 3.12.
cmake_minimum_required( VERSION 3.12 )

# The LCG include(s):
include( LCGFunctions )

# Option for forcing the usage of Python2 when both Python2 and Python3 are
# available.
option( ATLAS_FORCE_PYTHON2 "Force the usage of Python2" FALSE )

# Use cached version if available, to avoid another time-consuming search.
if ( Python_VERSION_MAJOR AND Python_VERSION_MINOR )
   set( Python_FIND_VERSION ${Python_VERSION_MAJOR}.${Python_VERSION_MINOR} )
   set( Python_FIND_VERSION_MAJOR ${Python_VERSION_MAJOR} )
endif()

# Call CMake's own FindPython.cmake module. It will prefer finding Python3
# over Python2 if it's available, which is generally the behaviour that
# we want, but ATLAS_FORCE_PYTHON2 can be used to force the usage of Python2
# if needed.
if( ATLAS_FORCE_PYTHON2 )
   set( Python_FIND_VERSION 2 )
   set( Python_FIND_VERSION_MAJOR 2 )
endif()

lcg_wrap_find_module( Python )

# Cache some additional variables.
if ( Python_FOUND )
   set( Python_VERSION_MAJOR "${Python_VERSION_MAJOR}" CACHE INTERNAL
      "Python major version" )
   set( Python_VERSION_MINOR "${Python_VERSION_MINOR}" CACHE INTERNAL
      "Python minor version" )
endif()

# Variables for the environment configuration.
if( Python_EXECUTABLE )

   # Cache executable.
   set( Python_EXECUTABLE "${Python_EXECUTABLE}" CACHE INTERNAL
      "Python executable" )

   # Binary search path.
   get_filename_component( _bindir ${Python_EXECUTABLE} DIRECTORY )
   if( GAUDI_ATLAS )
      set( Python_BINARY_PATH "${_bindir}" CACHE INTERNAL
         "Python binary/executable path" )
   else()
      set( _relocatableDir ${_bindir} )
      _lcg_make_paths_relocatable( _relocatableDir )
      set( Python_BINARY_PATH
         $<BUILD_INTERFACE:${_bindir}>
         $<INSTALL_INTERFACE:${_relocatableDir}> CACHE INTERNAL
         "Python binary/executable path" )
      unset( _relocatableDir )
   endif()

   # Derive PYTHONHOME and cache.
   get_filename_component( PYTHONHOME ${_bindir}/../ ABSOLUTE )
   set( PYTHONHOME "${PYTHONHOME}" CACHE INTERNAL "PYTHONHOME" )
   if( NOT "${PYTHONHOME}" STREQUAL "" AND NOT GAUDI_ATLAS )
      set ( Python_ENVIRONMENT FORCESET PYTHONHOME ${PYTHONHOME} )
   endif()

   unset( _bindir )
endif()

# Set up the RPM dependency.
lcg_need_rpm( Python FOUND_NAME Python VERSION_NAME PYTHON )

# Clean up.
unset( _moduleName )
