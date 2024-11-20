# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# This file is here to intercept find_package(Qt5) calls, and
# massage the paths produced by the system module, to make them relocatable.
#

# The LCG include(s):
include( LCGFunctions )

# Other external(s) needed for the runtime environment.
find_package( Freetype )

# Qt5 needs at least one component to be requested from it. So let's make
# Core compulsory.
list( APPEND Qt5_FIND_COMPONENTS Core )
list( REMOVE_DUPLICATES Qt5_FIND_COMPONENTS )

# Use the helper macro for the wrapping:
lcg_wrap_find_module( Qt5 )

# Names of the libraries that were processed already.
set( _processedQt5Libs )

# Helper macro for adding one Qt5 imported target's properties to the
# QT5_INCLUDE_DIRS, QT5_LIBRARY_DIRS and QT5_LIBRARIES variables.
macro( _processQt5Library name )

   # Check if this library was processed already or not.
   if( NOT ${name} IN_LIST _processedQt5Libs )

      # Remember that we've now processed this Qt5 library.
      list( APPEND _processedQt5Libs ${name} )

      # Get the include directories associated with this library.
      get_target_property( _incDirs Qt5::${name} INTERFACE_INCLUDE_DIRECTORIES )
      foreach( _incDir ${_incDirs} )
         set( _relocatableIncDir ${_incDir} )
         _lcg_make_paths_relocatable( _relocatableIncDir )
         list( APPEND QT5_INCLUDE_DIRS
            $<BUILD_INTERFACE:${_incDir}>
            $<INSTALL_INTERFACE:${_relocatableIncDir}> )
         unset( _relocatableIncDir )
      endforeach()
      unset( _incDirs )

      # Add this library itself to QT5_LIBRARIES.
      get_target_property( _location Qt5::${name} LOCATION )
      set( _relocatableLocation ${_location} )
      _lcg_make_paths_relocatable( _relocatableLocation )
      list( APPEND QT5_LIBRARIES
         $<BUILD_INTERFACE:${_location}>
         $<INSTALL_INTERFACE:${_relocatableLocation}> )
      unset( _relocatableLocation )

      # Add its directory to QT5_LIBRARY_DIRS.
      get_filename_component( _libDir ${_location} DIRECTORY )
      set( _relocatableLibDir ${_libDir} )
      _lcg_make_paths_relocatable( _relocatableLibDir )
      list( APPEND QT5_LIBRARY_DIRS
         $<BUILD_INTERFACE:${_libDir}>
         $<INSTALL_INTERFACE:${_relocatableLibDir}> )
      unset( _location )
      unset( _libDir )
      unset( _relocatableLibDir )

      # Get the (Qt5) dependencies of this library, and process them
      # recursively.
      get_target_property( _dependencies Qt5::${name}
         INTERFACE_LINK_LIBRARIES )
      foreach( _dependency ${_dependencies} )

         # Ignore non-target dependencies.
         if( NOT TARGET ${_dependency} )
            continue()
         endif()

         # Only case about Qt5 imported targets.
         if( "${_dependency}" MATCHES "^Qt5::(.*)" )
            _processQt5Library( ${CMAKE_MATCH_1} )
         endif()
      endforeach()
   endif()

endmacro( _processQt5Library )

# Set the environment variables needed for the Qt5 runtime.
if( Qt5_FOUND )

   # Get some useful paths and set some variables.
   get_target_property( QtCore_location Qt5::Core LOCATION )
   get_filename_component( QtCore_dir ${QtCore_location} DIRECTORY )
   get_filename_component( Qt_dir ${QtCore_dir} DIRECTORY )

   # Add the library directory to the runtime environment.
   set( _relocatableLibDir "${QtCore_dir}" )
   _lcg_make_paths_relocatable( _relocatableLibDir )
   set( QT5_LIBRARY_DIRS
      $<BUILD_INTERFACE:${QtCore_dir}>
      $<INSTALL_INTERFACE:${_relocatableLibDir}> )
   unset( _relocatableLibDir )

   # Set up the runtime environment variables.
   set( QT5_ENVIRONMENT
      FORCESET QTLIB "${QtCore_dir}"
      PREPEND QT_PLUGIN_PATH "${Qt_dir}/plugins" )

   # Clean up.
   unset( QtCore_location )
   unset( QtCore_dir )
   unset( Qt_dir )

   # Set up QT5_INCLUDE_DIRS and QT5_LIBRARIES as "simple" variables based
   # on the imported targets provided by the Qt5 installation.
   set( QT5_INCLUDE_DIRS )
   set( QT5_LIBRARIES )
   # Loop over the components/libraries set to be found.
   foreach( _component ${Qt5_FIND_COMPONENTS} )
      _processQt5Library( ${_component} )
   endforeach()
   # Remove all duplicates from the created variables.
   list( REMOVE_DUPLICATES QT5_INCLUDE_DIRS )
   list( REMOVE_DUPLICATES QT5_LIBRARY_DIRS )
   list( REMOVE_DUPLICATES QT5_LIBRARIES )

endif()

# Clean up.
unset(  _processedQt5Libs )

# Set up the RPM dependency.
lcg_need_rpm( Qt5 )
