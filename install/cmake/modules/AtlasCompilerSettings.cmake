# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# This file collects settings fine-tuning all the compiler and linker options
# used in an ATLAS build in one place. It is included by default when using
# any parts of AtlasCMake.
#

# CMake include(s).
include( CheckCXXCompilerFlag )

# Guard this file against multiple inclusions.
get_property( _compilersSet GLOBAL PROPERTY ATLAS_COMPILERS_SET SET )
if( _compilersSet )
   unset( _compilersSet )
   return()
endif()
set_property( GLOBAL PROPERTY ATLAS_COMPILERS_SET TRUE )

# Tell the user what we're doing.
message( STATUS "Setting ATLAS specific build flags" )

# Set the C++ standard to use for the build.
set( CMAKE_CXX_STANDARD_DEFAULT 17 )
if( ( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) AND
      ( "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "13" ) ) OR
    ( ( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) OR
        ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "LLVM" ) ) AND
      ( "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "16" ) ) )
   # Use C++20 by default with GCC 13+ and Clang16.
   set( CMAKE_CXX_STANDARD_DEFAULT 20 )
endif()
set( CMAKE_CXX_STANDARD "${CMAKE_CXX_STANDARD_DEFAULT}" CACHE STRING
   "C++ standard used for the build" )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE CACHE BOOL
   "C++ standard required as a minimum for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Set the C++ standard to use with CUDA..
set( CMAKE_CUDA_STANDARD 14 CACHE STRING "C++ standard used with CUDA" )
set( CMAKE_CUDA_STANDARD_REQUIRED TRUE CACHE BOOL
   "Flag stating that the CUDA C++ standard is a hard requirement" )

# Set the default architectures to compile CUDA binary code for.
set( CMAKE_CUDA_ARCHITECTURES "50;61;75" CACHE STRING
   "Architectures to generate CUDA device code for" )

# Set default values for the optimisation settings, ones that could
# be overridden from the command line.
if( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" ) OR
    ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) )

   # Provide custom defaults for GCC (and compatible compilers). The only
   # modification wrt. the CMake default values, at the time of writing, is to
   # use -O2 for release mode instead of -O3.
   set( ATLAS_CXX_FLAGS_RELEASE "-DNDEBUG -O2"
      CACHE STRING "Default optimisation settings for Release mode" )
   set( ATLAS_CXX_FLAGS_RELWITHDEBINFO "-DNDEBUG -O2 -g"
      CACHE STRING "Default optimisation settings for RelWithDebInfo mode" )
   set( ATLAS_CXX_FLAGS_DEBUG "-g"
      CACHE STRING "Default optimisation settings for Debug mode" )

else()

   # For all other compilers just accept the CMake defaults.
   set( ATLAS_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}"
      CACHE STRING "Default optimisation settings for Release mode" )
   set( ATLAS_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}"
      CACHE STRING "Default optimisation settings for RelWithDebInfo mode" )
   set( ATLAS_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
      CACHE STRING "Default optimisation settings for Debug mode" )

endif()

# Override the default values forcefully. Since by this time the cache
# values have already been set by CMake.
set( CMAKE_CXX_FLAGS_RELEASE "${ATLAS_CXX_FLAGS_RELEASE}"
   CACHE STRING "Default optimisation settings for Release mode" FORCE )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${ATLAS_CXX_FLAGS_RELWITHDEBINFO}"
   CACHE STRING "Default optimisation settings for RelWithDebInfo mode" FORCE )
set( CMAKE_CXX_FLAGS_DEBUG "${ATLAS_CXX_FLAGS_DEBUG}"
   CACHE STRING "Default optimisation settings for Release mode" FORCE )

# Function helping with extending global flags
function( _add_flag name value )

   # Escape special characters in the value:
   set( matchedValue "${value}" )
   foreach( c "*" "." "^" "$" "+" "?" )
      string( REPLACE "${c}" "\\${c}" matchedValue "${matchedValue}" )
   endforeach()

   # Check if the variable already has this value in it:
   if( "${${name}}" MATCHES "${matchedValue}" )
      return()
   endif()

   # If not, then let's add it now:
   set( ${name} "${${name}} ${value}" CACHE STRING
      "Compiler setting" FORCE )

endfunction( _add_flag )

# Set architecture level to v2 - enable generation of code using SSE4.2,
# popcnt instructions. But only use the flag if it's available from the
# compiler. And the user didn't specify some other -march flag yet.
if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" )
   set( X86_FLAG "-march=x86-64-v2" )
   check_cxx_compiler_flag( "${X86_FLAG}" ATLAS_SSE42_SUPPORTED )
   if( ATLAS_SSE42_SUPPORTED AND
       ( NOT "${CMAKE_CXX_FLAGS}" MATCHES "-march=" ) )
      _add_flag( CMAKE_CXX_FLAGS "${X86_FLAG}" )
   endif()
   unset( X86_FLAG )
endif()

# Fortran settings. Make the check depend on the CXX compiler that's in use.
# As a Fortran compiler may not have been set up yet when this file is included.
if( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" ) OR
    ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) )
   _add_flag( CMAKE_Fortran_FLAGS "-ffixed-line-length-132" )
   _add_flag( CMAKE_Fortran_FLAGS_RELEASE "-funroll-all-loops" )
   _add_flag( CMAKE_Fortran_FLAGS_RELWITHDEBINFO "-funroll-all-loops" )
