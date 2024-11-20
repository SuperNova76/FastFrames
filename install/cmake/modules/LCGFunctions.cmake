# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Some helper functions, used by the Find<Bla>.cmake modules of the package.
#

# CMake built-in module(s):
include( CMakeParseArguments )

# Master macro for finding "simple externals".
#
# Since many of the modules in this package look extremely similar, they are now
# implemented using this macro. To make their maintenance and development a bit
# easier.
#
# The produced code will rely on the <NAME>_LCGROOT variable to find the
# requested external, and will produce the following possible variables:
#   - <NAME>_INCLUDE_DIRS
#   - <NAME>_LIBRARIES
#   - <NAME>_LIBRARY_DIRS
#   - <NAME>_BINARY_PATH
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
# Usage: lcg_external_module( NAME name
#                             [INCLUDE_SUFFIXES include1 include2...]
#                             [INCLUDE_NAMES header1 header2...]
#                             [DEFAULT_COMPONENTS component1 component2...]
#                             [COMPULSORY_COMPONENTS component3 component4...]
#                             [LIBRARY_SUFFIXES libDir1 libDir2...]
#                             [LIBRARY_PREFIX prefix_]
#                             [LIBRARY_POSTFIX _postfix]
#                             [BINARY_NAMES exe1 exe2...]
#                             [BINARY_SUFFIXES dir3 dir4...]
#                             [EXTRA_OPTIONS cmakeOpt1...]
#                             [SEARCH_PATHS path1 path2...] )
#
function( lcg_external_module )

   # Parse the arguments:
   set( _singleArgParams NAME LIBRARY_PREFIX LIBRARY_POSTFIX )
   set( _multiArgParams INCLUDE_SUFFIXES INCLUDE_NAMES DEFAULT_COMPONENTS
      BINARY_NAMES BINARY_SUFFIXES COMPULSORY_COMPONENTS
      LIBRARY_SUFFIXES EXTRA_OPTIONS SEARCH_PATHS )
   cmake_parse_arguments( ARG "" "${_singleArgParams}" "${_multiArgParams}"
      ${ARGN} )

   # Create an uppercase version of the name:
   string( TOUPPER ${ARG_NAME} nameUpper )

   # If the external was already found, set the module to silent mode.
   if( ${ARG_NAME}_FOUND OR ${nameUpper}_FOUND )
      set( ${ARG_NAME}_FIND_QUIETLY TRUE PARENT_SCOPE )
   endif()

   # If we are to take the external from an LCG release, then hide the locally
   # installed stuff from the code:
   if( ${nameUpper}_LCGROOT )
      list( APPEND CMAKE_SYSTEM_IGNORE_PATH /usr/include
         /usr/bin /usr/bin32 /usr/bin64
         /usr/lib /usr/lib32 /usr/lib64
         /bin /bin32 /bin64
         /lib /lib32 /lib64 )
      list( REMOVE_DUPLICATES CMAKE_SYSTEM_IGNORE_PATH )
      list( APPEND ARG_EXTRA_OPTIONS
         NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH )
      list( REMOVE_DUPLICATES ARG_EXTRA_OPTIONS )
   endif()

   # Set up the LCG_SYSTEM_IGNORE_PATH cache variable.
   lcg_system_ignore_path_setup()

   # Find the include directory of the external:
   if( ARG_INCLUDE_NAMES )
      find_path( ${nameUpper}_INCLUDE_DIR NAMES ${ARG_INCLUDE_NAMES}
         PATH_SUFFIXES ${ARG_INCLUDE_SUFFIXES}
         PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
         ${ARG_EXTRA_OPTIONS} )
      set( _relocatableDir ${${nameUpper}_INCLUDE_DIR} )
      _lcg_make_paths_relocatable( _relocatableDir )
      if( GAUDI_ATLAS )
         set( ${nameUpper}_INCLUDE_DIRS ${${nameUpper}_INCLUDE_DIR} )
         set( ${nameUpper}_INCLUDE_DIRS ${${nameUpper}_INCLUDE_DIR}
            PARENT_SCOPE )
      else()
         set( ${nameUpper}_INCLUDE_DIRS
            $<BUILD_INTERFACE:${${nameUpper}_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${_relocatableDir}> )
         set( ${nameUpper}_INCLUDE_DIRS
            $<BUILD_INTERFACE:${${nameUpper}_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${_relocatableDir}> PARENT_SCOPE )
      endif()
      unset( _relocatableDir )
   endif()

   # Look for an optional binary directory, and the executables therein.
   if( ARG_BINARY_NAMES )
      foreach( binary ${ARG_BINARY_NAMES} )
         find_program( ${nameUpper}_${binary}_EXECUTABLE
            NAMES ${binary}
            PATH_SUFFIXES ${ARG_BINARY_SUFFIXES}
            PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
            ${ARG_EXTRA_OPTIONS} )
         if( ${nameUpper}_${binary}_EXECUTABLE AND
            NOT TARGET ${ARG_NAME}::${binary} )
            add_executable( ${ARG_NAME}::${binary} IMPORTED )
            set_target_properties( ${ARG_NAME}::${binary} PROPERTIES
               IMPORTED_LOCATION "${${nameUpper}_${binary}_EXECUTABLE}" )
         endif()
      endforeach()
      find_path( _${nameUpper}_BINARY_PATH
         NAMES ${ARG_BINARY_NAMES}
         PATH_SUFFIXES ${ARG_BINARY_SUFFIXES}
         PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
         ${ARG_EXTRA_OPTIONS} )
      set( _relocatableDir ${_${nameUpper}_BINARY_PATH} )
      _lcg_make_paths_relocatable( _relocatableDir )
      if( _${nameUpper}_BINARY_PATH )
         if( GAUDI_ATLAS )
            set( ${nameUpper}_BINARY_PATH ${_${nameUpper}_BINARY_PATH}
               PARENT_SCOPE )
         else()
            set( ${nameUpper}_BINARY_PATH
               $<BUILD_INTERFACE:${_${nameUpper}_BINARY_PATH}>
               $<INSTALL_INTERFACE:${_relocatableDir}> PARENT_SCOPE )
         endif()
      endif()
      unset( _relocatableDir )
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
      set( ${nameUpper}_LIBRARIES ${nameUpper}_LIBRARIES-NOTFOUND )
      set( ${nameUpper}_LIBRARY_DIRS ${nameUpper}_LIBRARY_DIRS-NOTFOUND )
      # Find the requested component(s):
      foreach( component ${${ARG_NAME}_FIND_COMPONENTS} )
         # Search for the requested library:
         find_library( _${nameUpper}_${component}_library
            NAMES ${ARG_LIBRARY_PREFIX}${component}${ARG_LIBRARY_POSTFIX}
            PATH_SUFFIXES ${ARG_LIBRARY_SUFFIXES}
            PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
            ${ARG_EXTRA_OPTIONS} )
         mark_as_advanced( _${nameUpper}_${component}_library )
         # Deal with the search result:
         if( _${nameUpper}_${component}_library )
            # Set up an imported target for the library.
            if( NOT TARGET ${ARG_NAME}::${component} )
               add_library( ${ARG_NAME}::${component} UNKNOWN IMPORTED )
               get_filename_component( _installPath
                  "${${nameUpper}_INCLUDE_DIR}" DIRECTORY )
               set_target_properties( ${ARG_NAME}::${component} PROPERTIES
                  INTERFACE_INCLUDE_DIRECTORIES "${${nameUpper}_INCLUDE_DIRS}"
                  IMPORTED_LOCATION "${_${nameUpper}_${component}_library}"
                  INSTALL_PATH "${_installPath}" )
               unset( _installPath )
            endif()
            # Set up "regular variables" for the library.
            set( ${nameUpper}_${component}_FOUND TRUE )
            set( ${nameUpper}_${component}_FOUND TRUE PARENT_SCOPE )
            set( _relocatableLib ${_${nameUpper}_${component}_library} )
            _lcg_make_paths_relocatable( _relocatableLib )
            if( GAUDI_ATLAS )
               set( ${nameUpper}_${component}_LIBRARY
                  ${_${nameUpper}_${component}_library} )
               set( ${nameUpper}_${component}_LIBRARY
                  ${_${nameUpper}_${component}_library} PARENT_SCOPE )
            else()
               set( ${nameUpper}_${component}_LIBRARY
                  $<BUILD_INTERFACE:${_${nameUpper}_${component}_library}>
                  $<INSTALL_INTERFACE:${_relocatableLib}> )
               set( ${nameUpper}_${component}_LIBRARY
                  $<BUILD_INTERFACE:${_${nameUpper}_${component}_library}>
                  $<INSTALL_INTERFACE:${_relocatableLib}> PARENT_SCOPE )
            endif()
            list( APPEND ${nameUpper}_FOUND_COMPONENTS ${component} )
            if( NOT ${nameUpper}_LIBRARIES )
               set( ${nameUpper}_LIBRARIES
                  ${${nameUpper}_${component}_LIBRARY} )
            else()
               list( APPEND ${nameUpper}_LIBRARIES
                  ${${nameUpper}_${component}_LIBRARY} )
            endif()
            get_filename_component( _libdir
               ${_${nameUpper}_${component}_library} PATH )
            if( NOT ${nameUpper}_LIBRARY_DIRS )
               set( ${nameUpper}_LIBRARY_DIRS ${_libdir} )
            else()
               list( APPEND ${nameUpper}_LIBRARY_DIRS ${_libdir} )
            endif()
            unset( _relocatableLib )
            unset( _libdir )
         else()
            set( ${nameUpper}_${component}_FOUND FALSE )
            set( ${nameUpper}_${component}_FOUND FALSE PARENT_SCOPE )
         endif()
         unset( _${nameUpper}_{component}_library )
         # Mark the component location as an advanced property:
         mark_as_advanced( ${nameUpper}_${component}_LIBRARY
            ${nameUpper}_${component}_FOUND )
      endforeach()
      # Remove the duplicate library directory entries:
      if( ${nameUpper}_LIBRARY_DIRS )
         list( REMOVE_DUPLICATES ${nameUpper}_LIBRARY_DIRS )
      endif()
   endif()

   # Return all the useful variables:
   set( ${nameUpper}_FOUND_COMPONENTS ${${nameUpper}_FOUND_COMPONENTS}
      PARENT_SCOPE )
   set( ${nameUpper}_LIBRARIES ${${nameUpper}_LIBRARIES} PARENT_SCOPE )
   set( ${nameUpper}_LIBRARY_DIRS ${${nameUpper}_LIBRARY_DIRS} PARENT_SCOPE )

endfunction( lcg_external_module )

# Macro used for setting up Python packages from an LCG release
#
# While lcg_external_module(...) is meant primarily for externals providing
# shared libraries and the accompanying headers, this function is focusing on
# setting up Python packages from an LCG release.
#
# The function relies on the <NAME>_LCGROOT variable in finding the Python
# package, and sets <NAME>_PYTHON_PATH to the appropriate value when the
# package was found. Note that the standard "lib/pythonX.Y/site-packages"
# and "lib/pythonX.Y/dist-packages" path suffixes are searched by default
# by the function.
#
# When binary names are provided using BINARY_NAMES, the function may also
# set <NAME>_BINARY_PATHS with the paths containing those binaries. (If found.)
#
# If <NAME>_PYTHON_PATH is found as a valid path, the function also tries to
# set <NAME>_VERSION by looking at the "__version__" variable/property of the
# Python module specified through MODULE_NAME, if MODULE_NAME is specified.
#
# Usage:
#   lcg_python_external_module( NAME name
#                               PYTHON_NAMES foo/bar.py...
#                               [MODULE_NAME name]
#                               [PYTHON_SUFFIXES dir1 dir2...]
#                               [BINARY_NAMES exe1 exe2...]
#                               [BINARY_SUFFIXES dir3 dir4...]
#                               [EXTRA_OPTIONS cmakeOpt1...]
#                               [SEARCH_PATHS path1 path2...] )
#
function( lcg_python_external_module )

   # Parse the arguments.
   set( _singleArgParams NAME MODULE_NAME )
   set( _multiArgParams PYTHON_NAMES PYTHON_SUFFIXES
      BINARY_NAMES BINARY_SUFFIXES EXTRA_OPTIONS SEARCH_PATHS )
   cmake_parse_arguments( ARG "" "${_singleArgParams}" "${_multiArgParams}"
      ${ARGN} )

   # Some sanity checks.
   if( NOT ARG_NAME )
      message( SEND_ERROR "A name must be specified for the python module!" )
      return()
   endif()
   if( NOT ARG_PYTHON_NAMES )
      message( SEND_ERROR "The function is meant to look for python files!" )
      return()
   endif()

   # Create an uppercase version of the name.
   string( TOUPPER ${ARG_NAME} nameUpper )

   # If the external was already found, set the module to silent mode.
   if( ${ARG_NAME}_FOUND OR ${nameUpper}_FOUND )
      set( ${ARG_NAME}_FIND_QUIETLY TRUE PARENT_SCOPE )
   endif()

   # If we are to take the external from an LCG release, then hide the locally
   # installed stuff from the code.
   if( ${nameUpper}_LCGROOT )
      list( APPEND CMAKE_SYSTEM_IGNORE_PATH /usr/include
         /usr/bin /usr/bin32 /usr/bin64
         /usr/lib /usr/lib32 /usr/lib64 )
      list( REMOVE_DUPLICATES CMAKE_SYSTEM_IGNORE_PATH )
      list( APPEND ARG_EXTRA_OPTIONS
         NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH )
      list( REMOVE_DUPLICATES ARG_EXTRA_OPTIONS )
   endif()

   # Set up the LCG_SYSTEM_IGNORE_PATH cache variable.
   lcg_system_ignore_path_setup()

   # Find Python (we only need the version below).
   if( (NOT Python_VERSION_MAJOR) OR (NOT Python_VERSION_MINOR) )
      find_package( Python COMPONENTS Interpreter )
      if( NOT Python_FOUND )
         return()
      endif()
   endif()

   # Find the location of the Python package.
   find_path( _${nameUpper}_PYTHON_PATH
      NAMES ${ARG_PYTHON_NAMES}
      PATH_SUFFIXES
      lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages
      lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/dist-packages
      ${ARG_PYTHON_SUFFIXES}
      PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
      ${ARG_EXTRA_OPTIONS} )
   mark_as_advanced( _${nameUpper}_PYTHON_PATH )

   # Try to get the version of the module.
   if( ARG_MODULE_NAME AND _${nameUpper}_PYTHON_PATH )
      execute_process( COMMAND "${CMAKE_COMMAND}"
         -E env PYTHONPATH="${_${nameUpper}_PYTHON_PATH}:$ENV{PYTHONPATH}"
         "${Python_EXECUTABLE}"
         -c "import ${ARG_MODULE_NAME}; print(${ARG_MODULE_NAME}.__version__)"
         OUTPUT_VARIABLE _output
         ERROR_VARIABLE _output
         RESULT_VARIABLE _returncode )
      if( ${_returncode} EQUAL 0 )
         string( STRIP "${_output}" _version )
         set( ${nameUpper}_VERSION "${_version}" PARENT_SCOPE )
      else()
         set( ${nameUpper}_VERSION "${nameUpper}_VERSION-NOTFOUND"
            PARENT_SCOPE )
      endif()
   endif()

   # Make the found path relocatable.
   if( _${nameUpper}_PYTHON_PATH )
      set( _relocatableDir ${_${nameUpper}_PYTHON_PATH} )
      _lcg_make_paths_relocatable( _relocatableDir )
      if( GAUDI_ATLAS )
         set( ${nameUpper}_PYTHON_PATH ${_${nameUpper}_PYTHON_PATH}
            PARENT_SCOPE )
      else()
         set( ${nameUpper}_PYTHON_PATH
            $<BUILD_INTERFACE:${_${nameUpper}_PYTHON_PATH}>
            $<INSTALL_INTERFACE:${_relocatableDir}> PARENT_SCOPE )
      endif()
      unset( _relocatableDir )
   endif()

   # Look for an optional binary directory, and the executables therein.
   if( ARG_BINARY_NAMES )
      foreach( binary ${ARG_BINARY_NAMES} )
         find_program( ${nameUpper}_${binary}_EXECUTABLE
            NAMES ${binary}
            PATH_SUFFIXES ${ARG_BINARY_SUFFIXES}
            PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
            ${ARG_EXTRA_OPTIONS} )
      endforeach()
      find_path( _${nameUpper}_BINARY_PATH
         NAMES ${ARG_BINARY_NAMES}
         PATH_SUFFIXES ${ARG_BINARY_SUFFIXES}
         PATHS ${ARG_SEARCH_PATHS} ${${nameUpper}_LCGROOT}
         ${ARG_EXTRA_OPTIONS} )
      mark_as_advanced( _${nameUpper}_BINARY_PATH )
      if( _${nameUpper}_BINARY_PATH )
         set( _relocatableDir ${_${nameUpper}_BINARY_PATH} )
         _lcg_make_paths_relocatable( _relocatableDir )
         if( GAUDI_ATLAS )
            set( ${nameUpper}_BINARY_PATH ${_${nameUpper}_BINARY_PATH}
               PARENT_SCOPE )
         else()
            set( ${nameUpper}_BINARY_PATH
               $<BUILD_INTERFACE:${_${nameUpper}_BINARY_PATH}>
               $<INSTALL_INTERFACE:${_relocatableDir}> PARENT_SCOPE )
         endif()
         unset( _relocatableDir )
      endif()
   endif()

endfunction( lcg_python_external_module )

# Helper function to set up a projects dependence on LCG RPM packages
#
# This function needs to be used in the Find<Bla>.cmake modules to set up the
# current project's dependence on LCG's RPM packages.
#
# Usage: lcg_need_rpm( Python
#                      [FOUND_NAME PYTHONLIBS]
#                      [VERSION_NAME PYTHON] )
#
function( lcg_need_rpm name )

   # Add the optional argument(s):
   cmake_parse_arguments( ARG "" "FOUND_NAME;VERSION_NAME" "" ${ARGN} )

   # In case no actual LCG release is set up, don't set up any LCG RPM
   # dependencies either:
   if( LCG_VERSION EQUAL 0 )
      return()
   endif()

   # If an LCG release *is* set up, but it doesn't provide this external,
   # then stop as well.
   string( TOUPPER ${name} nameUpper )
   if( NOT ${name}_LCGROOT AND NOT ${nameUpper}_LCGROOT )
      return()
   endif()
   unset( nameUpper )

   # Create the "found name" of the package:
   if( ARG_FOUND_NAME )
      set( nameFound ${ARG_FOUND_NAME} )
   else()
      string( TOUPPER ${name} nameFound )
   endif()

   # If the package was not found, return now:
   if( NOT ${nameFound}_FOUND )
      return()
   endif()

   # Create a "version name" for the package:
   if( ARG_VERSION_NAME )
      set( nameVersion ${ARG_VERSION_NAME} )
   else()
      set( nameVersion ${nameFound} )
   endif()

   # Construct the package's version. The trick here is that LCG replaces
   # "-" characters with "_" ones in the version names.
   string( REPLACE "-" "_" version "${${nameVersion}_LCGVERSION}" )

   # The same applies for the platform name. SFT for some reason likes to
   # use underscores in that too...
   string( REPLACE "-" "_" platform "${LCG_PLATFORM}" )

   # Construct the RPM package name for this dependency:
   set( _rpmName
      "LCG_${LCG_VERSION}_${name}_${version}_${platform}" )

   # Add it to the global list of dependencies:
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXTERNAL_RPMS ${_rpmName} )

endfunction( lcg_need_rpm )

# Helper function used internally to make paths easier to relocate
#
# This function is used internally to put in ${LCG_RELEASE_BASE} into path
# lists, so that the generated environment setup files would be easier to
# adjust to a relocation.
#
# Usage: _lcg_make_paths_relocatable( _python_paths )
#
function( _lcg_make_paths_relocatable pathList )

   # Create the result list:
   set( _result )

   # Loop over the received list, and do the replacements:
   foreach( _path ${${pathList}} )
      string( REPLACE "${LCG_RELEASE_BASE}" "\${LCG_RELEASE_BASE}"
         _relocatablePath ${_path} )
      list( APPEND _result ${_relocatablePath} )
   endforeach()

   # Return the list to the caller:
   set( ${pathList} ${_result} PARENT_SCOPE )

endfunction( _lcg_make_paths_relocatable )

# Helper function used internally to make replacements in paths
#
# In order to be able to generate setup files that would refer to ATLAS
# projects with valid relative paths after installation on AFS, this function
# is used to massage path names according to replacement rules specified
# by the user.
#
# Usage: _lcg_replace_paths( _python_paths REPLACE_LIST )
#
function( _lcg_replace_paths pathList replaceList )

   # Create the result list:
   set( _result )

   # Loop over the environment list:
   foreach( _path ${${pathList}} )

      # Loop over the replacement list:
      set( _replaceListCopy ${${replaceList}} )
      while( _replaceListCopy )

         # Extract the original and replacement strings:
         list( GET _replaceListCopy 0 _original )
         list( GET _replaceListCopy 1 _replacement )
         list( REMOVE_AT _replaceListCopy 0 1 )

         # In case we picked up something funky:
         if( "${_original}" STREQUAL "" )
            continue()
         endif()

         # If this is a valid replacement, then do so. Do not stop
         # though, as multiple replacements may need to be made in the
         # path.
         string( REPLACE "${_original}" "${_replacement}" _path ${_path} )
      endwhile()

      # Add the (possibly modified) path to the output:
      list( APPEND _result ${_path} )
   endforeach()

   # Return the list to the caller:
   set( ${pathList} ${_result} PARENT_SCOPE )

endfunction( _lcg_replace_paths )

# Helper function to evaluate if an environment setting points to some path
#
# The environment setup script tries to be smart with re-using previously
# configured environment variables in the setup of higher level environment
# variables. To do this correctly, we need to know which variables describe
# paths, and which describe something else. (Like a DB connection setting.)
#
# This function is used to select environment variables that should be
# considered to be re-used in the setup of other environment variables.
#
# Usage: _lcg_is_absolute_path( ${_name} _enviromentsCopy value )
#
function( _lcg_is_absolute_path varName environmentsListName returnValueName )

   # The default answer is no...
   set( ${returnValueName} FALSE )

   # Make a copy of the environment list:
   set( _envCopy ${${environmentsListName}} )

   # Loop over its elements:
   while( _envCopy )
      # Get the instruction and the mandatory name:
      list( GET _envCopy 0 _instruction )
      list( GET _envCopy 1 _name )
      # Handle the UNSET command in a special way:
      if( "${_instruction}" STREQUAL "UNSET" )
         list( REMOVE_AT _envCopy 0 1 )
      elseif( "${_instruction}" STREQUAL "SET" OR
            "${_instruction}" STREQUAL "FORCESET" )
         list( GET _envCopy 2 _value )
         # Check if we found the variable we're looking for:
         if( "${varName}" STREQUAL "${_name}" )
            # Check if it sets a path or not:
            if( IS_DIRECTORY "${_value}" AND
                  IS_ABSOLUTE "${_value}" )
               set( ${returnValueName} TRUE PARENT_SCOPE )
            else()
               set( ${returnValueName} FALSE PARENT_SCOPE )
            endif()
            return()
         endif()
         list( REMOVE_AT _envCopy 0 1 2 )
      else()
         list( REMOVE_AT _envCopy 0 1 2 )
      endif()
   endwhile()

endfunction( _lcg_is_absolute_path )

# Function creating environment setup files based on the found externals
#
# This function can be called at the end of configuring the build of a project,
# to produce configuration files of different types.
#
# Usage: lcg_generate_env( [SH_FILE ${CMAKE_BINARY_DIR}/setup.sh]
#                          [CSH_FILE ${CMAKE_BINARY_DIR}/setup.csh]
#                          [XENV_FILE ${CMAKE_BINARY_DIR}/setup.xenv]
#                          [REPLACE "orig1" "new1" "orig2" "new2"...] )
#
function( lcg_generate_env )

   # Parse the arguments:
   cmake_parse_arguments( ARG "" "SH_FILE;CSH_FILE;XENV_FILE"
      "REPLACE" ${ARGN} )

   # Tell the user what's happening:
   message( STATUS "Generating external environment configuration" )

   # Environment variables handled by this function:
   set( _python_paths )
   set( _binary_paths )
   set( _library_paths )
   set( _include_paths )
   set( _environments )

   # Get a list of all the packages that were found using find_package(...).
   # This is a (well hidden) CMake feature:
   get_property( _packages_found GLOBAL PROPERTY PACKAGES_FOUND )

   # Loop over these packages:
   foreach( _package ${_packages_found} )

      # Skip this package itself:
      if( "${_package}" STREQUAL "LCG" )
         continue()
      endif()

      # Also skip all the ATLAS offline and analysis packages:
      set( _atlasPackages AtlasExternals DetCommon AtlasCore AtlasConditions
         AtlasEvent AtlasReconstruction AtlasTrigger AtlasAnalysis
         AtlasSimulation AtlasOffline AtlasHLT
         AtlasProduction AtlasDerivation AtlasP1HLT AtlasPhysics AtlasCAFHLT
         AtlasP1MON AtlasProd1 AtlasProd2 AtlasTier0 AtlasTestHLT TrigMC
         AthAnalysisBase AthSimulationBase AnalysisBase )
      list( FIND _atlasPackages ${_package} _atlasPackageIndex )
      unset( _atlasPackages )
      if( NOT "${_atlasPackageIndex}" EQUAL -1 )
         continue()
      endif()
      unset( _atlasPackageIndex )

      # Skip Gaudi, as it should be put at the front of the environment paths.
      if( "${_package}" STREQUAL "Gaudi" )
         continue()
      endif()

      # The package name in upper-case:
      string( TOUPPER ${_package} _packageUpper )

      # Find the package again, to pull in its properties:
      find_package( ${_package} QUIET )

      # Get the paths of the package:
      list( APPEND _python_paths ${${_package}_PYTHON_PATH} )
      list( APPEND _binary_paths ${${_package}_BINARY_PATH} )
      list( APPEND _library_paths ${${_package}_LIBRARY_DIRS} )
      list( APPEND _include_paths ${${_package}_INCLUDE_DIRS} )
      list( APPEND _environments ${${_package}_ENVIRONMENT} )
      if( NOT _package STREQUAL _packageUpper )
         list( APPEND _python_paths ${${_packageUpper}_PYTHON_PATH} )
         list( APPEND _binary_paths ${${_packageUpper}_BINARY_PATH} )
         list( APPEND _library_paths ${${_packageUpper}_LIBRARY_DIRS} )
         list( APPEND _include_paths ${${_packageUpper}_INCLUDE_DIRS} )
         list( APPEND _environments ${${_packageUpper}_ENVIRONMENT} )
      endif()

   endforeach()

   # If Gaudi is used by the project, put it at the front of the environment
   # variables.
   list( FIND _packages_found "Gaudi" _gaudiPackageIndex )
   if( NOT "${_gaudiPackageIndex}" EQUAL -1 )
      find_package( Gaudi QUIET )
      list( INSERT _python_paths 0 ${GAUDI_PYTHON_PATH} )
      list( INSERT _binary_paths 0 ${GAUDI_BINARY_PATH} )
      list( INSERT _library_paths 0 ${GAUDI_LIBRARY_DIRS} )
      list( INSERT _include_paths 0 ${GAUDI_INCLUDE_DIRS} )
      list( APPEND _environments ${GAUDI_ENVIRONMENT} )
   endif()
   unset( _gaudiPackageIndex )

   # Remove the duplicates from the paths:
   foreach( _path _python_paths _binary_paths _library_paths _include_paths )
      if( NOT "${${_path}}" STREQUAL "" )
         list( REMOVE_DUPLICATES ${_path} )
      endif()
   endforeach()

   # Paths considered "system paths".
   set( LCG_IGNORED_SYSTEM_PATHS "(/|/usr|/usr/local)" CACHE STRING
        "Regular expression specifying which paths are system paths" )
   mark_as_advanced( LCG_IGNORED_SYSTEM_PATHS )

   # Remove system paths from these:
   set( _library_paths_copy ${_library_paths} )
   set( _library_paths )
   foreach( l ${_library_paths_copy} )
      if( NOT l MATCHES
            "^(.*BUILD_INTERFACE:|.*INSTALL_INTERFACE:)?${LCG_IGNORED_SYSTEM_PATHS}?/lib(32|64)?" )
         set( _library_paths ${_library_paths} ${l} )
      endif()
   endforeach()
   unset( _library_paths_copy )

   set( _binary_paths_copy ${_binary_paths} )
   set( _binary_paths )
   foreach( b ${_binary_paths_copy} )
      if( NOT b MATCHES
            "^(.*BUILD_INTERFACE:|.*INSTALL_INTERFACE:)?${LCG_IGNORED_SYSTEM_PATHS}?/bin(32|64)?" )
         set( _binary_paths ${_binary_paths} ${b} )
      endif()
   endforeach()
   unset( _binary_paths_copy )

   set( _include_paths_copy ${_include_paths} )
   set( _include_paths )
   foreach( b ${_include_paths_copy} )
      if( NOT b MATCHES
            "^(.*BUILD_INTERFACE:|.*INSTALL_INTERFACE:)?${LCG_IGNORED_SYSTEM_PATHS}?/include" )
         set( _include_paths ${_include_paths} ${b} )
      endif()
   endforeach()
   unset( _include_paths_copy )

   # Make a copy of the environments, which is used later on to evaluate
   # further replacements in the paths:
   set( _environmentsOrig "${_environments}" )

   # Use LCG_RELEASE_BASE in the paths if possible:
   if( LCG_RELEASE_BASE )
      _lcg_make_paths_relocatable( _python_paths )
      _lcg_make_paths_relocatable( _binary_paths )
      _lcg_make_paths_relocatable( _library_paths )
      _lcg_make_paths_relocatable( _include_paths )
      _lcg_make_paths_relocatable( _environments )
   endif()

   # Make replacements in the paths if necessary:
   if( ARG_REPLACE )
      _lcg_replace_paths( _python_paths ARG_REPLACE )
      _lcg_replace_paths( _binary_paths ARG_REPLACE )
      _lcg_replace_paths( _library_paths ARG_REPLACE )
      _lcg_replace_paths( _include_paths ARG_REPLACE )
      _lcg_replace_paths( _environments ARG_REPLACE )
   endif()

   # List of configured environment variables:
   set( _envVarNamesAndValues )

   # Process environment variables, eliminate duplicates
   set( _newEnv )
   while( _environments )
      # Get the instruction and the mandatory name:
      list( GET _environments 0 _cmd )
      list( GET _environments 1 _var )
      if( "${_cmd}" STREQUAL "APPEND" OR "${_cmd}" STREQUAL "PREPEND" )
         list( GET _environments 2 _val )
         _lcg_replace_paths( _val _envVarNamesAndValues )
         list( APPEND _newEnv "${_cmd}" "${_var}" "${_val}" )
         list( REMOVE_AT _environments 0 1 2 )
      elseif( "${_cmd}" STREQUAL "SET" OR "${_cmd}" STREQUAL "FORCESET" )
         list( GET _environments 2 _val )
         # check if we saw this variable before and only copy if not
         list( FIND _envVarNamesAndValues "\${${_var}}" _envVarListIdx )
         if( "${_envVarListIdx}" EQUAL -1 )
            # new variable
            _lcg_replace_paths( _val _envVarNamesAndValues )
            list( APPEND _newEnv "${_cmd}" "${_var}" "${_val}" )
            _lcg_is_absolute_path( ${_var} _environmentsOrig _isPath )
            if( ${_isPath} )
               list( APPEND _envVarNamesAndValues "${_val}" "\${${_var}}" )
            endif()
         else()
            # duplicated variable setting - ignore, but check if the value was the same
            MATH( EXPR _envVarListIdx  "${_envVarListIdx}-1" )
            list( GET _envVarNamesAndValues ${_envVarListIdx} _oldval )
            if( NOT _oldval STREQUAL _val )
               # warn that values are different
               message( WARNING "Duplicated setting for ENV variable ${_var}!\nfirst val='${_oldval}',  IGNORING val='${_val}'" )
            endif()
         endif()
         unset( _envVarListIdx )
         list( REMOVE_AT _environments 0 1 2 )
      elseif( "${_cmd}" STREQUAL "UNSET" )
         # just copy over this command
         list( APPEND _newEnv "${_cmd}" "${_var}" )
         list( REMOVE_AT _environments 0 1 )
      else()
         # Don't try to continue with this if we hit an issue:
         message( WARNING "Instruction ${_cmd} not known" )
         break()
      endif()
   endwhile()
   set( _environments ${_newEnv} )
   unset( _newEnv )


   # Write an SH file:
   if( ARG_SH_FILE )

      # Start the file:
      message( STATUS "Writing runtime environment to file: ${ARG_SH_FILE}" )
      file( WRITE ${ARG_SH_FILE}.in "# Generated by lcg_generate_env...\n" )

      # Set LCG_RELEASE_BASE if it's available:
      if( LCG_RELEASE_BASE )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${LCG_RELEASE_BASE}\" ]; then\n"
            "   export LCG_RELEASE_BASE=${LCG_RELEASE_BASE}\n"
            "fi\n" )
      endif()
      # Set LCG_PLATFORM if it's available:
      if( LCG_PLATFORM )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${LCG_PLATFORM}\" ]; then\n"
            "   export LCG_PLATFORM=${LCG_PLATFORM}\n"
            "fi\n" )
      endif()
      # Set the LCG_NIGHTLY variable if necessary:
      if( LCG_NIGHTLY )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${LCG_NIGHTLY}\" ]; then\n"
            "   export LCG_NIGHTLY=${LCG_NIGHTLY}\n"
            "fi\n" )
      endif()

      # Set the general environment variables:
      set( _envCopy ${_environments} )
      while( _envCopy )
         # Get the instruction and the mandatory name:
         list( GET _envCopy 0 _instruction )
         list( GET _envCopy 1 _name )
         # Now decide what to do with this instruction:
         if( "${_instruction}" STREQUAL "UNSET" )
            file( APPEND ${ARG_SH_FILE}.in
               "unset ${_name}\n" )
            list( REMOVE_AT _envCopy 0 1 )
         elseif( "${_instruction}" STREQUAL "SET" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_SH_FILE}.in
               "if [ -z \"\${${_name}}\" ]; then\n"
               "   export ${_name}=${_value}\n"
               "fi\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "FORCESET" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_SH_FILE}.in
               "export ${_name}=${_value}\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "APPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_SH_FILE}.in
               "if [ -z \"\${${_name}}\" ]; then\n"
               "   export ${_name}=${_value}\n"
               "else\n"
               "   export ${_name}=\${${_name}}:${_value}\n"
               "fi\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "PREPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_SH_FILE}.in
               "if [ -z \"\${${_name}}\" ]; then\n"
               "   export ${_name}=${_value}\n"
               "else\n"
               "   export ${_name}=${_value}:\${${_name}}\n"
               "fi\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         endif()
      endwhile()

      # Make another round of replacements in the paths:
      _lcg_replace_paths( _python_paths _envVarNamesAndValues )
      _lcg_replace_paths( _binary_paths _envVarNamesAndValues )
      _lcg_replace_paths( _library_paths _envVarNamesAndValues )
      _lcg_replace_paths( _include_paths _envVarNamesAndValues )

      # Clean up:
      unset( _envVarNamesAndValues )

      # Make the lists into lists understood by bash:
      string( REPLACE ";" ":" _python_paths_written  "${_python_paths}" )
      string( REPLACE ";" ":" _binary_paths_written  "${_binary_paths}" )
      string( REPLACE ";" ":" _library_paths_written "${_library_paths}" )
      string( REPLACE ";" ":" _include_paths_written "${_include_paths}" )

      # Set the various paths:
      if( NOT "${_python_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${PYTHONPATH}\" ]; then\n"
            "   export PYTHONPATH=${_python_paths_written}\n"
            "else\n"
            "   export PYTHONPATH=${_python_paths_written}:\${PYTHONPATH}\n"
            "fi\n" )
      endif()
      if( NOT "${_binary_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${PATH}\" ]; then\n"
            "   export PATH=${_binary_paths_written}\n"
            "else\n"
            "   export PATH=${_binary_paths_written}:\${PATH}\n"
            "fi\n" )
      endif()
      if( NOT "${_library_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${LD_LIBRARY_PATH}\" ]; then\n"
            "   export LD_LIBRARY_PATH=${_library_paths_written}\n"
            "else\n"
            "   export LD_LIBRARY_PATH=${_library_paths_written}:"
            "\${LD_LIBRARY_PATH}\n"
            "fi\n" )
      endif()
      if( APPLE AND NOT "${_library_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${DYLD_LIBRARY_PATH}\" ]; then\n"
            "   export DYLD_LIBRARY_PATH=${_library_paths_written}\n"
            "else\n"
            "   export DYLD_LIBRARY_PATH=${_library_paths_written}:"
            "\${DYLD_LIBRARY_PATH}\n"
            "fi\n" )
      endif()
      if( NOT "${_include_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_SH_FILE}.in
            "if [ -z \"\${ROOT_INCLUDE_PATH}\" ]; then\n"
            "   export ROOT_INCLUDE_PATH=${_include_paths_written}\n"
            "else\n"
            "   export ROOT_INCLUDE_PATH=${_include_paths_written}:"
            "\${ROOT_INCLUDE_PATH}\n"
            "fi\n" )
      endif()

      # Generate the proper file out of the created skeleton at the
      # generation step. (To resolve possible generator expressions
      # in the path names.)
      file( GENERATE OUTPUT ${ARG_SH_FILE} INPUT ${ARG_SH_FILE}.in )

   endif()

   # Write a CSH file:
   if( ARG_CSH_FILE )

      # Start the file:
      message( STATUS "Writing runtime environment to file: ${ARG_CSH_FILE}" )
      file( WRITE ${ARG_CSH_FILE}.in "# Generated by lcg_generate_env...\n" )

      # Set LCG_RELEASE_BASE if it's available:
      if( LCG_RELEASE_BASE )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?LCG_RELEASE_BASE ) then\n"
            "   setenv LCG_RELEASE_BASE ${LCG_RELEASE_BASE}\n"
            "endif\n" )
      endif()
      # Set LCG_PLATFORM if it's available:
      if( LCG_PLATFORM )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?LCG_PLATFORM ) then\n"
            "   setenv LCG_PLATFORM ${LCG_PLATFORM}\n"
            "endif\n" )
      endif()
      # Set LCG_NIGHTLY if necessary:
      if( LCG_NIGHTLY )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?LCG_NIGHTLY ) then\n"
            "   setenv LCG_NIGHTLY ${LCG_NIGHTLY}\n"
            "endif\n" )
      endif()

      # Set the general environment variables:
      set( _envCopy ${_environments} )
      while( _envCopy )
         # Get the instruction and the mandatory name:
         list( GET _envCopy 0 _instruction )
         list( GET _envCopy 1 _name )
         # Now decide what to do with this instruction:
         if( "${_instruction}" STREQUAL "UNSET" )
            file( APPEND ${ARG_CSH_FILE}.in
               "unset ${_name}\n" )
            list( REMOVE_AT _envCopy 0 1 )
         elseif( "${_instruction}" STREQUAL "SET" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_CSH_FILE}.in
               "if (! \$?${_name} ) then\n"
               "   setenv ${_name} ${_value}\n"
               "endif\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "FORCESET" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_CSH_FILE}.in
               "setenv ${_name} ${_value}\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "APPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_CSH_FILE}.in
               "if (! \$?${_name} ) then\n"
               "   setenv ${_name} ${_value}\n"
               "else\n"
               "   setenv ${_name} \$${_name}:${_value}\n"
               "endif\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "PREPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_CSH_FILE}.in
               "if (! \$?${_name} ) then\n"
               "   setenv ${_name} ${_value}\n"
               "else\n"
               "   setenv ${_name} ${_value}:\$${_name}\n"
               "endif\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         endif()
      endwhile()

      # Make another round of replacements in the paths:
      _lcg_replace_paths( _python_paths _envVarNamesAndValues )
      _lcg_replace_paths( _binary_paths _envVarNamesAndValues )
      _lcg_replace_paths( _library_paths _envVarNamesAndValues )
      _lcg_replace_paths( _include_paths _envVarNamesAndValues )

      # Make the lists into lists understood by tcsh:
      string( REPLACE ";" ":" _python_paths_written  "${_python_paths}" )
      string( REPLACE ";" ":" _binary_paths_written  "${_binary_paths}" )
      string( REPLACE ";" ":" _library_paths_written "${_library_paths}" )
      string( REPLACE ";" ":" _include_paths_written "${_include_paths}" )

      # Set the various paths:
      if( NOT "${_python_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?PYTHONPATH ) then\n"
            "   setenv PYTHONPATH ${_python_paths_written}\n"
            "else\n"
            "   setenv PYTHONPATH ${_python_paths_written}:\$PYTHONPATH\n"
            "endif\n" )
      endif()
      if( NOT "${_binary_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?PATH ) then\n"
            "   setenv PATH ${_binary_paths_written}\n"
            "else\n"
            "   setenv PATH ${_binary_paths_written}:\$PATH\n"
            "endif\n" )
      endif()
      if( NOT "${_library_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?LD_LIBRARY_PATH ) then\n"
            "   setenv LD_LIBRARY_PATH ${_library_paths_written}\n"
            "else\n"
            "   setenv LD_LIBRARY_PATH ${_library_paths_written}:"
            "\$LD_LIBRARY_PATH\n"
            "endif\n" )
      endif()
      if( APPLE AND NOT "${_library_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?DYLD_LIBRARY_PATH ) then\n"
            "   setenv DYLD_LIBRARY_PATH ${_library_paths_written}\n"
            "else\n"
            "   setenv DYLD_LIBRARY_PATH ${_library_paths_written}:"
            "\$DYLD_LIBRARY_PATH\n"
            "endif\n" )
      endif()
      if( NOT "${_include_paths_written}" STREQUAL "" )
         file( APPEND ${ARG_CSH_FILE}.in
            "if (! \$?ROOT_INCLUDE_PATH ) then\n"
            "   setenv ROOT_INCLUDE_PATH ${_include_paths_written}\n"
            "else\n"
            "   setenv ROOT_INCLUDE_PATH ${_include_paths_written}:"
            "\$ROOT_INCLUDE_PATH\n"
            "endif\n" )
      endif()

      # Generate the proper file out of the created skeleton at the
      # generation step. (To resolve possible generator expressions
      # in the path names.)
      file( GENERATE OUTPUT ${ARG_CSH_FILE} INPUT ${ARG_CSH_FILE}.in )

   endif()

   # Write an XENV file:
   if( ARG_XENV_FILE )

      # Tell the user what's happening:
      message( STATUS "Writing runtime environment to file: ${ARG_XENV_FILE}" )

      # Write the file's header:
      file( WRITE ${ARG_XENV_FILE}.in
         "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
         "<env:config xmlns:env=\"EnvSchema\" "
         "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
         "xsi:schemaLocation=\"EnvSchema EnvSchema.xsd\">\n" )

      # Set LCG_RELEASE_BASE if it's available:
      if( LCG_RELEASE_BASE )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:default variable=\"LCG_RELEASE_BASE\">"
            "${LCG_RELEASE_BASE}</env:default>\n" )
      endif()
      # Set LCG_PLATFORM if it's available:
      if( LCG_PLATFORM )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:default variable=\"LCG_PLATFORM\">"
            "${LCG_PLATFORM}</env:default>\n" )
      endif()
      # Set LCG_NIGHTLY if necessary:
      if( LCG_NIGHTLY )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:default variable=\"LCG_NIGHTLY\">"
            "${LCG_NIGHTLY}</env:default>\n" )
      endif()

      # Set the general environment variables:
      set( _envCopy ${_environments} )
      while( _envCopy )
         # Get the instruction and the mandatory name:
         list( GET _envCopy 0 _instruction )
         list( GET _envCopy 1 _name )
         # Now decide what to do with this instruction:
         if( "${_instruction}" STREQUAL "UNSET" )
            file( APPEND ${ARG_XENV_FILE}.in
               "   <env:unset variable=\"${_name}\"/>\n" )
            list( REMOVE_AT _envCopy 0 1 )
         elseif( "${_instruction}" STREQUAL "SET" OR
               "${_instruction}" STREQUAL "FORCESET" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_XENV_FILE}.in
               "   <env:set variable=\"${_name}\">${_value}</env:set>\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "APPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_XENV_FILE}.in
               "   <env:append variable=\"${_name}\">${_value}</env:append>\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         elseif( "${_instruction}" STREQUAL "PREPEND" )
            list( GET _envCopy 2 _value )
            file( APPEND ${ARG_XENV_FILE}.in
               "   <env:prepend variable=\"${_name}\">${_value}"
               "</env:prepend>\n" )
            list( REMOVE_AT _envCopy 0 1 2 )
         endif()
      endwhile()

      # Set the python paths:
      foreach( _path ${_python_paths} )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:prepend variable=\"PYTHONPATH\">${_path}</env:prepend>\n" )
      endforeach()
      # Set the binary paths:
      foreach( _path ${_binary_paths} )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:prepend variable=\"PATH\">${_path}</env:prepend>\n" )
      endforeach()
      # Set the library paths:
      foreach( _path ${_library_paths} )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:prepend variable=\"LD_LIBRARY_PATH\">${_path}"
            "</env:prepend>\n" )
         if( APPLE )
            file( APPEND ${ARG_XENV_FILE}.in
               "   <env:prepend variable=\"DYLD_LIBRARY_PATH\">${_path}"
               "</env:prepend>\n" )
         endif()
      endforeach()
      # Set the include paths:
      foreach( _path ${_include_paths} )
         file( APPEND ${ARG_XENV_FILE}.in
            "   <env:prepend variable=\"ROOT_INCLUDE_PATH\">${_path}"
            "</env:prepend>\n" )
      endforeach()

      # Write the file's footer:
      file( APPEND ${ARG_XENV_FILE}.in "</env:config>\n" )

      # Generate the proper file out of the created skeleton at the
      # generation step. (To resolve possible generator expressions
      # in the path names.)
      file( GENERATE OUTPUT ${ARG_XENV_FILE} INPUT ${ARG_XENV_FILE}.in )

   endif()

