# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# This is the main file that needs to be included in order to get access to the
# ATLAS CMake functions.
#
# When building a release or a test area according to the normal instructions,
# it's not necessary to include it explicity. But in certain situations it may
# be necessary to include it with
#
#  include( AtlasFunctions )
#
# in the user code. Note that user code should only ever include this one file,
# and even this should only be included if absolutely necessary.
#

# Minimum CMake version required:
cmake_minimum_required( VERSION 3.6 )

# Handle the install_name settings on macOS according to the "new rules".
if( POLICY CMP0068 )
   cmake_policy( SET CMP0068 NEW )
endif()
# Make sure that all _ROOT variables *are* used when they are set.
if( POLICY CMP0074 )
   cmake_policy( SET CMP0074 NEW )
endif()

# CMake module(s).
include( CMakePackageConfigHelpers )
include( CMakeParseArguments )
include( ExternalProject )

# ATLAS module(s).
include( AtlasInternals )
include( AtlasLibraryFunctions )
include( AtlasDictionaryFunctions )
include( AtlasInstallFunctions )
include( AtlasTestFunctions )
include( AtlasCompilerSettings )
include( AtlasInstallDirs )

# Function setting up the compilation of a set of "ATLAS packages".
#
# This function should be used as part of a project's configuration to set
# up the build of a set of "packages". Possibly against packages coming from
# some base project.
#
# A standalone project (one that doesn't build against another one) can
# use it like:
#
#   cmake_minimum_required( VERSION 3.6 )
#   project( BaseProject VERSION 1.0.0 [LANGUAGES ...] )
#   ...
#   find_package( AtlasCMake )
#   atlas_project()
#
# While a derived project (building against a base release) could use
# it like:
#
#   cmake_minimum_required( VERSION 3.6 )
#   project( DerivedProject VERSION 1.0.0 [LANGUAGES ...] )
#   ...
#   find_package( BaseProject 1.0.0 )
#   atlas_project( USE BaseProject 1.0.0 )
#
# Usage: atlas_project( [USE BaseRelease 1.2.3]
#                       [PROJECT_ROOT ../../] )
#
function( atlas_project )

   # Parse all options.
   cmake_parse_arguments( ARG "" "PROJECT_ROOT" "USE" ${ARGN} )

   # Tell the user what's happening.
   message( STATUS "Configuring ATLAS project with name "
      "\"${CMAKE_PROJECT_NAME}\" and version \"${CMAKE_PROJECT_VERSION}\"" )

   # Check that the user specified meaningful base projects using the
   # USE parameter.
   if( ARG_USE )
      list( LENGTH ARG_USE _nUse )
      if( _nUse LESS 2 )
         message( SEND_ERROR
            "Wrong number of arguments in USE parameters (${ARG_USE})" )
         return()
      endif()
   endif()

   # Reset some global properties used by the code.
   set_property( GLOBAL PROPERTY ATLAS_EXPORTS OFF )
   set_property( GLOBAL PROPERTY ATLAS_EXPORTED_TARGETS "" )
   set_property( GLOBAL PROPERTY ATLAS_EXPORTED_PACKAGES "" )

   # Do not set any rpath values on any of the files.
   set( CMAKE_SKIP_RPATH ON )
   set( CMAKE_SKIP_RPATH ON PARENT_SCOPE )
   set( CMAKE_SKIP_BUILD_RPATH ON )
   set( CMAKE_SKIP_BUILD_RPATH ON PARENT_SCOPE )
   set( CMAKE_SKIP_INSTALL_RPATH ON )
   set( CMAKE_SKIP_INSTALL_RPATH ON PARENT_SCOPE )

   # Do not consider the includes associated with imported targets as system
   # includes. System includes on imported targets are explicitly marked as
   # such.
   set( CMAKE_NO_SYSTEM_FROM_IMPORTED ON )
   set( CMAKE_NO_SYSTEM_FROM_IMPORTED ON PARENT_SCOPE )

   # Make sure that the linker commands see the runtime environment of the
   # current project. To make sure that private dependencies on libraries could
   # be resolved correctly.
   set( _abr "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh" )
   set( CMAKE_C_LINK_EXECUTABLE "${_abr} ${CMAKE_C_LINK_EXECUTABLE}" )
   set( CMAKE_CXX_LINK_EXECUTABLE "${_abr} ${CMAKE_CXX_LINK_EXECUTABLE}" )
   set( CMAKE_CUDA_LINK_EXECUTABLE "${_abr} ${CMAKE_CUDA_LINK_EXECUTABLE}" )
   set( CMAKE_C_CREATE_SHARED_LIBRARY
      "${_abr} ${CMAKE_C_CREATE_SHARED_LIBRARY}" )
   set( CMAKE_CXX_CREATE_SHARED_LIBRARY
      "${_abr} ${CMAKE_CXX_CREATE_SHARED_LIBRARY}" )
   set( CMAKE_CUDA_CREATE_SHARED_LIBRARY
      "${_abr} ${CMAKE_CUDA_CREATE_SHARED_LIBRARY}" )
   set( CMAKE_C_CREATE_SHARED_MODULE
      "${_abr} ${CMAKE_C_CREATE_SHARED_MODULE}" )
   set( CMAKE_CXX_CREATE_SHARED_MODULE
      "${_abr} ${CMAKE_CXX_CREATE_SHARED_MODULE}" )
   set( CMAKE_CUDA_CREATE_SHARED_MODULE
      "${_abr} ${CMAKE_CUDA_CREATE_SHARED_MODULE}" )
   unset( _abr )

   # Tell the user what build type is being used.
   if( CMAKE_BUILD_TYPE )
      message( STATUS "Using build type: ${CMAKE_BUILD_TYPE}" )
   else()
      message( STATUS
         "No explicit build type requested, using \"Default\" build" )
   endif()

   # Construct a platform name. In a slightly complicated way. Allowing the
   # users to override the name selected automatically, using the
   # ATLAS_FORCE_PLATFORM cache variable.
   atlas_platform_id( _platform )
   set( ATLAS_FORCE_PLATFORM ${_platform} CACHE STRING
      "Build platform of the project" )
   set( ATLAS_PLATFORM ${ATLAS_FORCE_PLATFORM} )
   message( STATUS "Using platform name: ${ATLAS_PLATFORM}" )
   unset( _platform )

   # Set a default installation path. One that RPM making works with.
   if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
      set( CMAKE_INSTALL_PREFIX
         "/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_VERSION}/InstallArea/${ATLAS_PLATFORM}"
         CACHE PATH "Installation path for the project" FORCE )
   endif()

   # Set where to put files during compilation.
   set( _basedir "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}" )
   foreach( _config "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO" )
      set( CMAKE_LIBRARY_OUTPUT_DIRECTORY${_config}
         "${_basedir}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH
         "Directory used to store shared libraries during compilation" )
      set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY${_config}
         "${_basedir}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH
         "Directory used to store static libraries during compilation" )
      set( CMAKE_RUNTIME_OUTPUT_DIRECTORY${_config}
         "${_basedir}/${CMAKE_INSTALL_BINDIR}" CACHE PATH
         "Directory used to store executables during compilation" )
   endforeach()
   set( CMAKE_INCLUDE_OUTPUT_DIRECTORY
      "${_basedir}/${CMAKE_INSTALL_INCLUDEDIR}" CACHE PATH
      "Directory used to look up header files during compilation" )
   set( CMAKE_PYTHON_OUTPUT_DIRECTORY
      "${_basedir}/python" CACHE PATH
      "Directory collecting python modules in the build area" )
   set( CMAKE_DATA_OUTPUT_DIRECTORY
      "${_basedir}/data" CACHE PATH
      "Directory collecting data files in the build area" )
   set( CMAKE_SCRIPT_OUTPUT_DIRECTORY
      "${_basedir}/scripts" CACHE PATH
      "Directory collecting script files in the build area" )
   set( CMAKE_JOBOPT_OUTPUT_DIRECTORY
      "${_basedir}/jobOptions" CACHE PATH
      "Directory collecting jobOptions in the build area" )
   set( CMAKE_XML_OUTPUT_DIRECTORY
      "${_basedir}/XML" CACHE PATH
      "Directory collecting XML files in the build area" )
   set( CMAKE_SHARE_OUTPUT_DIRECTORY
      "${_basedir}/share" CACHE PATH
      "Directory collecting data files" )
   set( CMAKE_DOC_OUTPUT_DIRECTORY
      "${_basedir}/doc" CACHE PATH
      "Directory collecting documentation files" )
   unset( _basedir )

   # The default installation component name.
   set( CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "Main"
      CACHE STRING "Default installation component name" )

   # By default do not (re-)check wildcards on every incremental build.
   set( ATLAS_ALWAYS_CHECK_WILDCARDS FALSE CACHE BOOL
      "(Re-)Check wildcards in file specifications on every build" )

   # Enable folder view in IDEs.
   set_property( GLOBAL PROPERTY USE_FOLDERS ON )

   # Install all the CMake module code into the install area. The code needs
   # to be this convoluted, because CMAKE_CURRENT_LIST_DIR and
   # CMAKE_CURRENT_LIST_FILE both refer to the CMakeLists.txt file that's
   # including this file.
   find_file( _thisFile NAMES AtlasFunctions.cmake
      PATHS ${CMAKE_MODULE_PATH} )
   get_filename_component( _thisDir ${_thisFile} DIRECTORY )
   install( DIRECTORY ${_thisDir}/
      DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/modules
      USE_SOURCE_PERMISSIONS
      PATTERN ".svn" EXCLUDE
      PATTERN ".git" EXCLUDE
      PATTERN "*~" EXCLUDE )
   mark_as_advanced( _thisFile )
   unset( _thisFile )
   unset( _thisDir )

   # Cleanup stale files in build area.
   find_file( _buildCleaner NAMES cleanBuildArea.sh.in
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
      DOC "Script used for cleaning the build aread of stale files" )
   if( _buildCleaner )
      configure_file( "${_buildCleaner}"
         "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cleanBuildArea.sh"
         @ONLY )
      message( STATUS "Cleaning stale files from build area" )
      execute_process(
         COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/cleanBuildArea.sh
         OUTPUT_QUIET ERROR_QUIET )
   endif()
   mark_as_advanced( _buildCleaner )

   # Set up the "atlas_tests" target. One that all of the unit test builds
   # will depend on. This target will either be built by default or not based
   # on the value of the ATLAS_ALWAYS_BUILD_TESTS configuration option.
   option( ATLAS_ALWAYS_BUILD_TESTS
      "Make unit test building part of the default build target" ON )
   if( ATLAS_ALWAYS_BUILD_TESTS )
      message( STATUS "Unit tests will be built by default" )
      add_custom_target( atlas_tests ALL )
   else()
      message( STATUS "Unit tests will *NOT* be built by default" )
      message( STATUS "Use the 'atlas_tests' build target to build the tests" )
      add_custom_target( atlas_tests )
   endif()
   set_property( TARGET atlas_tests PROPERTY FOLDER ${CMAKE_PROJECT_NAME} )

   # Find the packages of the project.
   set( _packageDirs )
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   file( GLOB_RECURSE _cmakelistsFiles RELATIVE "${CMAKE_SOURCE_DIR}"
      ${_extraFlags} "${ARG_PROJECT_ROOT}CMakeLists.txt" )
   unset( _extraFlags )
   # Ignore the CMakeLists.txt file calling this function.
   list( REMOVE_ITEM _cmakelistsFiles "CMakeLists.txt" )
   # Get the paths of all of those files.
   foreach( _file ${_cmakelistsFiles} )
      get_filename_component( _package ${_file} DIRECTORY )
      list( APPEND _packageDirs ${_package} )
      unset( _package )
   endforeach()
   unset( _cmakelistsFiles )
   list( LENGTH _packageDirs _nPackageDirs )
   message( STATUS "Found ${_nPackageDirs} package(s)" )

   # Reset the base project variables.
   set( ATLAS_BASE_PROJECTS )
   set( ATLAS_BASE_PROJECT_NAMES )

   # If the build depends on an installed release, let's find it now.
   if( ARG_USE )
      # Loop over all specified base releases, in the order in which they
      # were specified.
      set( _bases ${ARG_USE} )
      while( _bases )
         # Check that the number of arguments looks good.
         list( LENGTH _bases _len )
         if( _len LESS 2 )
            message( SEND_ERROR
               "Wrong number of arguments given to the USE option" )
            return()
         endif()
         # Extract the release name and version, and then remove these entries
         # from the list.
         list( GET _bases 0 _base_project )
         list( GET _bases 1 _base_version )
         list( REMOVE_AT _bases 0 1 )
         # Make sure that the project version is a regular version number.
         if( NOT _base_version MATCHES "^[0-9]+[0-9.]*" )
            # Let's not specify a version in this case...
            message( STATUS "Using base project \"${_base_project}\" without "
               "its \"${_base_version}\" version name/number" )
            set( _base_version )
         endif()
         # Find the base release.
         find_package( ${_base_project} ${_base_version} QUIET )
         # Remember what projects, and what exact versions we found:
         list( APPEND ATLAS_BASE_PROJECTS ${_base_project}
            ${${_base_project}_VERSION} )
         set( ATLAS_BASE_PROJECT_NAMES
            "${ATLAS_BASE_PROJECT_NAMES} ${_base_project}" )
      endwhile()
      string( STRIP "${ATLAS_BASE_PROJECT_NAMES}" ATLAS_BASE_PROJECT_NAMES)
      unset( _bases )
      unset( _base_project )
      unset( _base_version )
      unset( _len )
   endif()

   # Reset the platform name, as the base projects may use a different one than
   # the one used for this project.
   set( ATLAS_PLATFORM ${ATLAS_FORCE_PLATFORM} )

   # Set the ATLAS_BASE_PROJECTS variable in the parent scope. To be able to
   # use it in the CPack configuration.
   set( ATLAS_BASE_PROJECTS ${ATLAS_BASE_PROJECTS} PARENT_SCOPE )

   # Figure out how often to print messages about "considering" packages for
   # inclusion. With the default being not printing more than 20 messages,
   # and not printing a message for less than 10 packages at a time.
   math( EXPR _interval "${_nPackageDirs} / 20" )
   if( ${_interval} LESS 10 )
      set( _interval 10 )
   endif()
   set( ATLAS_PACKAGE_PRINTOUT_INTERVAL ${_interval}
      CACHE STRING "Package counter printout interval" )

   # Include the packages.
   string( TIMESTAMP _timeStart "%s" )
   set( _counter 0 )
   set( _selectedPackages 0 )
   set( _selectedPackageDirs )

   # Generate a packages.txt file with a list of all built packages.
   set( _packagesFileName ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/packages.txt )
   file( WRITE ${_packagesFileName}
      "# Package(s) built in ${CMAKE_PROJECT_NAME} - ${CMAKE_PROJECT_VERSION}\n" )

   foreach( _pkgDir ${_packageDirs} )
      # Construct a binary directory name.
      set( _binDir ${_pkgDir} )
      if( ARG_PROJECT_ROOT )
         file( RELATIVE_PATH _binDir ${ARG_PROJECT_ROOT}
            ${CMAKE_CURRENT_SOURCE_DIR}/${_pkgDir} )
      endif()
      # Ignore packages that have "CMakeFiles" in their path name. As it is
      # *always* a problem when we encounter such a "package".
      if( "${_binDir}" MATCHES ".*CMakeFiles.*" )
         message( WARNING "Package \"${_binDir}\" is ignored. "
            "Make sure that you are using an out-of-source build directory!" )
         continue()
      endif()
      # Update the counter and possibly print a status message.
      math( EXPR _doNotPrint
         "${_counter} % ${ATLAS_PACKAGE_PRINTOUT_INTERVAL}" )
      math( EXPR _counter "${_counter} + 1" )
      if( NOT _doNotPrint )
         message( STATUS "Considering package ${_counter} / ${_nPackageDirs}" )
      endif()
      # Check if this package should be set up.
      atlas_is_package_selected( "${_binDir}" _isSelected )
      if( NOT "${_isSelected}" )
         continue()
      endif()
      # Set up the package.
      add_subdirectory( "${_pkgDir}" "${_binDir}" )
      math( EXPR _selectedPackages "${_selectedPackages} + 1" )
      list( APPEND _selectedPackageDirs ${_pkgDir} )
      # Add a line to the package list.
      file( APPEND ${_packagesFileName} "${_binDir}\n" )

      unset( _binDir )
   endforeach()

   # Print some summary about the packages.
   string( TIMESTAMP _timeStop "%s" )
   math( EXPR _timeSeconds "${_timeStop}-${_timeStart}" )
   message( STATUS "Number of packages configured: ${_selectedPackages}" )
   message( STATUS "Time for package configuration: ${_timeSeconds} second(s)" )
   message( STATUS "Generated file: ${_packagesFileName}" )
   # Install package file.
   install( FILES ${_packagesFileName} DESTINATION . )

   # Clean up.
   unset( _counter )
   unset( _doNotPrint )
   unset( _isSelected )
   unset( _selectedPackages )
   unset( _timeStart )
   unset( _timeStop )
   unset( _timeSeconds )
   unset( _packagesFileName )

   # Check whether all rules from a possible package filter file were used.
   atlas_print_unused_package_selection_rules()

   # Find the project's base releases again. This time including all the
   # targets from them that we need.
   if( ARG_USE )
      # Loop over all specified base releases, in reverse order.
      set( _bases ${ARG_USE} )
      while( _bases )
         # Extract the release name and version, and then remove these entries
         # from the list.
         list( GET _bases -2 _base_project )
         list( GET _bases -1 _base_version )
         list( REMOVE_AT _bases -2 -1 )
         # Make sure that the project version is a regular version number.
         if( NOT _base_version MATCHES "^[0-9]+[0-9.]*" )
            # Let's not specify a version in this case...
            message( STATUS "Using base project \"${_base_project}\" without "
               "its \"${_base_version}\" version name/number" )
            set( _base_version )
         endif()
         # Find the base release.
         find_package( ${_base_project} ${_base_version}
            COMPONENTS INCLUDE QUIET )
      endwhile()
      unset( _bases )
      unset( _base_project )
      unset( _base_version )
   endif()

   # Reset the platform name, as the base projects may use a different one than
   # the one used for this project.
   set( ATLAS_PLATFORM ${ATLAS_FORCE_PLATFORM} )
   set( ATLAS_PLATFORM ${ATLAS_FORCE_PLATFORM} PARENT_SCOPE )

   # Generate a compilers.txt file that list the types and versions of the
   # compilers that were used to build this project.
   set( _compilersFileName ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/compilers.txt )
   file( WRITE ${_compilersFileName}
      "# Compilers used to build ${CMAKE_PROJECT_NAME} - ${CMAKE_PROJECT_VERSION}\n"
      "CMAKE_C_COMPILER_ID ${CMAKE_C_COMPILER_ID}\n"
      "CMAKE_C_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION}\n"
      "CMAKE_CXX_COMPILER_ID ${CMAKE_CXX_COMPILER_ID}\n"
      "CMAKE_CXX_COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION}\n"
      "CMAKE_Fortran_COMPILER_ID ${CMAKE_Fortran_COMPILER_ID}\n"
      "CMAKE_Fortran_COMPILER_VERSION ${CMAKE_Fortran_COMPILER_VERSION}\n" )
   # Tell the user what happened.
   message( STATUS "Generated file: ${_compilersFileName}" )
   # Install this file.
   install( FILES ${_compilersFileName} DESTINATION . )
   # Clean up.
   unset( _compilersFileName )

   # Export the project's library targets.
   get_property( _exports GLOBAL PROPERTY ATLAS_EXPORTS )
   if( _exports )
      install( EXPORT ${CMAKE_PROJECT_NAME}Targets
         FILE "${CMAKE_PROJECT_NAME}Config-targets.cmake"
         DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
         NAMESPACE "${CMAKE_PROJECT_NAME}::" )
      # Sanitize the created file, replacing all FATAL_ERROR messages
      # with WARNING ones.
      set( CMAKE_TARGETS_FILE
         "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${CMAKE_PROJECT_NAME}Config-targets.cmake" )
      find_file( _sanitizer atlas_export_sanitizer.cmake.in
         PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
         DOC "Script used to sanitize the exported CMake configuration" )
      if( _sanitizer )
         configure_file( "${_sanitizer}"
            "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_export_sanitizer.cmake"
            @ONLY )
         install( SCRIPT
            "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_export_sanitizer.cmake" )
      endif()
      unset( CMAKE_TARGETS_FILE )
      mark_as_advanced( _sanitizer )
      unset( _sanitizer )
   endif()

   # Get the names of the exported targets and packages, needed for the project
   # configuration file generation.
   get_property( ATLAS_EXPORTED_TARGETS GLOBAL PROPERTY ATLAS_EXPORTED_TARGETS )
   get_property( ATLAS_EXPORTED_PACKAGES GLOBAL PROPERTY
      ATLAS_EXPORTED_PACKAGES )

   # Construct the project's exported CMake configuration file(s).
   find_file( _projConf ProjectConfig.cmake.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH}
      DOC "Project configuration file skeleton" )
   mark_as_advanced( _projConf )
   set( ATLAS_INSTALL_DIR "" )
   configure_package_config_file( "${_projConf}"
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${CMAKE_PROJECT_NAME}Config.cmake"
      INSTALL_DESTINATION "${CMAKE_INSTALL_CMAKEDIR}"
      PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
                CMAKE_INSTALL_BINDIR CMAKE_INSTALL_CMAKEDIR
                CMAKE_INSTALL_PYTHONDIR ATLAS_INSTALL_DIR
      NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO )
   unset( ATLAS_INSTALL_DIR )
   write_basic_package_version_file(
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${CMAKE_PROJECT_NAME}Config-version.cmake"
      VERSION "${CMAKE_PROJECT_VERSION}"
      COMPATIBILITY SameMinorVersion
      ARCH_INDEPENDENT )
   install( FILES
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${CMAKE_PROJECT_NAME}Config.cmake"
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${CMAKE_PROJECT_NAME}Config-version.cmake"
      DESTINATION ${CMAKE_INSTALL_CMAKEDIR} )

   # Set up the environment setup file meant for BASH/ZSH.
   find_file( _setupSkeleton setup.sh.in
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
      DOC "Environment setup script skeleton" )
   configure_file( "${_setupSkeleton}"
      "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/setup.sh" @ONLY )
   mark_as_advanced( _setupSkeleton )
   unset( _setupSkeleton )
   install( FILES "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/setup.sh"
      DESTINATION . )
   set_property( DIRECTORY "${CMAKE_SOURCE_DIR}" APPEND PROPERTY
      ADDITIONAL_MAKE_CLEAN_FILES
      "${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/setup.sh" )

   # Set up Doxygen.
   set( ATLAS_DOXYFILE_INPUT "${CMAKE_SOURCE_DIR}/Doxyfile.in"
      CACHE FILEPATH "Path to the Doxyfile that needs to be configured" )

   if( EXISTS "${ATLAS_DOXYFILE_INPUT}" )

      if( NOT DOXYGEN_EXECUTABLE )
         find_package( Doxygen QUIET )
      endif()
      if ( DOXYGEN_EXECUTABLE )
         # Tell the user what's happening.
         message( STATUS "Setting up the 'doc' target using Doxygen" )

         # Collect the paths of all selected packages.
         set( SELECTED_PACKAGES )
         foreach( _pkgDir ${_selectedPackageDirs} )
            file( RELATIVE_PATH _srcDir "${CMAKE_BINARY_DIR}"
               "${CMAKE_SOURCE_DIR}/${_pkgDir}" )
            set( SELECTED_PACKAGES "${SELECTED_PACKAGES} ${_srcDir}" )
         endforeach()

         # Specialise the project-specific Doxyfile.
         set( _doxyfile "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Doxyfile" )
         configure_file( "${ATLAS_DOXYFILE_INPUT}" "${_doxyfile}" @ONLY )

         # Set up the custom command for calling Doxygen.
         add_custom_target( doc
            ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
            ${DOXYGEN_EXECUTABLE} "${_doxyfile}"
            COMMENT "Generating Doxygen documentation"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" )

         # Clean up.
         unset( SELECTED_PACKAGES )
         unset( _doxyfile )
      endif()

   endif()

   # Clean up.
   unset( _selectedPackageDirs )

   # On MacOS X copy the system's default BASH executable into the build
   # directory, and use it from there. To get around the issue with Apple's
   # System Integrity Protection against passing some environment variables to
   # certain applications.

   # Find bash.
   find_program( _bash_executable bash
      DOC "The bash executable to use during the build" )
   mark_as_advanced( _bash_executable )
   if( NOT _bash_executable )
      message( WARNING "BASH not found. The build will fail." )
   endif()

   if( APPLE )
      # Copy bash into the build directory.
      file( COPY "${_bash_executable}"
         DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )
      # And now set BASH_EXECUTABLE to point at this private copy.
      set( BASH_EXECUTABLE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bash" )
   else()
      # Just take bash from its normal location.
      set( BASH_EXECUTABLE "${_bash_executable}" )
   endif()

   # Set up a script that can be used during the build to run executables
   # in a "full runtime environment".
   find_file( _buildRun atlas_build_run.sh.in
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   configure_file( ${_buildRun}
      "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh" @ONLY )
   mark_as_advanced( _buildRun )
   unset( _buildRun )

   # Set up a ReleaseData file for the project.
   atlas_generate_releasedata()

   # Set up all project-wide operations, which come after all the packages
   # have been built:
   atlas_merge_project_files()

endfunction( atlas_project )

# This macro is used as the first declaration in software packages. It sets
# up a few things:
#  - Sets a variable called ATLAS_PACKAGE that holds the name of the package,
#    and is used by almost all of the build/install commands used later on.
#  - Sets up a custom target called "Package_<PkgName>", which will eventually
#    have all build operations of the package as a dependency. Which is used
#    both for interactive compilation (build everything in a given package),
#    and to provide build results to the nightly system.
#  - It installs all the compilable sources of the package into the install
#    area. Mainly for release archeology reasons.
#
# Usage: atlas_subdir( CxxUtils )
#
macro( atlas_subdir name )

   # Set the package's name, so other function calls in the package's
   # CMakeLists.txt file could pick it up.
   set( ATLAS_PACKAGE ${name} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXPORTED_PACKAGES ${name} )

   # Create a target that can be used to get the names of all the packages
   # in the current project, in a generator expression.
   if( NOT TARGET ATLAS_PACKAGES_TARGET )
      add_custom_target( ATLAS_PACKAGES_TARGET )
      set_property( TARGET ATLAS_PACKAGES_TARGET PROPERTY FOLDER
         ${CMAKE_PROJECT_NAME} )
   endif()

   # Tell the user what's happening.
   message( STATUS "Configuring the build of package: ${name}" )

   # Add it to the list of built packages.
   set_property( TARGET ATLAS_PACKAGES_TARGET APPEND PROPERTY
      ATLAS_PACKAGES ${name} )

   # Check if a package with this name was already declared:
   if( TARGET Package_${name} )
      message( FATAL_ERROR "Package ${name} is already declared." )
   endif()

   # Get the source path of the package:
   atlas_get_package_dir( pkgDir )

   # Check if the package name matches the (leaf) directory name:
   get_filename_component( pkgDirName ${pkgDir} NAME )
   if( NOT "${name}" STREQUAL "${pkgDirName}" )
      message( SEND_ERROR "The package name ${name} does not match "
         "the directory name ${pkgDirName} in ${pkgDir}." )
   endif()

   # Create a final target for the package. One that will depend on all
   # build operations of the package.
   set( _echoCommand )
   if( CTEST_USE_LAUNCHERS )
      set( _echoCommand COMMAND
         ${CMAKE_COMMAND} -E echo "${name}: Package build succeeded" )
   endif()
   add_custom_target( Package_${name} ALL
      ${_echoCommand}
      COMMENT "${name}: Package build succeeded" )
   set_property( TARGET Package_${name} PROPERTY
      FOLDER ${pkgDir}/Internals )
   unset( _echoCommand )

   # Set up the installation of the source files from the package.
   install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
      DESTINATION ${CMAKE_INSTALL_SRCDIR}/${pkgDir}
      USE_SOURCE_PERMISSIONS
      PATTERN ".svn" EXCLUDE
      PATTERN ".git" EXCLUDE
      PATTERN "*~" EXCLUDE )

   # Clean up.
   unset( pkgDir )

endmacro( atlas_subdir )

# This function declares the dependencies of a package on other packages.
# It takes a list of package names with their full path, but in reality
# it only uses the names of the packages themselves. (The last directory in the
# path.)
#
# Usage: atlas_depends_on_subdirs( [PUBLIC] Control/AthContainers
#                                  [PRIVATE Control/CxxUtils] )
#
function( atlas_depends_on_subdirs )

   message( SEND_ERROR "atlas_depends_on_subdirs has been deprecated. See \
      https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/SoftwareDevelopmentWorkBookCMakeInAtlas" )

endfunction( atlas_depends_on_subdirs )

# Function creating an alias for a specific file. It is used to create
# aliases of executables/scripts. Mostly just for convenience reasons.
#
# Usage: atlas_add_alias( name command1 command2... )
#
function( atlas_add_alias name )

   # A little sanity check:
   if( NOT ARGN )
      message( SEND_ERROR "No command specified for alias ${name}" )
      return()
   endif()

   # Construct the command to execute:
   set( _cmd )
   foreach( _arg ${ARGN} )
      set( _cmd "${_cmd} \"${_arg}\"" )
   endforeach()

   # Generate the script.
   find_file( _aliasTemplate NAMES aliasTemplate.in
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH}
      DOC "Template to set up aliases with" )
   mark_as_advanced( _aliasTemplate )
   configure_file( ${_aliasTemplate} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name}
      @ONLY )

   # And then install it.
   install( PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name}
      DESTINATION ${CMAKE_INSTALL_BINDIR} )