endif()

# Turn on compiler warnings for the relevant build modes. In a default
# build don't use extra warnings.
if( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" ) OR
    ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) )
   foreach( mode RELEASE RELWITHDEBINFO DEBUG )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wall" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wno-long-long" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wno-deprecated" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wno-unused-local-typedefs" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wwrite-strings" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wpointer-arith" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Woverloaded-virtual" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Wextra" )
      _add_flag( CMAKE_CXX_FLAGS_${mode} "-Werror=return-type" )
   endforeach()
   _add_flag( CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined" )
endif()
foreach( mode RELEASE RELWITHDEBINFO DEBUG )
   _add_flag( CMAKE_CXX_FLAGS_${mode} "-pedantic" )
endforeach()

#
# Set up the GCC checker plugin(s):
#
option( ATLAS_USE_GCC_CHECKERS
   "Enable using the GCC checker plugins if they are available" ON )
set( ATLAS_GCC_CHECKERS ""
   CACHE STRING "Checker(s) to activate during compilation" )
set( ATLAS_GCC_CHECKERS_CONFIG ""
   CACHE STRING "Configuration file(s) for the GCC checker plugins" )
if( ATLAS_USE_GCC_CHECKERS AND ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) )
   # First check if the library is even available:
   find_library( _gccchecker "checker_gccplugins"
     DOC "Path to the GCC checker library plugin to use for the build" )
   mark_as_advanced( _gccchecker )
   if( _gccchecker )
      message( STATUS "Found checker_gccplugins library under: ${_gccchecker}" )
      _add_flag( CMAKE_CXX_FLAGS "-fplugin=${_gccchecker}" )
      if( NOT "${ATLAS_GCC_CHECKERS}" STREQUAL "" )
         message( STATUS "Enabling checker_gccplugins with checker(s): "
            "${ATLAS_GCC_CHECKERS}" )
         _add_flag( CMAKE_CXX_FLAGS
            "-fplugin-arg-libchecker_gccplugins-checkers=${ATLAS_GCC_CHECKERS}" )
      endif()
      if( NOT "${ATLAS_GCC_CHECKERS_CONFIG}" STREQUAL "" )
         _add_flag( CMAKE_CXX_FLAGS
            "-fplugin-arg-libchecker_gccplugins-config=${ATLAS_GCC_CHECKERS_CONFIG}" )
      endif()
   else()
      message( STATUS "checker_gccplugins library not found" )
   endif()
endif()

# Make all the following settings only for GCC and Clang.
# Note that Apple's Clang compiler ("AppleClang") is excluded, as it
# interestingly doesn't support *any* of these linker options.
if( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) OR
    ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) )

   # By default use the --as-needed option for everything. Remember that this
   # behaviour can be turned off in individual packages using
   # atlas_disable_as_needed().
   _add_flag( CMAKE_EXE_LINKER_FLAGS    "-Wl,--as-needed" )
   _add_flag( CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed" )
   _add_flag( CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed" )

   _add_flag( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )
   _add_flag( CMAKE_MODULE_LINKER_FLAGS "-Wl,--no-undefined" )

   # Set platform specific segment alignment values. In order to minimise the
   # sizes of the produced binaries as much as possible. For unknown
   # architectures don't set anything explicitly.
   if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" )
      _add_flag( CMAKE_SHARED_LINKER_FLAGS "-Wl,-z,max-page-size=0x1000" )
      _add_flag( CMAKE_MODULE_LINKER_FLAGS "-Wl,-z,max-page-size=0x1000" )
   elseif( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64" )
      _add_flag( CMAKE_SHARED_LINKER_FLAGS "-Wl,-z,max-page-size=0x10000" )
      _add_flag( CMAKE_MODULE_LINKER_FLAGS "-Wl,-z,max-page-size=0x10000" )
   endif()

   # This has some benefits in the speed of loading libraries.
   _add_flag( CMAKE_EXE_LINKER_FLAGS    "-Wl,--hash-style=both" )
   _add_flag( CMAKE_SHARED_LINKER_FLAGS "-Wl,--hash-style=both" )
   _add_flag( CMAKE_MODULE_LINKER_FLAGS "-Wl,--hash-style=both" )
endif()

# Python related settings
set( ATLAS_PYTHON_CHECKER ""
   CACHE STRING "Python checker command to run during Python module compilation" )

if( NOT "${ATLAS_PYTHON_CHECKER}" STREQUAL "" )
   list( GET ATLAS_PYTHON_CHECKER 0 _executable )
   get_filename_component( _cmd "${_executable}" PROGRAM )
   if( EXISTS "${_cmd}" )
      set( ATLAS_PYTHON_CHECKER_FOUND TRUE CACHE INTERNAL BOOL )
      message( STATUS "Enabling Python code checker: ${_cmd}" )
   else()
      set( ATLAS_PYTHON_CHECKER_FOUND FALSE CACHE INTERNAL BOOL )
      message( WARNING "Could not locate Python code checker: ${ATLAS_PYTHON_CHECKER}" )
   endif()
   unset( _executable )
   unset( _cmd )
endif()

# Make CUDA generate debug symbols for the device code as well in a debug
# build.
_add_flag( CMAKE_CUDA_FLAGS_DEBUG "-G" )

# Clean up:
unset( _add_flag )
