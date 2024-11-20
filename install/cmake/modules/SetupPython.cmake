# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# File setting up Python for the build of packages in this repository.
#

# Do different things based on whether the current project is building Python or
# not.
if( ATLAS_BUILD_PYTHON )
   set( Python_EXECUTABLE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python3" )
   set( Python_INCLUDE_DIRS
      $<BUILD_INTERFACE:${CMAKE_INCLUDE_OUTPUT_DIRECTORY}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
   set( Python_LIBRARIES
      $<BUILD_INTERFACE:${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}python3${CMAKE_SHARED_LIBRARY_SUFFIX}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}python3${CMAKE_SHARED_LIBRARY_SUFFIX}> )
else()
   find_package( Python COMPONENTS Interpreter Development )
endif()