endfunction( atlas_add_alias )

# This function can be used to declare executables in a package.
#
# Usage:  atlas_add_executable( ExecutableName util/source1.cxx...
#                               [INCLUDE_DIRS Include1...]
#                               [LINK_LIBRARIES Library1...] )
#
function( atlas_add_executable exeName )

   # Parse the options given to the function.
   cmake_parse_arguments( ARG "" "" "INCLUDE_DIRS;LINK_LIBRARIES" ${ARGN} )

   # Set common compiler options.
   atlas_set_compiler_flags()

   # Get the package/subdirectory name.
   atlas_get_package_name( pkgName )

   # Get the package directory.
   atlas_get_package_dir( pkgDir )

   # Allow wildcards in the source names.
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   file( GLOB _sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
      ${ARG_UNPARSED_ARGUMENTS} )
   unset( _extraFlags )

   # Put the files into source groups. So they would show up in a ~reasonable
   # way in an IDE like Xcode.
   atlas_group_source_files( ${_sources} )

   # Declare the executable.
   add_executable( ${exeName} ${_sources} )

   # Set it's properties:
   add_dependencies( Package_${pkgName} ${exeName} )
   set_property( TARGET ${exeName} PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${exeName} PROPERTY FOLDER ${pkgDir} )
   if( ARG_INCLUDE_DIRS )
      target_include_directories( ${exeName} SYSTEM BEFORE PRIVATE
         ${ARG_INCLUDE_DIRS} )
   endif()
   target_include_directories( ${exeName} BEFORE PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
   if( ARG_LINK_LIBRARIES )
      target_link_libraries( ${exeName} PRIVATE ${ARG_LINK_LIBRARIES} )
   endif()

   # Make sure that .cu files are compiled as C++ files when CUDA is not
   # available during the compilation.
   if( NOT CMAKE_CUDA_COMPILER )
      foreach( _source ${_sources} )
         if( "${_source}" MATCHES ".*\.cu$" )
            set_source_files_properties( "${_source}" PROPERTIES LANGUAGE CXX )
            if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR
               "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" )
               set_source_files_properties( "${_source}" PROPERTIES
                  COMPILE_FLAGS "-x c++" )
            endif()
         endif()
      endforeach()
   endif()

   # In case we are building optimised executables with debug info, and we have
   # objcopy available, detach the debug information into a separate file.
   if( "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" AND CMAKE_OBJCOPY )
      add_custom_command( TARGET ${exeName} POST_BUILD
         COMMAND ${CMAKE_OBJCOPY} --only-keep-debug ${exeName} ${exeName}.dbg
         COMMAND ${CMAKE_OBJCOPY} --strip-debug ${exeName}
         COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=${exeName}.dbg ${exeName}
         WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
         COMMENT
         "Detaching debug info of ${exeName} into ${exeName}.dbg" )
      install( FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${exeName}.dbg
         DESTINATION ${CMAKE_INSTALL_BINDIR}
         COMPONENT "Debug"
         OPTIONAL )
   endif()

   # Set up the installation of the executable.
   install( TARGETS ${exeName}
      EXPORT ${CMAKE_PROJECT_NAME}Targets
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL )
   set_property( GLOBAL PROPERTY ATLAS_EXPORTS ON )
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXPORTED_TARGETS ${exeName} )

