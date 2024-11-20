# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Find the ROOT libraries, headers and tools.
# Sets:
#  ROOTSYS
#  ROOT_INCLUDE_DIR
#  ROOT_INCLUDE_DIRS
#  ROOT_LIBRARIES
#  ROOT_LIBRARY_DIRS
#  ROOT_<component>_LIBRARY
#  ROOT_<component>_FOUND
#  ROOT_VERSION_STRING
#  ROOT_BINARY_PATH
#  ROOT_PYTHON_PATH
#  ROOT_INSTALL_PATH
#
# As for all the other modules, the search of this module can be steered
# using ROOT_LCGROOT.

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME ROOT
   INCLUDE_SUFFIXES include INCLUDE_NAMES TROOT.h
   LIBRARY_SUFFIXES lib
   DEFAULT_COMPONENTS Core Cling RIO Hist Tree TreePlayer Matrix
   GenVector MathCore MathMore XMLIO Graf Gui Rint Physics
   COMPULSORY_COMPONENTS Core
   BINARY_NAMES root rootcint rootcling genreflex hadd
   BINARY_SUFFIXES bin )

# Get ROOT's version:
if( EXISTS "${ROOT_INCLUDE_DIR}/ROOT/RVersion.hxx" )
   file( STRINGS "${ROOT_INCLUDE_DIR}/ROOT/RVersion.hxx" _RVersionContent )
   foreach( _RVersionLine ${_RVersionContent} )
      if( "${_RVersionLine}" MATCHES "ROOT_VERSION_MAJOR \"?([0-9]+)\"?" )
         set( ROOT_VERSION_MAJOR "${CMAKE_MATCH_1}" )
         continue()
      elseif( "${_RVersionLine}" MATCHES "ROOT_VERSION_MINOR \"?([0-9]+)\"?" )
         set( ROOT_VERSION_MINOR "${CMAKE_MATCH_1}" )
         continue()
      elseif( "${_RVersionLine}" MATCHES "ROOT_VERSION_PATCH \"?([0-9]+)\"?" )
         set( ROOT_VERSION_PATCH "${CMAKE_MATCH_1}" )
         continue()
      endif()
   endforeach()
   set( ROOT_VERSION_STRING
      "${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}/${ROOT_VERSION_PATCH}" )
elseif( EXISTS "${ROOT_INCLUDE_DIR}/RVersion.h" )
   file( STRINGS ${ROOT_INCLUDE_DIR}/RVersion.h _RVersion
      REGEX "define *ROOT_RELEASE " )
   string( REGEX MATCH
      "\"(([0-9]+)\\.([0-9]+)/([0-9]+)([a-z]*|-rc[0-9]+))\""
      _RVersion "${_RVersion}" )
   set( ROOT_VERSION_STRING ${CMAKE_MATCH_1} CACHE INTERNAL
      "Version of ROOT" )
   set( ROOT_VERSION_MAJOR ${CMAKE_MATCH_2} CACHE INTERNAL
      "Major version of ROOT" )
   set( ROOT_VERSION_MINOR ${CMAKE_MATCH_3} CACHE INTERNAL
      "Minor version of ROOT" )
   set( ROOT_VERSION_PATCH ${CMAKE_MATCH_4} CACHE INTERNAL
      "Patch version of ROOT" )
else()
   set( ROOT_VERSION_STRING "Unknown" CACHE INTERNAL
      "Version of ROOT" )
endif()

# Ignore system paths when an LCG release was set up:
if( ROOT_LCGROOT )
   set( _extraROOTArgs NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH )
endif()
lcg_system_ignore_path_setup()

# Variable(s) used in the build code:
find_path( _ROOT_BINARY_PATH rootcling
   PATH_SUFFIXES bin
   PATHS ${ROOT_LCGROOT}
   DOC "Path to the ROOT binaries"
   ${_extraROOTArgs} )

# Find the python path:
find_path( ROOT_PYTHON_PATH ROOT.py ROOT/__init__.py
   PATH_SUFFIXES lib
   PATHS ${ROOT_LCGROOT}
   DOC "Path to the ROOT python module(s)"
   ${_extraROOTArgs} )

# Mark these path variables as advanced settings.
mark_as_advanced( _ROOT_BINARY_PATH ROOT_PYTHON_PATH )

# Main installation directory for ROOT.
get_filename_component( ROOT_INSTALL_PATH ${_ROOT_BINARY_PATH} DIRECTORY )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( ROOT
   FOUND_VAR ROOT_FOUND
   REQUIRED_VARS ROOT_INSTALL_PATH ROOT_INCLUDE_DIR ROOT_LIBRARIES
   _ROOT_BINARY_PATH ROOT_PYTHON_PATH
   VERSION_VAR ROOT_VERSION_STRING )
mark_as_advanced( ROOT_FOUND ROOT_INCLUDE_DIR ROOT_INCLUDE_DIRS
   ROOT_LIBRARIES ROOT_LIBRARY_DIRS _ROOT_BINARY_PATH ROOT_PYTHON_PATH )

# Settings for Gaudi:
if( GAUDI_ATLAS )
  set( ROOT_FIND_QUIETLY TRUE )
  set( ROOT_rootcling_CMD ${_ROOT_BINARY_PATH}/rootcling )
  set( ROOT_genreflex_CMD ${_ROOT_BINARY_PATH}/genreflex )
  set( _root_required_vars ROOT_Core_LIBRARY )
  include( EnableROOT6 )
endif()

# Set additional environment variables.
if( ROOT_FOUND )
   get_filename_component( ROOTSYS ${ROOT_INCLUDE_DIR} PATH )
   if( GAUDI_ATLAS )
      set( ROOT_ENVIRONMENT SET ROOTSYS ${ROOTSYS} )
   else()
      set( ROOT_ENVIRONMENT FORCESET ROOTSYS ${ROOTSYS} )
   endif()
   list( APPEND ROOT_ENVIRONMENT SET CLING_STANDARD_PCH none )
endif()

# Set up the RPM dependency:
lcg_need_rpm( ROOT )

# Clean up:
if( _extraROOTArgs )
   unset( _extraROOTArgs )
endif()
