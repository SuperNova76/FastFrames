# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Locate the HDF5 external package. See
#
#    https://cmake.org/cmake/help/v3.8/module/FindHDF5.html
#
# for further information about the variables set up by the module.
#

# The LCG function(s):
include( LCGFunctions )

# Don't allow CMake's FindHDF5.cmake module to look for a hdf5-config.cmake
# file. (Since that would try to set up imported library targets, and we don't
# generally use imported targets for non-ATLAS projects at the moment.)
set( HDF5_NO_FIND_PACKAGE_CONFIG_FILE TRUE )

# By default make the search ignore HDF5 from under /usr. As usually we want to
# find the library either in an LCG release, or in the ATLAS externals project.
# But if the user sets the HDF5_ROOT environment variable, which can be used
# to tell CMake's FindHDF5.cmake module where to look for HDF5, then don't do
# anything extra. Just let CMake's code behave as it normally would.
if( "$ENV{HDF5_ROOT}" STREQUAL "" )
   set( _hdf5_ignorePathBackup "${CMAKE_SYSTEM_IGNORE_PATH}" )
   set( CMAKE_SYSTEM_IGNORE_PATH /usr/include /usr/bin /usr/lib /usr/lib32
      /usr/lib64 )
endif()

# Let the helper macro do most of the work. Note that while in some versions of
# CMake the built-in find-module seems to be setting HDF5_LIBRARY_DIRS, in other
# (newer) versions it doesn't. So to be safe, let's set it up ourselves all the
# time.
lcg_wrap_find_module( HDF5 NO_LIBRARY_DIRS )

# Set up the HDF5_BINARY_PATH variable.
if( HDF5_C_COMPILER_EXECUTABLE )
   # Use the directory holding h5cc as the binary directory.
   get_filename_component( HDF5_BINARY_PATH
      "${HDF5_C_COMPILER_EXECUTABLE}" DIRECTORY )
endif()

# Set up the LCG RPM dependency:
lcg_need_rpm( hdf5 )

# Restore CMAKE_SYSTEM_IGNORE_PATH:
if( DEFINED _hdf5_ignorePathBackup )
   set( CMAKE_SYSTEM_IGNORE_PATH ${_ignorePathBackup} )
   unset( _hdf5_ignorePathBackup )
endif()
