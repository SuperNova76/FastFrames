# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# File setting up ROOT for the build of packages in this repository.
#

# ROOT libraries to be used.
set( ROOT_NECESSARY_LIBRARIES Core MathCore RIO Hist RooFitCore RooFit RooStats
                              HistFactory Matrix XMLParser Gpad Graf HistPainter )

# Do different things based on whether the current project is building ROOT or
# not.
if( ATLAS_BUILD_ROOT )
   # Set all variables necessary:
   set( ROOT_FOUND TRUE )
   set( ROOT_INCLUDE_DIR "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}" )
   set( ROOT_INCLUDE_DIRS
      $<BUILD_INTERFACE:${ROOT_INCLUDE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> )
   set( ROOT_BINARY_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE PATH
      "Location of the ROOT executables" )
   set( ROOT_PYTHON_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" CACHE PATH
      "Location of the ROOT python module(s)" )
   set( _ROOT_Core_library
      "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}Core${CMAKE_SHARED_LIBRARY_SUFFIX}" )
   set( ROOT_LIBRARIES )
   foreach( _lib ${ROOT_NECESSARY_LIBRARIES} )
      list( APPEND ROOT_LIBRARIES
         $<BUILD_INTERFACE:${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_SHARED_LIBRARY_PREFIX}${_lib}${CMAKE_SHARED_LIBRARY_SUFFIX}>
         $<INSTALL_INTERFACE:\${_IMPORT_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${_lib}${CMAKE_SHARED_LIBRARY_SUFFIX}> )
   endforeach()
else()
   # Set up ROOT simply through FindROOT.cmake.
   find_package( ROOT COMPONENTS ${ROOT_NECESSARY_LIBRARIES} )
   message("Found ROOT: " ${ROOT_INCLUDE_DIRS})
endif()
