# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Try to find TDAQ
# Defines:
#  - TDAQ_FOUND
#  - TDAQ_INCLUDE_DIR
#  - TDAQ_INCLUDE_DIRS
#  - TDAQ_<component>_FOUND
#  - TDAQ_<component>_LIBRARY
#  - TDAQ_LIBRARIES
#  - TDAQ_LIBRARY_DIRS
#  - TDAQ_PYTHON_PATH
#  - TDAQ_BINARY_PATH
#  - TDAQ_DB_PATH
#  - TDAQ_VERSION
#
# Can be steered by TDAQ_ATROOT.
#

# Include the helper code:
include( AtlasInternals )

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
atlas_external_module( NAME tdaq
   INCLUDE_SUFFIXES installed/include INCLUDE_NAMES RunControl/RunControl.h
   LIBRARY_SUFFIXES installed/${TDAQ_PLATFORM}/lib
   COMPULSORY_COMPONENTS ipc )

# Add the platform specific header directory, if the platform agnostic directory
# was found:
if( TDAQ_INCLUDE_DIRS )
   list( APPEND TDAQ_INCLUDE_DIRS
      ${TDAQ_ATROOT}/installed/${TDAQ_PLATFORM}/include )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( tdaq DEFAULT_MSG TDAQ_INCLUDE_DIR
   TDAQ_LIBRARIES )
mark_as_advanced( TDAQ_FOUND TDAQ_INCLUDE_DIR TDAQ_INCLUDE_DIRS TDAQ_LIBRARIES
   TDAQ_LIBRARY_DIRS )

# Set TDAQ specific environment:
if( TDAQ_FOUND )
   set( TDAQ_PYTHON_PATH ${TDAQ_ATROOT}/installed/share/lib/python
      ${TDAQ_LIBRARY_DIRS} )
   set( TDAQ_BINARY_PATH ${TDAQ_ATROOT}/installed/${TDAQ_PLATFORM}/bin
      ${TDAQ_ATROOT}/installed/share/bin )
   set( TDAQ_ENVIRONMENT
      SET    TDAQ_VERSION ${TDAQ_VERSION}
      SET    TDAQ_RELEASE_BASE $ENV{TDAQ_RELEASE_BASE}
      SET    TDAQ_INST_PATH ${TDAQ_ATROOT}/installed
      SET    TDAQ_PLATFORM ${TDAQ_PLATFORM}
      SET    TDAQ_DB_PATH ${TDAQ_ATROOT}/installed/share/data
      APPEND TDAQ_DB_PATH ${TDAQ_ATROOT}/installed/databases
      APPEND TDAQ_DB_PATH ${TDAQ_ATROOT}/databases )
endif()

# When using tdaq, some macro definitions are made in the tdaq headers,
# and should not be set by the build system itself.
remove_definitions( -DHAVE_BOOL -DHAVE_DYNAMIC_CAST -DHAVE_NAMESPACES )

# Add TDAQ specific dependencies otherwise not provided by Athena:
find_package( LibXslt )

# Add the RPM dependency.
if( TDAQ_FOUND )
   # Set up a dependency on the main tdaq RPM package:
   set( _tdaqProjectName ${TDAQ_PROJECT_NAME} )
   if( "${_tdaqProjectName}" STREQUAL "" )
      set( _tdaqProjectName "tdaq" )
   endif()
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXTERNAL_RPMS
      "${_tdaqProjectName}-${TDAQ_VERSION}_${TDAQ_PLATFORM}" )
   unset( _tdaqProjectName )
endif()