endfunction( lcg_generate_env )

# Macro used by some modules to "wrap" built-in CMake modules
#
# CMake has built-in Find-modules for a bunch of externals provided by LCG.
# In order to generate CMake descriptions of the shared libraries build by
# CMake, we however need to massage the results generated by those modules, to
# make the code work with LCG being picked up from a different location than
# on the build machine.
#
# This macro generalises this Find-module-wrapping, to avoid writing the same
# sort of code too many times.
#
macro( lcg_wrap_find_module name )

   # Parse the arguments:
   cmake_parse_arguments( ARG "NO_LIBRARY_DIRS" "" "IMPORTED_TARGETS" ${ARGN} )

   # The name in all-caps:
   string( TOUPPER ${name} _nameUpper )

   # If the module was already found, set it to silent mode:
   if( ${name}_FOUND OR ${_nameUpper}_FOUND )
      set( ${name}_FIND_QUIETLY TRUE )
   endif()

   # If we are to take the external from an LCG release, then hide the locally
   # installed stuff from the code:
   if( ${name}_LCGROOT OR ${_nameUpper}_LCGROOT )
      set( _ignorePathBackup ${CMAKE_SYSTEM_IGNORE_PATH} )
      set( CMAKE_SYSTEM_IGNORE_PATH /usr/include /usr/bin /usr/bin32 /usr/bin64
         /usr/lib /usr/lib32 /usr/lib64 /bin /bin32 /bin64 /lib /lib32 /lib64 )
   endif()

   # Set up the LCG_SYSTEM_IGNORE_PATH cache variable.
   lcg_system_ignore_path_setup()

   # Temporarily clean out CMAKE_MODULE_PATH, so that we could pick up
   # the built-in Find-module from CMake:
   set( _modulePathBackup ${CMAKE_MODULE_PATH} )
   set( CMAKE_MODULE_PATH )

   # Decide whether to add QUIET to the find_package(...) call. When the
   # underlying package is picked up using its own CMake configuration files,
   # the variable set by the "parent call" is not enough to silence a warning
   # about a possible missing package.
   set( _quietFlag )
   if( ${name}_FIND_QUIETLY )
      set( _quietFlag QUIET )
   endif()

   # Call CMake's own Find-module. Note that the arguments created for the
   # wrapper script by CMake are passed through to the official script like
   # this. There's no need to add extra arguments to this call.
   find_package( ${name} ${_quietFlag} )

   # Restore all paths:
   set( CMAKE_MODULE_PATH ${_modulePathBackup} )
   unset( _modulePathBackup )
   if( ${name}_LCGROOT OR ${_nameUpper}_LCGROOT )
      set( CMAKE_SYSTEM_IGNORE_PATH ${_ignorePathBackup} )
      unset( _ignorePathBackup )
   endif()
   unset( _quietFlag )

   # If the wrapped module only set the "lower case found name", make sure that
   # an upper case name is set as well. Otherwise lcg_need_rpm(...) could get
   # confused.
   if( ${name}_FOUND AND NOT ${_nameUpper}_FOUND )
      set( ${_nameUpper}_FOUND ${${name}_FOUND} )
   endif()

   # Make the paths relocatable. But only if we're not building Gaudi. As its
   # CMake code can't deal correctly with these sort of relocatable paths.
   if( ${name}_FOUND AND NOT GAUDI_ATLAS )

      # Massage the include directories first:
      set( _newIncludes )
      set( _allIncludes ${${name}_INCLUDE_DIR} ${${name}_INCLUDE_DIRS}
         ${${_nameUpper}_INCLUDE_DIR} ${${_nameUpper}_INCLUDE_DIRS} )
      foreach( _target ${ARG_IMPORTED_TARGETS} )
         # Check if there are any include directories associated with the
         # imported target.
         get_property( _incAvailable TARGET ${_target}
            PROPERTY INTERFACE_INCLUDE_DIRECTORIES
            SET )
         if( NOT "${_incAvailable}" )
            continue()
         endif()
         unset( _incAvailable )
         # If there are, add them to the "all includes" list.
         get_target_property( _inc ${_target} INTERFACE_INCLUDE_DIRECTORIES )
         list( APPEND _allIncludes ${_inc} )
         unset( _inc )
      endforeach()
      if( _allIncludes )
         list( REMOVE_DUPLICATES _allIncludes )
      endif()
      foreach( _inc ${_allIncludes} )
         if( "${_inc}" MATCHES "BUILD_INTERFACE" OR
               "${_inc}" MATCHES "INSTALL_INTERFACE" )
            list( APPEND _newIncludes ${_inc} )
            continue()
         endif()
         set( _relocatableDir ${_inc} )
         _lcg_make_paths_relocatable( _relocatableDir )
         list( APPEND _newIncludes
            $<BUILD_INTERFACE:${_inc}>
            $<INSTALL_INTERFACE:${_relocatableDir}> )
         unset( _relocatableDir )
      endforeach()
      set( ${name}_INCLUDE_DIRS ${_newIncludes} )
      set( ${_nameUpper}_INCLUDE_DIRS ${_newIncludes} )
      unset( _newIncludes )
      unset( _allIncludes )

      # Get physical library names from the imported (library) targets.
      set( _importedLibs )
      foreach( _target ${ARG_IMPORTED_TARGETS} )
         # Check if there are any (library) files associated with the target.
         get_property( _configAvailable TARGET ${_target}
            PROPERTY IMPORTED_CONFIGURATIONS
            SET )
         if( NOT "${_configAvailable}" )
            continue()
         endif()
         unset( _configAvailable )
         # If there are, extract each of their physical locations.
         get_target_property( _configs ${_target} IMPORTED_CONFIGURATIONS )
         foreach( _config ${_configs} )
            get_target_property( _lib ${_target} IMPORTED_LOCATION_${_config} )
            list( APPEND _importedLibs "${_lib}" )
            unset( _lib )
         endforeach()
         unset( _configs )
      endforeach()

      # Massage the library directories variable:
      if( ARG_NO_LIBRARY_DIRS )
         # If the wrapped module doesn't provide a LIBRARY_DIR or LIBRARY_DIRS
         # variable, then let's just use the paths of the found
         # library/libraries.
         set( ${name}_LIBRARY_DIRS )
         foreach( _lib ${${name}_LIBRARIES} ${${_nameUpper}_LIBRARIES}
            ${_importedLibs} )
            get_filename_component( _libDir ${_lib} PATH )
            if( "${_libDir}" MATCHES "BUILD_INTERFACE" OR
                  "${_libDir}" MATCHES "INSTALL_INTERFACE" )
               # This is a very weird one... If find_package(...) was called
               # "at the same level" multiple times, on subsequent calls the
               # library names will have been made relocatable already. In
               # which case get_filename_component(...) would've resulted in
               # a string that begins with a generator expression, but does
               # not end in ">". The following hack tries to go around this...
               list( APPEND ${name}_LIBRARY_DIRS "${_libDir}>" )
            else()
               set( _relocatableDir ${_libDir} )
               _lcg_make_paths_relocatable( _relocatableDir )
               list( APPEND ${name}_LIBRARY_DIRS
                  $<BUILD_INTERFACE:${_libDir}>
                  $<INSTALL_INTERFACE:${_relocatableDir}> )
            endif()
            unset( _libDir )
            unset( _relocarableDir )
         endforeach()
         set( ${_nameUpper}_LIBRARY_DIRS ${${name}_LIBRARY_DIRS} )
      else()
         # If the wrapped module provides a LIBRARY_DIR or LIBRARY_DIRS
         # variable, then just make that relocatable.
         set( _newLibDirs )
         set( _allLibDirs ${${name}_LIBRARY_DIR} ${${name}_LIBRARY_DIRS}
            ${${_nameUpper}_LIBRARY_DIR} ${${_nameUpper}_LIBRARY_DIRS} )
         if( _allLibDirs )
            list( REMOVE_DUPLICATES _allLibDirs )
         endif()
         foreach( _libDir ${_allLibDirs} )
            if( "${_libDir}" MATCHES "BUILD_INTERFACE" OR
                  "${_libDir}" MATCHES "INSTALL_INTERFACE" )
               list( APPEND _newLibDirs ${_libDir} )
               continue()
            endif()
            set( _relocatableDir ${_libDir} )
            _lcg_make_paths_relocatable( _relocatableDir )
            list( APPEND _newLibDirs
               $<BUILD_INTERFACE:${_libDir}>
               $<INSTALL_INTERFACE:${_relocatableDir}> )
            unset( _relocatableDir )
         endforeach()
         set( ${name}_LIBRARY_DIRS ${_newLibDirs} )
         set( ${_nameUpper}_LIBRARY_DIRS ${_newLibDirs} )
         unset( _newLibDirs )
         unset( _allLibDirs )
      endif()

      # Massage the library names:
      set( _newLibs )
      foreach( _lib ${${name}_LIBRARIES} ${${_nameUpper}_LIBRARIES} )
         if( "${_lib}" MATCHES "BUILD_INTERFACE" OR
               "${_lib}" MATCHES "INSTALL_INTERFACE" )
            list( APPEND _newLibs ${_lib} )
            continue()
         endif()
         set( _relocatableLib ${_lib} )
         _lcg_make_paths_relocatable( _relocatableLib )
         list( APPEND _newLibs
            $<BUILD_INTERFACE:${_lib}>
            $<INSTALL_INTERFACE:${_relocatableLib}> )
         unset( _relocatableLib )
      endforeach()
      set( ${name}_LIBRARIES ${_newLibs} )
      set( ${_nameUpper}_LIBRARIES ${_newLibs} )
      unset( _newLibs )
      list( REMOVE_DUPLICATES ${name}_LIBRARIES )
      list( REMOVE_DUPLICATES ${_nameUpper}_LIBRARIES )

      # Massage the component variables:
      foreach( _component ${${name}_FIND_COMPONENTS} )
         if( ${name}_${_component}_FOUND OR ${_nameUpper}_${_component}_FOUND )
            if( "${${name}_${_component}_LIBRARY}" MATCHES "BUILD_INTERFACE"
                  OR "${${name}_${_component}_LIBRARY}" MATCHES
                  "INSTALL_INTERFACE" )
               continue()
            endif()
            set( _relocatableLib ${${name}_${_component}_LIBRARY} )
            _lcg_make_paths_relocatable( _relocatableLib )
            set( ${name}_${_component}_LIBRARY
               $<BUILD_INTERFACE:${${name}_${_component}_LIBRARY}>
               $<INSTALL_INTERFACE:${_relocatableLib}> )
            unset( _relocatableLib )
         endif()
      endforeach()

   endif()

   # Some final cleanup:
   unset( _nameUpper )

