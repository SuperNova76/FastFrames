# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  TBB_FOUND
#  TBB_INCLUDE_DIR
#  TBB_INCLUDE_DIRS
#  TBB_LIBRARIES
#  TBB_LIBRARY_DIRS
#  TBB_VERSION
#  TBB_INSTALL_PATH
#
# Can be steered using TBB_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME TBB
   INCLUDE_SUFFIXES include INCLUDE_NAMES tbb/tbb.h
   LIBRARY_SUFFIXES lib
   COMPULSORY_COMPONENTS tbb )

# Deduce the (interface) version of TBB.
if( TBB_INCLUDE_DIR )
   if( EXISTS "${TBB_INCLUDE_DIR}/tbb/tbb_stddef.h" )
      file( READ "${TBB_INCLUDE_DIR}/tbb/tbb_stddef.h" _tbb_version_file )
   elseif( EXISTS "${TBB_INCLUDE_DIR}/oneapi/tbb/version.h" )
      file( READ "${TBB_INCLUDE_DIR}/oneapi/tbb/version.h" _tbb_version_file )
   endif()
   string( REGEX REPLACE ".*#define TBB_VERSION_MAJOR ([0-9]+).*" "\\1"
      TBB_VERSION_MAJOR "${_tbb_version_file}" )
   string( REGEX REPLACE ".*#define TBB_VERSION_MINOR ([0-9]+).*" "\\1"
      TBB_VERSION_MINOR "${_tbb_version_file}" )
   set( TBB_VERSION "${TBB_VERSION_MAJOR}.${TBB_VERSION_MINOR}" )
endif()

# Main installation directory for TBB.
get_filename_component( TBB_INSTALL_PATH ${TBB_INCLUDE_DIR} DIRECTORY )

# Update the library names on the imported library targets to point at the
# real physical files. Since in most cases libtbb.so will actually be a text
# file with the name of the real library file. CMake can deal with that
# correctly through ${TBB_LIBRARIES}, but not when we use imported targets...
set( _components tbb ${TBB_FIND_COMPONENTS} )
list( REMOVE_DUPLICATES _components )
foreach( _comp ${_components} )
   # If this library/component was not found, don't do anything.
   if( NOT TARGET TBB::${_comp} )
      continue()
   endif()
   # Get the library name set on the imported target.
   get_target_property( _libName TBB::${_comp} IMPORTED_LOCATION )
   # Read in the first 4 bytes of the file and see if they are the ELF magic
   # number
   set( _elfMagic "7f454c46" )
   file( READ "${_libName}" _hexData OFFSET 0 LIMIT 4 HEX )
   if( "${_hexData}" STREQUAL "${_elfMagic}" )
      # Yepp, we don't need to do anything here.
      continue()
   endif()
   # Read in the entire file as text, and try to set up the path to the real
   # library based on that.
   file( READ "${_libName}" _data OFFSET 0 LIMIT 1024 )
   if( "${_data}" MATCHES "INPUT \\(([^(]+)\\)" )
      # Update the library location on the imported target.
      get_filename_component( _libDir "${_libName}" DIRECTORY )
      set_target_properties( TBB::${_comp} PROPERTIES
         IMPORTED_LOCATION "${_libDir}/${CMAKE_MATCH_1}" )
      unset( _libDir )
   endif()
   unset( _data )
   # If we got this far, then let's cross our fingers that things will work
   # correctly, and don't update anything on the imported target...
endforeach()
unset( _components )
unset( _elfMagic )
unset( _hexData )
unset( _libName )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( TBB
   FOUND_VAR TBB_FOUND
   REQUIRED_VARS TBB_INSTALL_PATH TBB_INCLUDE_DIR TBB_INCLUDE_DIRS TBB_LIBRARIES
   VERSION_VAR TBB_VERSION )
mark_as_advanced( TBB_FOUND TBB_INCLUDE_DIR TBB_INCLUDE_DIRS
   TBB_LIBRARIES TBB_LIBRARY_DIRS )

# Set up the TBB_INSTALL_DIR directory:
get_filename_component( TBB_INSTALL_DIR ${TBB_INCLUDE_DIR} PATH CACHE )

# Set up the RPM dependency:
lcg_need_rpm( tbb )
