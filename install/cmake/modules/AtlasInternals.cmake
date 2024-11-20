# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Functions used internally by functions declared in AtlasFunctions.
# Which are for general use inside the package descriptions.
#

# Include the required module(s):
include( CMakeParseArguments )

# This function can be used to determine whether we are currently in a named
# package. Which is needed in certain situations.
#
# Usage: atlas_is_package( value )
#
function( atlas_is_package var )

   # If the ATLAS_PACKAGE variable is set, then we're in a package.
   # Otherwise we're not.
   if( ATLAS_PACKAGE )
      set( ${var} TRUE PARENT_SCOPE )
   else()
      set( ${var} FALSE PARENT_SCOPE )
   endif()

endfunction( atlas_is_package )

# This function can get the name of the current package to a function that
# needs it.
#
# Usage: atlas_get_package_name( name )
#
function( atlas_get_package_name name )

   # Get the package name from the ${ATLAS_PACKAGE} variable, which should
   # be set by the atlas_subdir(...) function.
   if( ATLAS_PACKAGE )
      set( ${name} ${ATLAS_PACKAGE} PARENT_SCOPE )
   else()
      message( SEND_ERROR "Package name unknown. Please use "
         "atlas_subdir(...) to set it!" )
   endif()

endfunction( atlas_get_package_name )

# This function can return the full path of the current package in the
# source directory.
#
# Usage: atlas_get_package_dir( pkgDir )
#
function( atlas_get_package_dir dir )

   # Just remove the prefix of the source directory of the current
   # package's source directory, and that's it...
   string( REPLACE "${CMAKE_BINARY_DIR}/" "" _packageDir
      ${CMAKE_CURRENT_BINARY_DIR} )

   # Return the string:
   set( ${dir} ${_packageDir} PARENT_SCOPE )

endfunction( atlas_get_package_dir )

# This macro is used in the functions taking care of compiling code, to
# set some common compilation options for the packages.
#
# Usage: atlas_set_compiler_flags()
#
macro( atlas_set_compiler_flags )

   # Get the package's name:
   atlas_get_package_name( _pkgName )

   # Add package information to the build commands:
   add_definitions( -DATLAS_PACKAGE_NAME=\"${_pkgName}\" )

   # Clean up:
   unset( _pkgName )

endmacro( atlas_set_compiler_flags )

# This function is used by the code to get the "compiler portion"
# of the platform name. E.g. for GCC 4.9.2, return "gcc49". In case
# the compiler and version are not understood, the functions returns
# a false value in its second argument.
#
# Usage: atlas_compiler_id( _cmp _isValid )
#
function( atlas_compiler_id compiler isValid )

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

endfunction( atlas_compiler_id )

# This function is used to get a compact OS designation for the platform
# name. Like "slc6", "mac1010", "ubuntu1604", "centos7" or "el9".
#
# Usage: atlas_os_id( _os _isValid )
#
function( atlas_os_id os isValid )

   # Return cached result if possible:
   if ( ATLAS_OS_ID )
      set( "${os}" ${ATLAS_OS_ID} PARENT_SCOPE )
      set( "${isValid}" TRUE PARENT_SCOPE )
      return()
   endif()

   # Reset the result variable as a start.
   set( _name )

   if( APPLE )
      # Set a default version in case the following should not work:
      set( _name "mac1010" )
      # Get the MacOS X version number from the command line:
      execute_process( COMMAND sw_vers -productVersion
         TIMEOUT 30
         OUTPUT_VARIABLE _atlasMacVers )
      # Parse the variable, which should be in the form "X.Y.Z", or
      # possibly just "X.Y":
      if( _macVers MATCHES "^([0-9]+).([0-9]+).*" )
         set( _name "mac${CMAKE_MATCH_1}${CMAKE_MATCH_2}" )
      else()
         message( WARNING "MacOS version (${_atlasMacVers}) not parseable" )
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
            OUTPUT_VARIABLE _atlasLinuxId )
         string( REPLACE "Distributor ID:" "" _atlasLinuxId "${_atlasLinuxId}" )
         string( STRIP "${_atlasLinuxId}" _atlasLinuxId )
         execute_process( COMMAND "${LSB_RELEASE_EXECUTABLE}" "-r"
            TIMEOUT 30
            OUTPUT_VARIABLE _atlasLinuxVers )
         string( REPLACE "Release:" "" _atlasLinuxVers "${_atlasLinuxVers}" )
         string( STRIP "${_atlasLinuxVers}" _atlasLinuxVers )
      elseif( NOT "${_osReleaseContent}" STREQUAL "" )
         # If /etc/os-release is available, use that.
         foreach( _osReleaseLine ${_osReleaseContent} )
            if( "${_osReleaseLine}" MATCHES "^ID=\"?([^\"]*)\"?" )
               set( _atlasLinuxId "${CMAKE_MATCH_1}" )
               continue()
            elseif( "${_osReleaseLine}" MATCHES "^VERSION_ID=\"?([^\"]*)\"?" )
               set( _atlasLinuxVers "${CMAKE_MATCH_1}" )
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
      if( _atlasLinuxId MATCHES "Scientific" )
         set( _linuxShort "slc" )
      elseif( ( _atlasLinuxId MATCHES "Ubuntu" ) OR
              ( _atlasLinuxId MATCHES "ubuntu" ) )
         set( _linuxShort "ubuntu" )
      elseif( ( _atlasLinuxId MATCHES "CentOS" ) OR
              ( _atlasLinuxId MATCHES "centos" ) OR
              ( _atlasLinuxId MATCHES "RedHat" ) OR
              ( _atlasLinuxId MATCHES "rhel" ) OR
              ( _atlasLinuxId MATCHES "almalinux" ) OR
              ( _atlasLinuxId MATCHES "rocky" ) )
         # Up until version 7.X, these are called "centos" across the board.
         # Starting from version 8, they are called "el" (Enterprise Linux).
         if( _atlasLinuxVers VERSION_LESS "8" )
            set( _linuxShort "centos" )
         else()
            set( _linuxShort "el" )
         endif()
      elseif( ( _atlasLinuxId MATCHES "Debian" ) OR
              ( _atlasLinuxId MATCHES "debian" ) )
         set( _linuxShort "debian" )
      elseif( ( _atlasLinuxId MATCHES "SUSE" ) OR
              ( _atlasLinuxId MATCHES "suse" ) )
         set( _linuxShort "suse" )
      else()
         message( WARNING
            "ATLAS Linux flavour (${_atlasLinuxId}) not recognised" )
         set( _linuxShort "linux" )
      endif()
      # Combine the flavour and version.
      if( _atlasLinuxVers MATCHES "([0-9]+)\\.([0-9]+)" )
         if( ( "${_linuxShort}" STREQUAL "ubuntu" ) OR
               ( "${_linuxShort}" STREQUAL "suse" ) )
           # For Ubuntu and SUSE include the minor version number as well.
           set( _name "${_linuxShort}${CMAKE_MATCH_1}${CMAKE_MATCH_2}" )
         else()
            # For other Linux flavours use only the major version number.
            set( _name "${_linuxShort}${CMAKE_MATCH_1}" )
         endif()
      elseif(_atlasLinuxVers MATCHES "([0-9]+)" )
         set( _name "${_linuxShort}${CMAKE_MATCH_1}" )
      else()
         message( WARNING
            "ATLAS Linux version (${_atlasLinuxVers}) not parseable" )
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
   set( ATLAS_OS_ID "${_name}" CACHE INTERNAL "Compact platform name" )
   set( "${os}" ${_name} PARENT_SCOPE )
   set( "${isValid}" TRUE PARENT_SCOPE )

