# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Defines:
#
#  VALGRIND_FOUND
#  VALGRIND_INCLUDE_DIR
#  VALGRIND_INCLUDE_DIRS
#  VALGRIND_BINARY_PATH
#  VALGRIND_LIBEXEC_DIR
#  VALGRIND_varlgrind_EXECUTABLE
#
# Can be steered by VALGRIND_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Declare the external module:
lcg_external_module( NAME valgrind
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES valgrind/valgrind.h
   BINARY_SUFFIXES bin
   BINARY_NAMES valgrind )

# Look for the location of the "valgrind executables".
set( _valgrindOS "unknown" )
if( UNIX AND NOT APPLE )
   set( _valgrindOS "linux" )
endif()
set( _valgrindPlatform "unknown" )
if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" )
   set( _valgrindPlatform "amd64" )
elseif( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64" )
   set( _valgrindPlatform "arm64" )
endif()
set( _valgrindExtraOpts )
if( VALGRIND_LCGROOT )
   set( _valgrindExtraOpts
      NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH )
endif()
find_path( VALGRIND_LIBEXEC_DIR
   NAMES "memcheck-${_valgrindPlatform}-${_valgrindOS}"
         "callgrind-${_valgrindPlatform}-${_valgrindOS}"
   PATHS "${VALGRIND_LCGROOT}"
   PATH_SUFFIXES "lib/valgrind" "libexec/valgrind"
   DOC "Directory holding the 'valgrind executables'"
   ${_valgrindExtraOpts} )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( valgrind DEFAULT_MSG
   VALGRIND_INCLUDE_DIR VALGRIND_BINARY_PATH VALGRIND_LIBEXEC_DIR )

# Set additional environment variables:
set( VALGRIND_ENVIRONMENT
   SET VALGRIND_LIB  "${VALGRIND_LIBEXEC_DIR}"
   SET VALGRIND_OPTS "--smc-check=all" )

# Set up the RPM dependency:
lcg_need_rpm( valgrind )

# Clean up.
unset( _valgrindOS )
unset( _valgrindPlatform )
unset( _valgrindExtraOpts )
