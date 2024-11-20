# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# File implementing the code that gets called when a project imports
# LCG using something like:
#
#  find_package( LCG 75 )
#
# Hopefully this could be adopted by the SFT group in the official LCG
# builds, but for now it's only a demonstrator.
#
# To make CMake find this file, you have to call CMake either like:
#
#   CMAKE_PREFIX_PATH=../source/AtlasCMake cmake ../source
#
# , or like:
#
#   cmake -DLCG_DIR=../../AtlasCMake ../source
#
# The script takes the value of the environment variable (or CMake cache
# variable) LCG_RELEASES_BASE to find LCG releases. Or if it's not set, it
# looks up the releases from AFS.
#

# Require at least cmake-3.6 to use this code.
cmake_minimum_required( VERSION 3.6 )

# Make sure that all _ROOT variables *are* used when they are set.
if( POLICY CMP0074 )
   cmake_policy( SET CMP0074 NEW )
endif()

# This function is used by the code to get the "compiler portion"
# of the platform name. E.g. for GCC 4.9.2, return "gcc49". In case
# the compiler and version are not understood, the functions returns
# a false value in its second argument.
#
# The decision is made based on the C++ compiler.
#
# Usage: lcg_compiler_id( _cmp _isValid )
#
function( lcg_compiler_id compiler isValid )

   # Translate the compiler ID:
   set( _prefix )
   if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
      set( _prefix "gcc" )
   elseif( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
      set( _prefix "clang" )
   elseif( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel" )
      set( _prefix "icc" )
   else()
      set( ${compiler} "unknown" PARENT_SCOPE )
      set( ${isValid} FALSE PARENT_SCOPE )
      return()
   endif()

   # Translate the compiler version. Handle GCC >=7 and Clang >=4 in special
   # ways. Only taking the major version number for those compilers.
   set( _version )
   if( CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+).([0-9]+).*"
         AND ( NOT ( ( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) AND
                  ( "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "7" ) ) OR
               ( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) AND
                  ( "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "4" ) ) ) ) )
      set( _version "${CMAKE_MATCH_1}${CMAKE_MATCH_2}" )
   elseif( CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+).*" )
      set( _version "${CMAKE_MATCH_1}" )
   endif()

   # Set the return variables:
   set( ${compiler} "${_prefix}${_version}" PARENT_SCOPE )
   set( ${isValid} TRUE PARENT_SCOPE )

endfunction( lcg_compiler_id )

# This function is used to get a compact OS designation for the platform
# name. Like "slc6", "mac1010", "centos7" or "el9".
#
# Usage: lcg_os_id( _os _isValid )
#
function( lcg_os_id os isValid )

   # Return cached result if possible:
   if ( LCG_OS_ID )
      set( "${os}" "${LCG_OS_ID}" PARENT_SCOPE )
      set( "${isValid}" TRUE PARENT_SCOPE )
      return()
   endif()

   # Reset the result variable as a start:
   set( _name )

   if( APPLE )
      # Get the MacOS X version number from the command line:
      execute_process( COMMAND sw_vers -productVersion
         TIMEOUT 30
         OUTPUT_VARIABLE _lcgMacVers )
      # Parse the variable, which should be in the form "X.Y.Z", or
      # possibly just "X.Y":
      if( _lcgMacVers MATCHES "^([0-9]+).([0-9]+).*" )
         set( _name "mac${CMAKE_MATCH_1}${CMAKE_MATCH_2}" )
      else()
         message( WARNING "MacOS version (${_lcgMacVers}) not parseable" )
         set( "${os}" "unknown" PARENT_SCOPE )
         set( "${isValid}" FALSE PARENT_SCOPE )
         return()
      endif()
   elseif( UNIX )
      # Check if lsb_release is available.
      find_program( LSB_RELEASE_EXECUTABLE NAMES "lsb_release" )
      mark_as_advanced( LSB_RELEASE_EXECUTABLE )
      # Check if /etc/os-release is available, and read it if it is.
      set( _osReleaseContent )
      if( EXISTS "/etc/os-release" )
         file( STRINGS "/etc/os-release" _osReleaseContent )
      endif()
      # Get the linux release ID and version number.
      if( LSB_RELEASE_EXECUTABLE )
         # If lsb_release is available, use that.
         execute_process( COMMAND "${LSB_RELEASE_EXECUTABLE}" "-i"
            TIMEOUT 30
            OUTPUT_VARIABLE _lcgLinuxId )
         string( REPLACE "Distributor ID:" "" _lcgLinuxId "${_lcgLinuxId}" )
         string( STRIP "${_lcgLinuxId}" _lcgLinuxId )
         execute_process( COMMAND "${LSB_RELEASE_EXECUTABLE}" "-r"
            TIMEOUT 30
            OUTPUT_VARIABLE _lcgLinuxVers )
         string( REPLACE "Release:" "" _lcgLinuxVers "${_lcgLinuxVers}" )
         string( STRIP "${_lcgLinuxVers}" _lcgLinuxVers )
      elseif( NOT "${_osReleaseContent}" STREQUAL "" )
         # If /etc/os-release is available, use that.
         foreach( _osReleaseLine ${_osReleaseContent} )
            if( "${_osReleaseLine}" MATCHES "^ID=\"?([^\"]*)\"?" )
               set( _lcgLinuxId "${CMAKE_MATCH_1}" )
               continue()
            elseif( "${_osReleaseLine}" MATCHES "^VERSION_ID=\"?([^\"]*)\"?" )
               set( _lcgLinuxVers "${CMAKE_MATCH_1}" )
               continue()
            endif()
         endforeach()
      else()
         # If nothing is available, give up.
         set( "${os}" "unknown" PARENT_SCOPE )
         set( "${isValid}" FALSE PARENT_SCOPE )
         return()
      endif()
      # Translate it to a shorthand according to our own naming:
      set( _linuxShort )
      if( _lcgLinuxId MATCHES "Scientific" )
         set( _linuxShort "slc" )
      elseif( ( _lcgLinuxId MATCHES "Ubuntu" ) OR
              ( _lcgLinuxId MATCHES "ubuntu" ) )
         set( _linuxShort "ubuntu" )
      elseif( ( _lcgLinuxId MATCHES "CentOS" ) OR
              ( _lcgLinuxId MATCHES "centos" ) OR
              ( _lcgLinuxId MATCHES "RedHat" ) OR
              ( _lcgLinuxId MATCHES "rhel" ) OR
              ( _lcgLinuxId MATCHES "almalinux" ) OR
              ( _lcgLinuxId MATCHES "rocky" ) )
         # Up until version 7.X, these are called "centos" across the board.
         # Starting from version 8, they are called "el" (Enterprise Linux).
         if( _lcgLinuxVers VERSION_LESS "8" )
            set( _linuxShort "centos" )
         else()
            set( _linuxShort "el" )
         endif()
      else()
         message( WARNING "LCG Linux flavour (${_lcgLinuxId}) not recognised" )
         set( _linuxShort "linux" )
      endif()
      # Combine the flavour and version.
      if( _lcgLinuxVers MATCHES "([0-9]+)\\.([0-9]+)" )
         if( ( "${_linuxShort}" STREQUAL "ubuntu" ) OR
               ( "${_linuxShort}" STREQUAL "suse" ) )
           # For Ubuntu and SUSE include the minor version number as well.
           set( _name "${_linuxShort}${CMAKE_MATCH_1}${CMAKE_MATCH_2}" )
         else()
            # For other Linux flavours use only the major version number.
            set( _name "${_linuxShort}${CMAKE_MATCH_1}" )
         endif()
      elseif(_lcgLinuxVers MATCHES "([0-9]+)" )
         set( _name "${_linuxShort}${CMAKE_MATCH_1}" )
      else()
         message( WARNING "LCG Linux version (${_lcgLinuxVers}) not parseable" )
         set( "${os}" "unknown" PARENT_SCOPE )
         set( "${isValid}" FALSE PARENT_SCOPE )
         return()
      endif()
   else()
      set( "${os}" "unknown" PARENT_SCOPE )
      set( "${isValid}" FALSE PARENT_SCOPE )
      return()
   endif()

   # Set and cache the return values:
   set( LCG_OS_ID "${_name}" CACHE INTERNAL "Compact platform name" )
   set( "${os}" "${_name}" PARENT_SCOPE )
   set( "${isValid}" TRUE PARENT_SCOPE )

endfunction( lcg_os_id )

# This function is used internally to construct a platform name for a
# project. Something like: "x86_64-slc6-gcc48-opt".
#
# Usage: lcg_platform_id( _platform )
#
function( lcg_platform_id platform )

   # Get the OS's name:
   lcg_os_id( _os _valid )
   if( NOT _valid )
      set( ${platform} "generic" PARENT_SCOPE )
      return()
   endif()

   # Get the compiler name:
   lcg_compiler_id( _cmp _valid )
   if( NOT _valid )
      set( ${platform} "generic" PARENT_SCOPE )
      return()
   endif()

   # Construct the postfix of the platform name:
   if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
      set( _postfix "dbg" )
   else()
      set( _postfix "opt" )
   endif()

   # Set the platform return value:
   set( ${platform} "${CMAKE_SYSTEM_PROCESSOR}-${_os}-${_cmp}-${_postfix}"
      PARENT_SCOPE )

endfunction( lcg_platform_id )

# Function setting the home/root directories of all the LCG packages.
# The function takes the name of an LCG text file as argument. One that
# holds the description of a list of packages held by the release.
#
# Usage: lcg_setup_packages( ${fileName} ${LCG_RELEASE_DIR} )
#
function( lcg_setup_packages lcgFile lcgReleaseDir )

   # Tell the user what's happening:
   if( NOT LCG_FIND_QUIETLY )
      message( STATUS "Setting up LCG release using: ${lcgFile}" )
   endif()

   # Read in the contents of the file:
   file( STRINGS ${lcgFile} _fileContents LIMIT_INPUT 5000000
      REGEX ".*;.*;.*;.*;.*" )

   # Loop over each line of the configuration file:
   foreach( _line ${_fileContents} )

      # The component's name:
      list( GET _line 0 name )
      string( TOUPPER ${name} nameUpper )
      string( REPLACE "+" "P" nameUpper ${nameUpper} )
      # The component's identifier:
      list( GET _line 1 id )
      # The component's version:
      list( GET _line 2 version )
      string( STRIP ${version} version )
      # The component's base directory:
      list( GET _line 3 dir1 )
      string( STRIP ${dir1} dir2 )
      if( IS_ABSOLUTE ${dir2} )
         set( dir3 ${dir2} )
      else()
         string( REPLACE "./" "" dir3 ${dir2} )
      endif()

      # Set up the component, by setting up a <Foo>_LCGROOT cache variable
      # pointing at it. Which the Find<Foo>.cmake module will take into account
      # while looking for <Foo>.
      if( IS_ABSOLUTE ${dir3} )
         set( ${nameUpper}_LCGROOT ${dir3}
            CACHE PATH "Directory for ${name}-${version}" )
      else()
         set( ${nameUpper}_LCGROOT ${lcgReleaseDir}/${dir3}
            CACHE PATH "Directory for ${name}-${version}" )
      endif()
      set( ${nameUpper}_LCGVERSION ${version}
         CACHE STRING "Version of ${name}" )

   endforeach()

endfunction( lcg_setup_packages )

# Function setting up all the packages from the selected LCG release.
# The code is moved into a function, so that variables generated inside the
# function would not be visible to the file calling find_package(LCG).
#
# Usage: lcg_setup_release( ${LCG_RELEASE_DIR} )
#
function( lcg_setup_release lcgReleaseDir )

   # The components to set up:
   set( LCG_COMPONENTS "externals;generators"
      CACHE STRING "LCG components to set up" )

   # Start out with the assumption that LCG is found:
   set( LCG_FOUND TRUE CACHE BOOL
      "Flag showing whether LCG was found or not" )
   mark_as_advanced( LCG_FOUND )

   # Set up the packages provided by LCG using the files specified:
   foreach( _component ${LCG_COMPONENTS} )

      # Construct the file name to load:
      set( _file ${lcgReleaseDir}/LCG_${_component}_${LCG_PLATFORM}.txt )
      if( NOT EXISTS ${_file} )
         message( WARNING
            "LCG component \"${_component}\" not available for platform: "
            "${LCG_PLATFORM}" )
         set( LCG_FOUND FALSE CACHE BOOL
            "Flag showing whether LCG was found or not" FORCE )
         continue()
      endif()

      # Set up the packages described by this file:
      lcg_setup_packages( ${_file} ${lcgReleaseDir} )

      # Clean up:
      unset( _file )

   endforeach()

endfunction( lcg_setup_release )

# Only set up a release, if the requested release number was
# something other than 0.
if( NOT LCG_VERSION EQUAL 0 )

   # Get the platform ID:
   if( NOT "$ENV{LCG_PLATFORM}" STREQUAL "" )
      set( _lcg_platform $ENV{LCG_PLATFORM} )
   else()
      lcg_platform_id( _lcg_platform )
   endif()
   set( LCG_PLATFORM ${_lcg_platform}
      CACHE STRING "Platform name for the LCG release being used" )
   unset( _lcg_platform )

   # Tell the user what's happening:
   if( NOT LCG_FIND_QUIETLY )
      message( STATUS
         "Setting up LCG release \"${LCG_VERSION_STRING}\" for platform: "
         "${LCG_PLATFORM}" )
   endif()

   # Some sanity checks:
   if( LCG_FIND_COMPONENTS )
      message( WARNING "Components \"${LCG_FIND_COMPONENTS}\" requested, but "
         "finding LCG components is not supported" )
   endif()

   # Construct the path to pick up the release from:
   set( LCG_RELEASE_DIR ${LCG_RELEASE_BASE}/${LCG_VERSION_STRING}
      CACHE PATH "Directory holding the LCG release" )

   # Set up the release, if not done already.
   if( NOT LCG_FOUND )

      # "Basic" setup, setting all of the <FOO>_LCGROOT and <FOO>_LCGVERSION
      # variables.
      lcg_setup_release( ${LCG_RELEASE_DIR} )

      # Extend the CMAKE_PREFIX_PATH environment variable, to make the
      # "wrapped" find-modules work correctly.
      set( _cmakePaths )
      list( APPEND _cmakePaths ${BOOST_LCGROOT} ${ZLIB_LCGROOT}
         ${DOXYGEN_LCGROOT} ${QT_LCGROOT} ${QT5_LCGROOT} ${GRAPHVIZ_LCGROOT}
         ${COIN3D_LCGROOT} ${EXPAT_LCGROOT} ${RANGEV3_LCGROOT}
         ${LIBXML2_LCGROOT} ${PNG_LCGROOT} ${HDF5_LCGROOT}
         ${BLAS_LCGROOT} ${LAPACK_LCGROOT} ${LIBXSLT_LCGROOT}
         ${FREETYPE_LCGROOT} ${CURL_LCGROOT} ${TIFF_LCGROOT} ${PYTHON_LCGROOT}
         ${XERCESC_LCGROOT} ${FMT_LCGROOT} ${JSONMCPP_LCGROOT} ${EIGEN_LCGROOT}
         ${SQLITE_LCGROOT} ${CUDA_LCGROOT} ${PROTOBUF_LCGROOT} ${VC_LCGROOT}
         ${GTEST_LCGROOT} )
      string( REPLACE ";" ":" _path "${_cmakePaths}" )
      set( ENV{CMAKE_PREFIX_PATH} "$ENV{CMAKE_PREFIX_PATH}:${_path}" )
      unset( _path )
      unset( _cmakePaths )
   endif()

   # Extra settings, made every time that find_package(LCG...) is called.
   file( GLOB BOOST_INCLUDEDIR "${BOOST_LCGROOT}/include/*" )
   if( "${LCG_PLATFORM}" MATCHES "-dbg$" )
      set( Boost_USE_DEBUG_RUNTIME TRUE CACHE BOOL
         "Switch for using the debug libraries for Boost" )
   else()
      set( Boost_USE_DEBUG_RUNTIME FALSE CACHE BOOL
         "Switch for using the debug libraries for Boost" )
   endif()
   set( GSL_ROOT_DIR ${GSL_LCGROOT} )
   set( JAVA_HOME ${JAVA_LCGROOT} )
   set( TCMALLOC_LCGROOT ${GPERFTOOLS_LCGROOT} )

endif()

# Get the current directory:
get_filename_component( _thisdir "${CMAKE_CURRENT_LIST_FILE}" PATH )

# Add the module directory to CMake's module path:
list( INSERT CMAKE_MODULE_PATH 0 ${_thisdir}/modules )
list( REMOVE_DUPLICATES CMAKE_MODULE_PATH )

# Include the LCGFunctions module:
include( LCGFunctions )

# Set the C++ standard to use.
set( CMAKE_CXX_STANDARD_DEFAULT 17 )
if( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" ) AND
    ( "${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER_EQUAL "13" ) )
   # Use C++20 by default with GCC 13+.
   set( CMAKE_CXX_STANDARD_DEFAULT 20 )
endif()
set( CMAKE_CXX_STANDARD "${CMAKE_CXX_STANDARD_DEFAULT}" CACHE STRING
   "C++ standard used for the build" )
set( GAUDI_CXX_STANDARD "c++${CMAKE_CXX_STANDARD}"
   CACHE STRING "C++ standard used for the Gaudi build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow using GNU extensions" )

# Install this package with the current project:
install( DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules
   DESTINATION cmake
   USE_SOURCE_PERMISSIONS
   PATTERN ".svn" EXCLUDE
   PATTERN "*~" EXCLUDE )
install( FILES ${CMAKE_CURRENT_LIST_DIR}/LCGConfig.cmake
   ${CMAKE_CURRENT_LIST_DIR}/LCGConfig-version.cmake
   DESTINATION cmake )

# By default prefer not to use frameworks on macOS. See
# https://cmake.org/cmake/help/v3.0/command/find_file.html for details on
# the behaviour of this cache variable.
set( CMAKE_FIND_FRAMEWORK "LAST" CACHE STRING
   "Framework finding behaviour on macOS" )

# If CTEST_USE_LAUNCHERS is turned on, create a dummy log file already
# during configuration, which NICOS can pick up, and conclude that the
# package's "build" was successful.
if( CTEST_USE_LAUNCHERS )
   file( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/BuildLogs )
   file( WRITE ${CMAKE_BINARY_DIR}/BuildLogs/AtlasLCG.log
      "Dummy build log\n"
      "AtlasLCG: Package build succeeded" )
endif()

# Clean up:
unset( _thisdir )
