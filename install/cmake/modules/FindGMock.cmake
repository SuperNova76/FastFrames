# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# A simple module for finding the GoogleMock headers and libraries. It
# tries to mimic the behaviour of CMake's own FindGTest.cmake module,
# so have a look at that module for the definition of the different
# variables that you should expect from this module.
#
# The script can be guided by the GMOCK_LCGROOT variable.
#

# LCG include(s).
include( LCGFunctions )

# Declare the module.
lcg_external_module( NAME GMock
   INCLUDE_SUFFIXES include INCLUDE_NAMES gmock/gmock.h
   LIBRARY_SUFFIXES lib lib64
   DEFAULT_COMPONENTS gmock gmockd
   SEARCH_PATHS ${GTEST_LCGROOT} )

# If both the debug and optimised versions of the library were found, let's
# declare them correctly to CMake.
if( GMOCK_gmock_LIBRARY AND GMOCK_gmockd_LIBRARY )
   set( GMOCK_LIBRARIES optimized ${GMOCK_gmock_LIBRARY}
                        debug     ${GMOCK_gmockd_LIBRARY} )
endif()

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( GMock DEFAULT_MSG GMOCK_INCLUDE_DIR
   GMOCK_LIBRARIES )
mark_as_advanced( GMOCK_FOUND GMOCK_INCLUDE_DIR GMOCK_INCLUDE_DIRS
   GMOCK_LIBRARIES GMOCK_LIBRARY_DIRS )

# If the main GMock library was found, set up variables mimicking those
# configured by CMake's own FindGTest.cmake.
if( GMOCK_FOUND )
   find_library( GMOCK_MAIN_LIBRARY
      NAMES gmock_main PATH_SUFFIXES lib lib64
      PATHS ${GMOCK_LCGROOT} )
   find_library( GMOCK_MAIN_LIBRARY_DEBUG
      NAMES gmock_maind PATH_SUFFIXES lib lib64
      PATHS ${GMOCK_LCGROOT} )
   if( GMOCK_MAIN_LIBRARY AND GMOCK_MAIN_LIBRARY_DEBUG )
      set( GMOCK_MAIN_LIBRARIES optimized ${GMOCK_MAIN_LIBRARY}
                                debug     ${GMOCK_MAIN_LIBRARY_DEBUG} )
   elseif( GMOCK_MAIN_LIBRARY )
      set( GMOCK_MAIN_LIBRARIES ${GMOCK_MAIN_LIBRARY} )
   elseif( GMOCK_MAIN_LIBRARY_DEBUG )
      set( GMOCK_MAIN_LIBRARIES ${GMOCK_MAIN_LIBRARY_DEBUG} )
   endif()
endif()

# Set up an RPM dependency.
lcg_need_rpm( gtest FOUND_NAME GMOCK VERSION_NAME GTEST )