endfunction( atlas_os_id )

# This function is used internally to construct a platform name for a
# project. Something like: "x86_64-slc6-gcc48-opt".
#
# Usage: atlas_platform_id( _platform )
#
function( atlas_platform_id platform )

  # Get the OS's name:
  atlas_os_id( _os _valid )
  if( NOT _valid )
    set( ${platform} "generic" PARENT_SCOPE )
    return()
  endif()

  # Get the compiler name:
  atlas_compiler_id( _cmp _valid )
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

endfunction( atlas_platform_id )

# Function used internally to describe to IDEs how they should group source
# files of libraries and executables in their interface.
#
# Usage: atlas_group_source_files( ${_sources} )
#
function( atlas_group_source_files )

   # Collect all the passed file names:
   cmake_parse_arguments( ARG "" "" "" ${ARGN} )

   # Loop over all of them:
   foreach( f ${ARG_UNPARSED_ARGUMENTS} )
      # Ignore absolute path names. Like dictionary source files. Those should
      # just show up outside of any group.
      if( NOT IS_ABSOLUTE "${f}" )
         # Get the file's path:
         get_filename_component( _path "${f}" PATH )
         # Replace the forward slashes with double backward slashes:
         string( REPLACE "/" "\\\\" _group "${_path}" )
         # Put the file into the right group:
         source_group( "${_group}" FILES "${f}" )
      endif()
   endforeach()

endfunction( atlas_group_source_files )

# Function returning the number of cores that can be used in a Makefile
# based build for the "-jX" parameter.
#
# Usage: atlas_cpu_cores( nCores )
#
function( atlas_cpu_cores nCores )

   # Check if we need to re-evaluate the value:
   if( NOT DEFINED _processorCount )

      # Unknown:
      set( _processorCount 1 )

      # Linux:
      set( cpuinfo_file "/proc/cpuinfo" )
      if( EXISTS "${cpuinfo_file}" )
         file( STRINGS "${cpuinfo_file}" procs REGEX "^processor.: [0-9]+$" )
         list( LENGTH procs _processorCount )
      endif()

      # Mac:
      if( APPLE )
         find_program( cmd_sysctl "sysctl" )
         if( cmd_sysctl )
            execute_process( COMMAND ${cmd_sysctl} -n hw.ncpu
               OUTPUT_VARIABLE _info )
            string( STRIP ${_info} _processorCount )
         endif()
         mark_as_advanced( cmd_sysctl )
      endif()

      # Windows:
      if( WIN32 )
         set( _processorCount "$ENV{NUMBER_OF_PROCESSORS}" )
      endif()
   endif()

   # Set the value for the caller:
   set( ${nCores} ${_processorCount} PARENT_SCOPE )

endfunction( atlas_cpu_cores )

# This macro is used in AtlasProjectConfig.cmake to set up the project's
# use of the atlas_ctest.sh wrapper macro. Which takes care of writing
# package specific log files to the build area if CTEST_USE_LAUNCHERS is
# enabled.
#
# Usage: atlas_ctest_setup()
#
macro( atlas_ctest_setup )

   # Only run this code once:
   get_property( _ctestConfigured GLOBAL PROPERTY ATLAS_CTEST_CONFIGURED SET )
   set_property( GLOBAL PROPERTY ATLAS_CTEST_CONFIGURED TRUE )
   if( NOT _ctestConfigured )

      # Decide where to take bash from:
      if( APPLE )
         # atlas_project(...) should take care of putting it here:
         atlas_platform_id( _platform )
         set( BASH_EXECUTABLE "${CMAKE_BINARY_DIR}/${_platform}/bin/bash" )
         unset( _platform )
      else()
         # Just take it from its default location:
         find_program( BASH_EXECUTABLE bash )
      endif()

      # Only do anything fancy if we are in CTEST_USE_LAUNCHERS mode:
      if( CTEST_USE_LAUNCHERS )
         # Find the atlas_ctest.sh.in script skeleton:
         find_file( _atlasCTestSkeleton NAMES atlas_ctest.sh.in
            PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
            DOC "Skeleton file used for build log file writing" )
         mark_as_advanced( _atlasCTestSkeleton )
         # Check whether we found it.
         if( NOT _atlasCTestSkeleton )
            message( WARNING
               "Couldn't find atlas_ctest.sh.in. Not saving build logs!" )
            # Still include CTest, even in this case.
            include( CTest )
         else()
            # Configure the script:
            configure_file( "${_atlasCTestSkeleton}"
               "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_ctest.sh"
               @ONLY )
            set( _atlasCTest
               "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_ctest.sh" )
            set( CMAKE_CTEST_COMMAND ${_atlasCTest} )
            message( STATUS "Saving build logs using: ${_atlasCTest}" )
            # The name of the log directory:
            set( _logDir ${CMAKE_BINARY_DIR}/BuildLogs )
            # Create the directory that we want to write logs to:
            file( MAKE_DIRECTORY ${_logDir} )
            # Let CTest make the basic setup:
            include( CTest )
            # And now customise it for ourselves:
            foreach( _buildop COMPILE LINK CUSTOM )
               get_property( _origLauncher GLOBAL PROPERTY
                  RULE_LAUNCH_${_buildop} )
               string( REGEX REPLACE "--$" "" _launcher ${_origLauncher} )
               set( _newLauncher
                  "${_launcher} --log-dir ${_logDir} --binary-dir" )
               set( _newLauncher "${_newLauncher} ${CMAKE_BINARY_DIR} --" )
               set_property( GLOBAL PROPERTY RULE_LAUNCH_${_buildop}
                  ${_newLauncher} )
            endforeach()
            # Now clean up:
            unset( _atlasCTest )
            unset( _logDir )
            unset( _origLauncher )
            unset( _launcher )
            unset( _newLauncher )
         endif()
      else()
         # Otherwise just include CTest, and be done with it:
         include( CTest )
      endif()
   endif()

   # Some additional cleanup:
   unset( _ctestConfigured )

