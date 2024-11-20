# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(Boost) calls, and
# massage the paths produced by the system module, to make them relocatable.
#

# The LCG include(s):
include( LCGFunctions )

# Make sure that Boost is not searched for using CMAKE_PREFIX_PATH, looking
# for BoostConfig.cmake. As that would take precedence over the setting made
# using BOOST_LCGROOT.
if( BOOST_LCGROOT )
   set( Boost_NO_BOOST_CMAKE TRUE )
   set( Boost_NO_SYSTEM_PATHS TRUE )
endif()

# Use the helper macro to do most of the work:
lcg_wrap_find_module( Boost )

# Add the appropriate thread library to the library list.
# As many ATLAS packages depend on Boost code that internally
# depends on pthread.
find_package( Threads )
if( THREADS_FOUND )
   # Gaudi (at the time of writing) can't handle imported interface libraries,
   # at least in v27. So keep giving ${CMAKE_THREAD_LIBS_INIT} to it. Even
   # though when using the current externals, this variable will be empty.
   # But Gaudi anyway adds -pthread to all of its compiler commands anyway, so
   # this does not break the Gaudi build.
   if( GAUDI_ATLAS )
      list( APPEND Boost_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )
   else()
      list( APPEND Boost_LIBRARIES Threads::Threads )
   endif()
endif()

# Adding additional link options for Boost libraries cf ATLINFR-1150
lcg_os_id( _os _isValid )
if( _isValid AND "${_os}" STREQUAL "centos7" )
   list( APPEND Boost_LIBRARIES "-Wl,--copy-dt-needed-entries" )
endif()
unset( _os )
unset( _isValid )

# Set up the RPM dependency:
lcg_need_rpm( Boost )