endfunction( atlas_add_executable )

# Macro for turning off the -Wl,--as-needed linker flag for the current
# package. Needed a couple of packages.
#
# Usage: atlas_disable_as_needed()
#
macro( atlas_disable_as_needed )

   # Remove this flag from all of the linker flag variables:
   string( REPLACE "-Wl,--as-needed" "" CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS}" )
   string( REPLACE "-Wl,--as-needed" "" CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS}" )
   string( REPLACE "-Wl,--as-needed" "" CMAKE_MODULE_LINKER_FLAGS
      "${CMAKE_MODULE_LINKER_FLAGS}" )

endmacro( atlas_disable_as_needed )

# Macro for turning off the -Wl,--no-undefined linked flag for the current
# package. Should only be used in exceptional circumstances.
#
# Usage: atlas_disable_no_undefined()
#
macro( atlas_disable_no_undefined )

   # Remove this flag from all of the linker flag variables:
   string( REPLACE "-Wl,--no-undefined" "" CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS}" )
   string( REPLACE "-Wl,--no-undefined" "" CMAKE_SHARED_LINKER_FLAGS
      "${CMAKE_SHARED_LINKER_FLAGS}" )
   string( REPLACE "-Wl,--no-undefined" "" CMAKE_MODULE_LINKER_FLAGS
      "${CMAKE_MODULE_LINKER_FLAGS}" )

endmacro( atlas_disable_no_undefined )