endmacro( atlas_ctest_setup )

# This macro should be used by projects to set them up for packaging
# using CPack. Projects depending on other ATLAS projects get this
# set up automatically, but base projects (like AtlasExternals) need
# to call it explicitly.
#
# Usage: atlas_cpack_setup()
#
macro( atlas_cpack_setup )

   # Decide where to take bash from:
   if( APPLE )
      # atlas_project(...) should take care of putting it here:
      set( BASH_EXECUTABLE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bash" )
   else()
      # Just take it from its default location:
      find_program( BASH_EXECUTABLE bash )
   endif()

   # Set some variables, constructed from variables set originally for CMake
   # itself:
   if( CMAKE_PROJECT_NAME AND CMAKE_PROJECT_VERSION )
      set( CPACK_PACKAGE_DESCRIPTION
         "${CMAKE_PROJECT_NAME} - ${CMAKE_PROJECT_VERSION}" )
      set( CPACK_PACKAGE_DESCRIPTION_SUMMARY
         "${CMAKE_PROJECT_NAME} - ${CMAKE_PROJECT_VERSION}" )
   elseif( NOT CMAKE_PROJECT_NAME )
      set( CMAKE_PROJECT_NAME "AtlasDummyProject" )
   endif()
   set( CPACK_PACKAGE_VENDOR "ATLAS Collaboration" )
   if( CMAKE_PROJECT_VERSION )
      set( CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION} )
   else()
      set( CPACK_PACKAGE_VERSION "0.0.1" )
   endif()

   # Contact information for the release:
   set( CPACK_PACKAGE_CONTACT "atlas-sw-core@cern.ch" )

   # Create a readme file for the release:
   find_file( _readme NAMES README.txt.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
      DOC "Readme skeleton for the project" )
   mark_as_advanced( _readme )
   configure_file( "${_readme}"
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/README.txt" @ONLY )
   unset( _readme )
   install( FILES "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/README.txt"
      DESTINATION . )
   set( CPACK_PACKAGE_DESCRIPTION_FILE
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/README.txt" )
   set( CPACK_RESOURCE_FILE_README
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/README.txt" )

   # Create a license file for the release:
   find_file( _license NAMES LICENSE.txt
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
      DOC "License file used for the installed release" )
   mark_as_advanced( _license )
   set( CPACK_RESOURCE_FILE_LICENSE ${_license} )
   install( FILES ${_license} DESTINATION . )
   unset( _license )

   # The installed release is supposed to be fully relocatable:
   set( CPACK_PACKAGE_RELOCATABLE TRUE )
   # This is the current convention which the environment setup scripts assume.
   # When changing the convention, one must change it in sync in:
   #  - NICOS;
   #  - The environment setup scripts;
   #  - cpack_install.sh.in;
   #  - Here.
   set( CPACK_PACKAGE_INSTALL_DIRECTORY
      "${CMAKE_PROJECT_NAME}/${CPACK_PACKAGE_VERSION}/InstallArea/${ATLAS_PLATFORM}" )
   set( CPACK_INSTALL_PREFIX "usr/${CPACK_PACKAGE_INSTALL_DIRECTORY}" )
   # This is chosen to be in sync with how CPack chooses a name for the source
   # package archive:
   set( CPACK_PACKAGE_FILE_NAME
      "${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${ATLAS_PLATFORM}" )
   # RPM specific settings:
   set( CPACK_RPM_PACKAGE_NAME
      "${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION}_${ATLAS_PLATFORM}" )
   set( CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}" )
   set( CPACK_RPM_PACKAGE_PROVIDES "/bin/sh" )
   set( CPACK_RPM_PACKAGE_ARCHITECTURE "noarch" )
   set( CPACK_RPM_PACKAGE_GROUP "ATLAS Software" )
   set( CPACK_RPM_PACKAGE_LICENSE "Apache License Version 2.0" )

   # Set the ATLAS project package dependencies of this RPM:
   set( CPACK_RPM_PACKAGE_AUTOREQ " no" )
   set( CPACK_RPM_PACKAGE_AUTOREQPROV " no" )
   set( CPACK_RPM_PACKAGE_REQUIRES )
   set( _bases ${ATLAS_BASE_PROJECTS} )
   while( _bases )
      list( GET _bases 0 _base_project )
      list( GET _bases 1 _base_version )
      list( REMOVE_AT _bases 0 1 )
      if( NOT "${CPACK_RPM_PACKAGE_REQUIRES}" STREQUAL "" )
         set( CPACK_RPM_PACKAGE_REQUIRES
            "${CPACK_RPM_PACKAGE_REQUIRES}, " )
      endif()
      set( _req "${_base_project}_${_base_version}_${ATLAS_PLATFORM}" )
      set( CPACK_RPM_PACKAGE_REQUIRES
         "${CPACK_RPM_PACKAGE_REQUIRES}${_req}" )
   endwhile()
   unset( _bases )
   unset( _base_project )
   unset( _base_version )
   unset( _req )

   # Add the external RPM dependencies:
   get_property( _extRpms GLOBAL PROPERTY ATLAS_EXTERNAL_RPMS )
   if( _extRpms )
      list( REMOVE_DUPLICATES _extRpms )
      foreach( _rpm ${_extRpms} )
         if( NOT "${CPACK_RPM_PACKAGE_REQUIRES}" STREQUAL "" )
            set( CPACK_RPM_PACKAGE_REQUIRES
               "${CPACK_RPM_PACKAGE_REQUIRES}, " )
         endif()
         set( CPACK_RPM_PACKAGE_REQUIRES
            "${CPACK_RPM_PACKAGE_REQUIRES}${_rpm}" )
      endforeach()
   endif()
   unset( _extRpms )

   # Disable binary stripping during the RPM building:
   set( CPACK_RPM_SPEC_MORE_DEFINE "
%global __os_install_post %{nil}
%define _unpackaged_files_terminate_build 0
%define _binaries_in_noarch_packages_terminate_build 0
%define _source_payload w2.xzdio
%define _binary_payload w2.xzdio
%undefine __brp_mangle_shebangs" )

   # Use a custom CPack install script:
   find_file( _installScript NAMES cpack_install.cmake
      PATHS ${CMAKE_MODULE_PATH} PATH_SUFFIXES scripts
      DOC "CPack installation script" )
   mark_as_advanced( _installScript )
   set( CPACK_INSTALL_SCRIPT ${_installScript} )
   unset( _installScript )

   # Installing the release for CPack only works correctly if the user
   # specified
   # "/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${ATLAS_PLATFORM}"
   # as CMAKE_INSTALL_PREFIX on the command line. So print a warning if it is
   # not the case. We still don't hardcode this value into the configuration,
   # as there can be good reasons for using AtlasCMake without building a
   # CPack package, with any given value for CMAKE_INSTALL_PREFIX.
   if( NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL
      "/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${ATLAS_PLATFORM}" )
      message( WARNING "CPack packaging will only work correctly with "
         "CMAKE_INSTALL_PREFIX=/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${ATLAS_PLATFORM}" )
   endif()

   # Use a custom script for installing the release for CPack. Which hides
   # build errors from CPack.
   set( ATLAS_USE_CUSTOM_CPACK_INSTALL_SCRIPT TRUE CACHE BOOL
      "Use a custom script for installation, hiding all installation errors" )
   if( ATLAS_USE_CUSTOM_CPACK_INSTALL_SCRIPT )
      set( CPACK_INSTALL_CMAKE_PROJECTS "" )
      find_file( ATLAS_CPACK_INSTALL_SCRIPT NAMES "cpack_install.sh.in"
         PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
         DOC "CPack installation script skeleton" )
      mark_as_advanced( ATLAS_CPACK_INSTALL_SCRIPT )
      configure_file( "${ATLAS_CPACK_INSTALL_SCRIPT}"
         "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cpack_install.sh"
         @ONLY )
      set( CPACK_INSTALL_COMMANDS
         "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cpack_install.sh" )
   endif()

   # Select the generator(s):
   if( APPLE )
      set( CPACK_GENERATOR "TGZ" )
      set( CPACK_SOURCE_GENERATOR "TGZ" )
   else()
      set( CPACK_GENERATOR "RPM" )
      set( CPACK_SOURCE_GENERATOR "RPM" )
      set( CPACK_PACKAGE_DEFAULT_LOCATION "/usr" )
   endif()

   # Set up the generator customisation file:
   find_file( _cpackOptions NAMES CPackOptions.cmake.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
      DOC "CPack options file skeleton" )
   mark_as_advanced( _cpackOptions )
   configure_file( "${_cpackOptions}"
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CPackOptions.cmake"
      @ONLY )
   set( CPACK_PROJECT_CONFIG_FILE
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CPackOptions.cmake" )
   unset( _cpackOptions )

   # Generate the debug RPM generator file, and its dependencies, if needed:
   if( ( "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" ) AND
       ATLAS_USE_CUSTOM_CPACK_INSTALL_SCRIPT )
      find_file( _dbgRpmConfig NAMES CPackDbgRPMConfig.cmake.in
         PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
         DOC "Opt-Debug RPM file configuration skeleton" )
      mark_as_advanced( _dbgRpmConfig )
      configure_file( "${_dbgRpmConfig}"
         "${CMAKE_BINARY_DIR}/CPackDbgRPMConfig.cmake" @ONLY )
      find_file( _dbgRpmInstall NAMES cpack_dbg_install.sh.in
         PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
         DOC "Opt-Debug RPM installation script skeleton" )
      mark_as_advanced( _dbgRpmInstall )
      configure_file( "${_dbgRpmInstall}"
         "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cpack_dbg_install.sh"
         @ONLY )
   endif()

   # Include CPack itself:
   include( CPack )

endmacro( atlas_cpack_setup )

# Master macro for finding "simple externals".
#
# Since many of the modules in this package look extremely similar, they are now
# implemented using this macro. To make their maintenance and development a bit
# easier.
#
# The produced code will rely on the <NAME>_ATROOT variable to find the
# requested external, and will produce the following possible variables:
#   - <NAME>_INCLUDE_DIRS
#   - <NAME>_LIBRARIES
#   - <NAME>_LIBRARY_DIRS
#   - <NAME>_<component>_FOUND
#   - <NAME>_<component>_LIBRARY
#   - <NAME>_FOUND_COMPONENTS
#
# Remember that this function by itself is not enough to implement a find
# module. At the very least one has to use find_package_handle_standard_args
# after this macro call to make the module work as expected.
#
# Note that there is a slight, but important difference between the
# DEFAULT_COMPONENTS and COMPULSORY_COMPONENTS options. The first one lists
# components that will be looked up if the user didn't ask for any components
# at all, the second one lists components that will be added to the list of
# components regardless whether the user asked for them or not.
#
# Usage: atlas_external_module( NAME name
#                               [INCLUDE_SUFFIXES include1 include2...]
#                               [INCLUDE_NAMES header1 header2...]
#                               [DEFAULT_COMPONENTS component1 component2...]
#                               [COMPULSORY_COMPONENTS component3 component4...]
#                               [LIBRARY_SUFFIXES libDir1 libDir2...]
#                               [LIBRARY_PREFIX prefix_]
#                               [LIBRARY_POSTFIX _postfix]
#                               [EXTRA_OPTIONS cmakeOpt1...]
#                               [SEARCH_PATHS path1 path2...] )
#
macro( atlas_external_module )

   # Parse the arguments.
   set( _singleArgParams NAME LIBRARY_PREFIX LIBRARY_POSTFIX )
   set( _multiArgParams INCLUDE_SUFFIXES INCLUDE_NAMES DEFAULT_COMPONENTS
      COMPULSORY_COMPONENTS LIBRARY_SUFFIXES EXTRA_OPTIONS SEARCH_PATHS )
   cmake_parse_arguments( ARG "" "${_singleArgParams}" "${_multiArgParams}"
      ${ARGN} )

   # Create an uppercase version of the name:
   string( TOUPPER ${ARG_NAME} nameUpper )

   # If the external was already found, set the module to silent mode.
   if( ${ARG_NAME}_FOUND OR ${nameUpper}_FOUND )
      set( ${ARG_NAME}_FIND_QUIETLY TRUE )
   endif()

   # Find the include directory of the external:
   if( ARG_INCLUDE_NAMES )
      find_path( ${nameUpper}_INCLUDE_DIR NAMES ${ARG_INCLUDE_NAMES}
         PATH_SUFFIXES ${ARG_INCLUDE_SUFFIXES}
         PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_ATROOT}
         NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
         ${ARG_EXTRA_OPTIONS} )
      set( ${nameUpper}_INCLUDE_DIRS ${${nameUpper}_INCLUDE_DIR} )
   endif()

   # Set up the components:
   if( ARG_DEFAULT_COMPONENTS AND NOT ${ARG_NAME}_FIND_COMPONENTS )
      set( ${ARG_NAME}_FIND_COMPONENTS ${ARG_DEFAULT_COMPONENTS} )
   endif()
   if( ARG_COMPULSORY_COMPONENTS )
      list( APPEND ${ARG_NAME}_FIND_COMPONENTS ${ARG_COMPULSORY_COMPONENTS} )
      list( REMOVE_DUPLICATES ${ARG_NAME}_FIND_COMPONENTS )
   endif()

   # Find the library/libraries:
   if( ${ARG_NAME}_FIND_COMPONENTS )
      # Clear out the library variables:
      set( ${nameUpper}_LIBRARIES )
      set( ${nameUpper}_LIBRARY_DIRS )
      # Find the requested component(s):
      foreach( component ${${ARG_NAME}_FIND_COMPONENTS} )
         # Search for the requested library:
         find_library( ${nameUpper}_${component}_LIBRARY
            NAMES ${ARG_LIBRARY_PREFIX}${component}${ARG_LIBRARY_POSTFIX}
            PATH_SUFFIXES ${ARG_LIBRARY_SUFFIXES}
            PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_ATROOT}
            NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH
            ${ARG_EXTRA_OPTIONS} )
         # Deal with the search result:
         if( ${nameUpper}_${component}_LIBRARY AND
               EXISTS "${${nameUpper}_${component}_LIBRARY}" )
            set( ${nameUpper}_${component}_FOUND TRUE )
            list( APPEND ${nameUpper}_FOUND_COMPONENTS ${component} )
            list( APPEND ${nameUpper}_LIBRARIES
               ${${nameUpper}_${component}_LIBRARY} )
            get_filename_component( libdir ${${nameUpper}_${component}_LIBRARY}
               PATH )
            list( APPEND ${nameUpper}_LIBRARY_DIRS ${libdir} )
         else()
            set( ${nameUpper}_${component}_FOUND FALSE )
         endif()
         # Mark the component location as an advanced property:
         mark_as_advanced( ${nameUpper}_${component}_LIBRARY
            ${nameUpper}_${component}_FOUND )
      endforeach()
      # Remove the duplicate library directory entries:
      if( ${nameUpper}_LIBRARY_DIRS )
         list( REMOVE_DUPLICATES ${nameUpper}_LIBRARY_DIRS )
      endif()
   endif()

   # Clean up.
   unset( nameUpper )

endmacro( atlas_external_module )

# Macro used privately by atlas_merge_project_files to cut down on the number
# of code lines in the file.
function( _merge_files targetName propPrefix fileName dest mergeCommand )

   # Check if the property is set:
   get_property( _propSet GLOBAL PROPERTY ${propPrefix}_FILES SET )

   # If yes, set up the merge for the files defined in it:
   if( _propSet )

      # Get the names of the files to be merged:
      get_property( _files GLOBAL PROPERTY ${propPrefix}_FILES )
      # Get the targets creating these files:
      get_property( _targets GLOBAL PROPERTY ${propPrefix}_TARGETS )

      # Generate a text file with the names of the files to be merged:
      set( _listFileName
         "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${targetName}Files.txt" )
      file( REMOVE ${_listFileName} )
      foreach( _file ${_files} )
         file( APPEND ${_listFileName} "${_file}\n" )
      endforeach()

      # Get the directory name of the output file:
      get_filename_component( _dir ${fileName} DIRECTORY )

      # Set up the target/command perfoming the merge:
      add_custom_command( OUTPUT ${fileName}
         COMMAND ${CMAKE_COMMAND} -E make_directory ${_dir}
         COMMAND ${mergeCommand} ${fileName} ${_listFileName}
         DEPENDS ${_files} ${_targets}
         COMMENT "Built ${fileName}" )
      add_custom_target( ${targetName} ALL
         DEPENDS ${fileName} )
      set_property( TARGET ${targetName} PROPERTY FOLDER ${CMAKE_PROJECT_NAME} )
      set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY
         ADDITIONAL_MAKE_CLEAN_FILES ${fileName} )

      # Now, this code will never be run if *any* of the partial files failed
      # to build. This is okay for a test/build area. But when installing the
      # project, we need to execute the merge no matter what.
      install( CODE "if( NOT EXISTS ${fileName} )
                        message( WARNING \"Creating partial ${fileName}\" )
                        execute_process( COMMAND ${mergeCommand} ${fileName}
                           ${_listFileName} )
                     endif()" )

      # Set up the installation of the merged file:
      install( FILES ${fileName} DESTINATION ${dest} OPTIONAL )

      # Clean up:
      unset( _files )
      unset( _targets )
      unset( _dir )
      unset( _listFileName )

   endif()
   unset( _propSet )

endfunction( _merge_files )

# This function takes care of merging all the file types of the projects that
# should be merged. Like the rootmap and clid files generated for the individual
# libraries.
#
# Usage: atlas_merge_project_files()
#
function( atlas_merge_project_files )

   # Find the merge scripts:
   find_program( _mergeFilesCmd mergeFiles.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _mergeFilesCmd )

   find_program( _mergeClidsCmd mergeClids.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _mergeClidsCmd )

   find_program( _mergeConfdb2Cmd mergeConfdb2.py
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _mergeConfdb2Cmd )

   # Merge the rootmap files:
   _merge_files( ${CMAKE_PROJECT_NAME}RootMapMerge ATLAS_ROOTMAP
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.rootmap
      ${CMAKE_INSTALL_LIBDIR}
      ${_mergeFilesCmd} )

   # Merge the CLID files:
   _merge_files( ${CMAKE_PROJECT_NAME}ClidMerge ATLAS_CLID
      ${CMAKE_SHARE_OUTPUT_DIRECTORY}/clid.db
      ${CMAKE_INSTALL_SHAREDIR}
      ${_mergeClidsCmd} )

   # Merge the components files:
   _merge_files( ${CMAKE_PROJECT_NAME}ComponentsMerge ATLAS_COMPONENTS
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.components
      ${CMAKE_INSTALL_LIBDIR}
      ${_mergeFilesCmd} )

   # Merge the confdb files:
   _merge_files( ${CMAKE_PROJECT_NAME}ConfdbMerge ATLAS_CONFDB
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.confdb
      ${CMAKE_INSTALL_LIBDIR}
      ${_mergeFilesCmd} )

   # Merge the confdb2 files:
   _merge_files( ${CMAKE_PROJECT_NAME}Confdb2Merge ATLAS_CONFDB2
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.confdb2
      ${CMAKE_INSTALL_LIBDIR}
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh;${_mergeConfdb2Cmd}" )

endfunction( atlas_merge_project_files )

# Delete the private macro:
unset( _merge_files )

# Function filtering which packages from the repository should be used in a
# particular build setup.
#
# Usage: atlas_is_package_selected( ${pkgDir} isSelected )
#
function( atlas_is_package_selected dirName returnName )

   # Check if the configuration file was already read in:
   get_property( _filtersSet GLOBAL PROPERTY ATLAS_PACKAGE_FILTERS SET )

   # If not yet, then do so now:
   if( NOT ${_filtersSet} )

      # The default location of the filter:
      set( ATLAS_PACKAGE_FILTER_FILE "${CMAKE_SOURCE_DIR}/package_filters.txt"
         CACHE FILEPATH "File describing the package filtering rules" )

      # Check if the file exists:
      if( EXISTS "${ATLAS_PACKAGE_FILTER_FILE}" )

         # Intermediate list(s) with the rules read:
         set( _filters )
         set( _paths )

         # Read in the file:
         file( STRINGS ${ATLAS_PACKAGE_FILTER_FILE} _fileContents )

         # Process all lines:
         foreach( _line ${_fileContents} )
            # Strip the line:
            string( STRIP "${_line}" _line )
            # Skip empty or comment lines:
            if( "${_line}" STREQUAL "" OR
                  "${_line}" MATCHES " *#.*" )
               continue()
            endif()
            # Now interpret this line:
            if( "${_line}" MATCHES "^([\+-]) *([^ ]*)" )
               list( APPEND _filters ${CMAKE_MATCH_1} ${CMAKE_MATCH_2} )
               list( APPEND _paths "${CMAKE_MATCH_1} ${CMAKE_MATCH_2}" )
            else()
               message( WARNING "Line \"${_line}\" not understood" )
            endif()
         endforeach()

         # Remember the selection rules:
         set( ATLAS_PACKAGE_FILTERS ${_filters} )
         set_property( GLOBAL PROPERTY ATLAS_PACKAGE_FILTERS
            ${ATLAS_PACKAGE_FILTERS} )
         set( ATLAS_UNUSED_PACKAGE_PATHS ${_paths} )
         set_property( GLOBAL PROPERTY ATLAS_UNUSED_PACKAGE_PATHS
            ${ATLAS_UNUSED_PACKAGE_PATHS} )

         # Print the rules that were read:
         message( STATUS "Package filtering rules read:" )
         while( _filters )
            list( GET _filters 0 _ruleCommand )
            list( GET _filters 1 _rulePath )
            list( REMOVE_AT _filters 0 1 )
            message( STATUS "  ${_ruleCommand} ${_rulePath}" )
            unset( _ruleCommand )
            unset( _rulePath )
         endwhile()

         # Clean up:
         unset( _filters )
         unset( _fileContents )

      else()
         # Set an empty ruleset:
         set( ATLAS_PACKAGE_FILTERS "<none>" )
         set_property( GLOBAL PROPERTY ATLAS_PACKAGE_FILTERS
            ${ATLAS_PACKAGE_FILTERS} )
         set( ATLAS_UNUSED_PACKAGE_PATHS "<none>" )
         set_property( GLOBAL PROPERTY ATLAS_UNUSED_PACKAGE_PATHS
            ${ATLAS_UNUSED_PACKAGE_PATHS} )
         message( STATUS "No package filtering rules read" )
      endif()

   else()
      # Get the filters from the global property:
      get_property( ATLAS_PACKAGE_FILTERS GLOBAL PROPERTY
         ATLAS_PACKAGE_FILTERS )
      get_property( ATLAS_UNUSED_PACKAGE_PATHS GLOBAL PROPERTY
         ATLAS_UNUSED_PACKAGE_PATHS )
   endif()

   # If no rules were found, mark the package selected:
   if( "${ATLAS_PACKAGE_FILTERS}" STREQUAL "<none>" )
      set( ${returnName} TRUE PARENT_SCOPE )
      return()
   endif()

   # Loop over the rules:
   set( _rulesCopy ${ATLAS_PACKAGE_FILTERS} )
   while( _rulesCopy )
      # Extract the rule from the list:
      list( GET _rulesCopy 0 _ruleCommand )
      list( GET _rulesCopy 1 _rulePath )
      list( REMOVE_AT _rulesCopy 0 1 )
      # Check if the rule applies to the directory passed to the function:
      if( "${dirName}" MATCHES "^${_rulePath}$" )
         # Now decide what to do:
         if( "${_ruleCommand}" STREQUAL "+" )
            set( ${returnName} TRUE PARENT_SCOPE )
         elseif( "${_ruleCommand}" STREQUAL "-" )
            set( ${returnName} FALSE PARENT_SCOPE )
         else()
            message( SEND_ERROR "Internal coding error detected" )
         endif()
         # Remove the path from the "unused list", if it's still there:
         list( FIND ATLAS_UNUSED_PACKAGE_PATHS "${_ruleCommand} ${_rulePath}"
            _pathIndex )
         if( NOT ${_pathIndex} EQUAL -1 )
            list( REMOVE_AT ATLAS_UNUSED_PACKAGE_PATHS ${_pathIndex} )
            set_property( GLOBAL PROPERTY ATLAS_UNUSED_PACKAGE_PATHS
               ${ATLAS_UNUSED_PACKAGE_PATHS} )
         endif()
         # Now return:
         return()
      endif()
   endwhile()

   # Clean up:
   unset( _rulesCopy )

   # If no rule was found for the package, then it is selected:
   set( ${returnName} TRUE PARENT_SCOPE )

endfunction( atlas_is_package_selected )

# Function used to print a "loud" warning after reading in the packages of a
# project, if any rules read from the project's package filter file were left
# unused. As that's very likely a hint that there's a configuration error in
# the selection file.
#
# Used by atlas_project(...), after it is done with reading in the configuration
# of the individual package.
#
# Usage: atlas_print_unused_package_selection_rules()
#
function( atlas_print_unused_package_selection_rules )

   # Check if the unused path list is set:
   get_property( _pathsSet GLOBAL PROPERTY ATLAS_UNUSED_PACKAGE_PATHS SET )

   # If it is not set, there's nothing more to do:
   if( NOT _pathsSet )
      return()
   endif()

   # Get the list of unused paths:
   get_property( ATLAS_UNUSED_PACKAGE_PATHS GLOBAL PROPERTY
      ATLAS_UNUSED_PACKAGE_PATHS )

   # If it is empty, or set to "<none>", then don't do anything:
   if( "${ATLAS_UNUSED_PACKAGE_PATHS}" STREQUAL "" OR
         "${ATLAS_UNUSED_PACKAGE_PATHS}" STREQUAL "<none>" )
      return()
   endif()

   # Format the unused paths in a visually pleasing way:
   set( _message )
   while( ATLAS_UNUSED_PACKAGE_PATHS )
      list( GET ATLAS_UNUSED_PACKAGE_PATHS 0 _path )
      list( REMOVE_AT ATLAS_UNUSED_PACKAGE_PATHS 0 )
      set( _message "${_message}   ${_path}\n" )
   endwhile()

   # If we got this far, apparently some package filtering rules were not used
   # during the configuration.
   message( WARNING
      "Ignored these rule(s) while selecting packages to build:\n"
      "${_message}"
      "Make sure that you didn't make a typo, and that there is no "
      "duplicate in:\n"
      "   ${ATLAS_PACKAGE_FILTER_FILE}\n" )

endfunction( atlas_print_unused_package_selection_rules )

# This function is used to generate a "ReleaseData" file for the project.
# One that captures all relevant information about a release, such that it
# would allow users to re-generate the release from scratch if necessary.
#
# The function allows the user to override the following automatically set cache
# variables:
#  - ATLAS_BUILD_DATE: Date string in the form of "YYYY-MM-DDTHHMM";
#  - ATLAS_NIGHTLY_NAME: The name of the git branch that the build is set up
#    from;
#  - ATLAS_NIGHTLY_RELEASE: The hash of the git repository that the build is set
#    up from;
#
# Usage: atlas_generate_releasedata()
#
function( atlas_generate_releasedata )

   # Set variables necessary for the ReleaseData generation:
   unset( ENV{TZ} ) # Work around CMake bug #18431
   string( TIMESTAMP _timeStamp "%Y-%m-%dT%H%M" )
   set( ATLAS_BUILD_DATE "${_timeStamp}"
      CACHE STRING "Timestamp of build" )
   unset( _timeStamp )

   # Check if timestamp is valid:
   string( REGEX MATCH
      "^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]T[0-9][0-9][0-9][0-9]$"
      _match ${ATLAS_BUILD_DATE} )
   if ( NOT _match )
      message( SEND_ERROR
         "Invalid ATLAS_BUILD_DATE \"${ATLAS_BUILD_DATE}\" "
         "(required: YYYY-MM-DDTHHMM)")
   endif()
   unset( _match )

   # Find the helper script generating the nightly name:
   find_file( _nightlyNameScript nightly_name.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _nightlyNameScript )
   # Try to figure out the git branch name:
   execute_process( COMMAND ${_nightlyNameScript}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      RESULT_VARIABLE _gitReturn
      OUTPUT_VARIABLE _gitBranch
      OUTPUT_STRIP_TRAILING_WHITESPACE )
   if( ( NOT "${_gitReturn}" EQUAL 0 ) OR ( "${_gitBranch}" STREQUAL "" ) )
      set( _gitBranch "Unknown" )
   endif()
   set( ATLAS_NIGHTLY_NAME "${_gitBranch}" CACHE STRING
      "The \"name\" of the nightly being built" )
   unset( _gitBranch )
   unset( _nightlyNameScript )

   # Try to figure out the git commit hash:
   execute_process( COMMAND git log -1 --format=%h
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      RESULT_VARIABLE _gitReturn
      OUTPUT_VARIABLE _gitHash
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET )
   if( NOT "${_gitReturn}" EQUAL 0 )
      set( _gitReturn "Unknown" )
   endif()
   set( ATLAS_NIGHTLY_RELEASE "${_gitHash}" CACHE STRING
      "The \"release version\" of the nightly being built" )
   unset( _gitReturn )
   unset( _gitHash )

   # And now generate/install the ReleaseData file:
   find_file( _releaseData ReleaseData.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   set( _targetFile ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/ReleaseData )
   configure_file( ${_releaseData} ${_targetFile} )
   mark_as_advanced( _releaseData )
   unset( _releaseData )
   install( FILES ${_targetFile} DESTINATION . )
   message( STATUS "Generated file: ${_targetFile}" )
   unset( _targetFile )

   # Do not preserve cache variables across cmake runs.
   unset( ATLAS_BUILD_DATE CACHE )
   unset( ATLAS_NIGHTLY_NAME CACHE )
   unset( ATLAS_NIGHTLY_RELEASE CACHE )

endfunction( atlas_generate_releasedata )

# Create a deep copy of an imported target, "in the global namespace".
#
# This function is used by the exported project configuration files to set up
# targets correctly for the build of a project on top of a base project.
#
# Usage: atlas_copy_target( BaseProject LibraryA )
#
function( atlas_copy_target projectName targetName )

   # Get the type of this target.
   get_property( _type TARGET ${projectName}::${targetName} PROPERTY TYPE )

   # Check if it is some kind of library.
   if( "${_type}" MATCHES "(.*)_LIBRARY" )

      # Create a deep copy of the library.
      add_library( ${targetName} ${CMAKE_MATCH_1} IMPORTED GLOBAL )

      # Loop over all known/relevant properties that are not set per build
      # type.
      foreach( _propName INTERFACE_LINK_LIBRARIES
         INTERFACE_SYSTEM_INCLUDE_DIRECTORIES INTERFACE_INCLUDE_DIRECTORIES
         INTERFACE_COMPILE_DEFINITIONS INTERFACE_COMPILE_OPTIONS )

         # Check if this property needs to be copied.
         get_property( _propSet
            TARGET ${projectName}::${targetName}
            PROPERTY ${_propName} SET )
         if( NOT _propSet )
            unset( _propSet )
            continue()
         endif()
         unset( _propSet )

         # Copy the property.
         get_property( _propVal
            TARGET ${projectName}::${targetName}
            PROPERTY ${_propName} )
         string( REPLACE "${projectName}::" "" _propVal "${_propVal}" )
         set_property( TARGET ${targetName} PROPERTY ${_propName} ${_propVal} )
         unset( _propVal )
      endforeach()

      # Check if any build type configuration options are set on the target.
      get_property( _confSet TARGET ${projectName}::${targetName}
         PROPERTY IMPORTED_CONFIGURATIONS SET )
      if( NOT _confSet )
         # If not, we're done.
         return()
      endif()
      unset( _confSet )

      # Copy all (relevant/known) "file/builttype-based" properties between
      # the two.
      get_property( _builds
         TARGET ${projectName}::${targetName}
         PROPERTY IMPORTED_CONFIGURATIONS )
      set_property( TARGET ${targetName} PROPERTY
         IMPORTED_CONFIGURATIONS ${_builds} )

      # Iterate over the available build configurations.
      foreach( _build ${_builds} )
         # Iterate over the possible properties.
         foreach( _propertyName IMPORTED_LOCATION_${_build}
            IMPORTED_SONAME_${_build} IMPORTED_NO_SONAME_${_build}
            IMPORTED_LINK_DEPENDENT_LIBRARIES_${_build}
            IMPORTED_LINK_INTERFACE_LANGUAGES_${_build}
            IMPORTED_COMMON_LANGUAGE_RUNTIME_${_build}
            IMPORTED_OBJECTS_${_build} )

            # Check if this property needs to be copied.
            get_property( _propSet
               TARGET ${projectName}::${targetName}
               PROPERTY ${_propertyName} SET )
            if( NOT _propSet )
               unset( _propSet )
               continue()
            endif()
            unset( _propSet )

            # Copy the property.
            get_property( _property TARGET ${projectName}::${targetName}
               PROPERTY ${_propertyName} )
            string( REPLACE "${projectName}::" "" _property "${_property}" )
            set_property( TARGET ${targetName}
               PROPERTY ${_propertyName} ${_property} )
            unset( _property )
         endforeach()
      endforeach()
      unset( _builds )

   # Check if it is an executable.
   elseif( "${_type}" STREQUAL "EXECUTABLE" )

      # Create a deep copy of the executable.
      add_executable( ${targetName} IMPORTED GLOBAL )

      # Copy the build configuration dependent properties for it.
      get_property( _builds
         TARGET ${projectName}::${targetName}
         PROPERTY IMPORTED_CONFIGURATIONS )
      set_property( TARGET ${targetName} PROPERTY
         IMPORTED_CONFIGURATIONS ${_builds} )

      # Iterate over the available build configurations.
      foreach( _build ${_builds} )
         # Iterate over the possible properties.
         foreach( _propertyName IMPORTED_LOCATION_${_build} )

            # Check if this property needs to be copied.
            get_property( _propSet
               TARGET ${projectName}::${targetName}
               PROPERTY ${_propertyName} SET )
            if( NOT _propSet )
               unset( _propSet )
               continue()
            endif()
            unset( _propSet )

            # Copy the property.
            get_property( _property TARGET ${projectName}::${targetName}
               PROPERTY ${_propertyName} )
            string( REPLACE "${projectName}::" "" _property "${_property}" )
            set_property( TARGET ${targetName}
               PROPERTY ${_propertyName} ${_property} )
            unset( _property )
         endforeach()
      endforeach()
      unset( _builds )
   else()
      message( WARNING "Unknown target type (${_type}) encountered" )
   endif()
   unset( _type )

endfunction( atlas_copy_target )
