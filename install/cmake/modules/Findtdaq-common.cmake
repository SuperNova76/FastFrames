# Copyright (C) 2002-2021 CERN for the benefit of the ATLAS collaboration
#
# Try to find TDAQ-COMMON.
# Defines:
#  - TDAQ-COMMON_FOUND
#  - TDAQ-COMMON_INCLUDE_DIR
#  - TDAQ-COMMON_INCLUDE_DIRS
#  - TDAQ-COMMON_<component>_FOUND
#  - TDAQ-COMMON_<component>_LIBRARY
#  - TDAQ-COMMON_LIBRARIES
#  - TDAQ-COMMON_LIBRARY_DIRS
#  - TDAQ-COMMON_PYTHON_PATH
#  - TDAQ-COMMON_BINARY_PATH
#  - TDAQ_PYTHON_HOME
#
# Can be steered by TDAQ-COMMON_ATROOT.
#

# Include the helper code:
include( AtlasInternals )

# For python location:
find_package( Python COMPONENTS Interpreter )

# Select the platform to pick up the tdaq release for.
if( NOT "$ENV{TDAQ_PLATFORM}" STREQUAL "" )
   set( TDAQ_PLATFORM_DEFAULT "$ENV{TDAQ_PLATFORM}" )
else()
   atlas_platform_id( TDAQ_PLATFORM_DEFAULT )
endif()
set( TDAQ_PLATFORM "${TDAQ_PLATFORM_DEFAULT}" CACHE STRING
   "Platform for which the TDAQ releases should be picked up" )
unset( TDAQ_PLATFORM_DEFAULT )

# Declare the module:
atlas_external_module( NAME tdaq-common
   INCLUDE_SUFFIXES installed/include INCLUDE_NAMES eformat/eformat.h
   LIBRARY_SUFFIXES installed/${TDAQ_PLATFORM}/lib
   COMPULSORY_COMPONENTS eformat ers )

# Check whether tdaq-common comes with its own externals or not. If yes, then
# let's make sure that that path is included in CMAKE_PREFIX_PATH.
if( TDAQ-COMMON_INCLUDE_DIR )
   get_filename_component( _TDAQ_COMMON_MAIN_DIR "${TDAQ-COMMON_INCLUDE_DIR}"
      DIRECTORY CACHE )
   set( _TDAQ_COMMON_EXTERNALS_DIR
      "${_TDAQ_COMMON_MAIN_DIR}/external/${TDAQ_PLATFORM}" )
   if( ( EXISTS "${_TDAQ_COMMON_EXTERNALS_DIR}" ) AND NOT
       ( "$ENV{CMAKE_PREFIX_PATH}" MATCHES "${_TDAQ_COMMON_EXTERNALS_DIR}" ) )
      message( STATUS "Setting up the tdaq-common externals..." )
      set( ENV{CMAKE_PREFIX_PATH}
         "$ENV{CMAKE_PREFIX_PATH}:${_TDAQ_COMMON_EXTERNALS_DIR}" )
   endif()
   unset( _TDAQ_COMMON_EXTERNALS_DIR )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( tdaq-common DEFAULT_MSG
   TDAQ-COMMON_INCLUDE_DIR TDAQ-COMMON_LIBRARIES Python_FOUND )
mark_as_advanced( TDAQ-COMMON_FOUND TDAQ-COMMON_INCLUDE_DIR
   TDAQ-COMMON_INCLUDE_DIRS TDAQ-COMMON_LIBRARIES TDAQ-COMMON_LIBRARY_DIRS )

# Set TDAQ specific environment:
if( TDAQ-COMMON_FOUND )
   set( TDAQ-COMMON_PYTHON_PATH ${TDAQ-COMMON_ATROOT}/installed/share/lib/python
      ${TDAQ-COMMON_LIBRARY_DIRS} )
   set( TDAQ-COMMON_BINARY_PATH
      ${TDAQ-COMMON_ATROOT}/installed/${TDAQ_PLATFORM}/bin
      ${TDAQ-COMMON_ATROOT}/installed/share/bin )
   set( TDAQ-COMMON_ENVIRONMENT
      SET TDAQ_RELEASE_BASE $ENV{TDAQ_RELEASE_BASE}
      SET TDAQ_PYTHON_HOME  ${PYTHONHOME}
      SET TDAQ_PLATFORM     ${TDAQ_PLATFORM} )
endif()

# Add TDAQ specific dependencies otherwise not provided by Athena:
find_package( future )

# Add the RPM dependencies:
if( TDAQ-COMMON_FOUND )
   # Set up a dependency on the main tdaq-common RPM package:
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXTERNAL_RPMS
      "tdaq-common-${TDAQ-COMMON_VERSION}_${TDAQ_PLATFORM}" )
endif()