endmacro( lcg_wrap_find_module )

# Macro setting up the LCG_SYSTEM_IGNORE_PATH cache variable
#
# The variable is used to extend the CMAKE_SYSTEM_IGNORE_PATH variable, allowing
# the user to specify extra directories that should be ignored by all LCG
# modules while looking for externals.
#
macro( lcg_system_ignore_path_setup )

   # Set up the LCG_SYSTEM_IGNORE_PATH cache variable, taking the
   # ${LCG_SYSTEM_IGNORE_PATH} shell environment variable into account.
   set( _defaultIgnore )
   if( NOT "$ENV{LCG_SYSTEM_IGNORE_PATH}" STREQUAL "" )
      string( REPLACE ":" ";" _defaultIgnore "$ENV{LCG_SYSTEM_IGNORE_PATH}" )
   endif()
   set( LCG_SYSTEM_IGNORE_PATH "${_defaultIgnore}" CACHE STRING
      "Paths ignored by the LCG modules when looking for externals" )
   unset( _defaultIgnore )
   if( NOT "${LCG_SYSTEM_IGNORE_PATH}" STREQUAL "" )
      list( APPEND CMAKE_SYSTEM_IGNORE_PATH ${LCG_SYSTEM_IGNORE_PATH} )
   endif()

endmacro( lcg_system_ignore_path_setup )
