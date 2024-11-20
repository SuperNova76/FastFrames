# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# This file collects the ATLAS CMake helper functions that are used to install
# various types of files/directories from a package.
#
# This file should not be included directly, but through AtlasFunctions.cmake.
#

# Obsolete:
function( atlas_install_headers )
    message( SEND_ERROR
       "atlas_install_headers is no longer supported, use "
       "atlas_add_library to install public header files: "
       "https://atlassoftwaredocs.web.cern.ch/guides/cmake/#atlas_install_headers-obsolete" )
endfunction( atlas_install_headers )


# This function installs the headers from one or many of the package's
# directories. For INTERNAL USE only.
#
# Usage: _atlas_install_headers( CxxUtils )
#
function( _atlas_install_headers )

   # Parse the options (header directories) given to the function:
   cmake_parse_arguments( ARG "" "" "" ${ARGN} )

   # If there are no arguments given to the function, return now:
   if( NOT ARG_UNPARSED_ARGUMENTS )
      message( WARNING "Function received no arguments" )
      return()
   endif()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Create the header installation target of the package if it doesn't exist
   # yet:
   if( NOT TARGET ${pkgName}HeaderInstall )
      add_custom_target( ${pkgName}HeaderInstall ALL SOURCES
         $<TARGET_PROPERTY:${pkgName}HeaderInstall,ROOT_HEADER_DIRS> )
      add_dependencies( Package_${pkgName} ${pkgName}HeaderInstall )
      set_property( TARGET ${pkgName}HeaderInstall PROPERTY LABELS ${pkgName} )
      set_property( TARGET ${pkgName}HeaderInstall PROPERTY
         FOLDER ${pkgDir}/Internals )
   endif()

   # Create an installation rule setting up the include directory in the
   # install area:
   install( CODE "
      set( _destdir \"\$ENV{DESTDIR}\")
      if( NOT _destdir STREQUAL \"\" )
         set( _destdir \"\${_destdir}/\" )
      endif()
      execute_process( COMMAND \${CMAKE_COMMAND}
         -E make_directory
         \${_destdir}\${CMAKE_INSTALL_PREFIX}/include )
      unset( _destdir )" )

   # Loop over the specified directories:
   foreach( dir ${ARG_UNPARSED_ARGUMENTS} )
      # Check if the directory exists:
      if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir} )
         message( SEND_ERROR "Cannot install headers from non-existing directory "
            ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
      endif()
      # Set up the installation of the header directory into the build area:
      file( RELATIVE_PATH _target ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}
         ${CMAKE_CURRENT_SOURCE_DIR}/${dir} )
      add_custom_command( OUTPUT ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${dir}
         COMMAND ${CMAKE_COMMAND} -E make_directory
         ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}
         COMMAND ${CMAKE_COMMAND} -E create_symlink ${_target}
         ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${dir} )
      # Clean up on "make clean":
      set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY
         ADDITIONAL_MAKE_CLEAN_FILES
         ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${dir} )
      # Add it to the header installation target:
      set_property( TARGET ${pkgName}HeaderInstall APPEND PROPERTY
         ROOT_HEADER_DIRS ${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${dir} )
      # Set up the installation of this folder:
      install( CODE "
         set( _destdir \"\$ENV{DESTDIR}\")
         if( NOT _destdir STREQUAL \"\" )
            set( _destdir \"\${_destdir}/\" )
         endif()
         execute_process( COMMAND \${CMAKE_COMMAND}
            -E create_symlink ../src/${pkgDir}/${dir}
            \${_destdir}\${CMAKE_INSTALL_PREFIX}/include/${dir} )
         unset( _destdir )" )
   endforeach()

endfunction( _atlas_install_headers )

# Helper macro setting up installation targets. Not for use outside of this
# file.
#
macro( _atlas_create_install_target tgtName )

   if( NOT TARGET ${tgtName} )
      add_custom_target( ${tgtName} ALL SOURCES
         $<TARGET_PROPERTY:${tgtName},INSTALLED_FILES> )
   endif()

endmacro( _atlas_create_install_target )


# Helper function to create a POST_BUILD_CMD that is invoked by TARGET.
#
function( _atlas_create_post_build_cmd )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "" "TARGET" "POST_BUILD_CMD" ${ARGN} )

   # Produce absolute file names, in case we got relative ones. (Assume for any
   # relative path name that it's meant to be inside the source directory.)
   set( _files ${ARG_UNPARSED_ARGUMENTS} )
   set( _absFileNames )
   foreach( _file ${_files} )
      if( IS_ABSOLUTE "${_file}" )
         list( APPEND _absFileNames "${_file}" )
      else()
         list( APPEND _absFileNames "${CMAKE_CURRENT_SOURCE_DIR}/${_file}" )
      endif()
   endforeach()

   # Run post build command:
   list( GET ARG_POST_BUILD_CMD 0 _executable )
   get_filename_component( _prog "${_executable}" PROGRAM )
   if( EXISTS "${_prog}" )
      # If any source file changed, we run the post-build command on all files:
      set( _stampFile
         "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${ARG_TARGET}PostBuild.stamp" )
      add_custom_command( OUTPUT "${_stampFile}"
         COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
         ${ARG_POST_BUILD_CMD} -- ${_files}
         COMMAND ${CMAKE_COMMAND} -E touch "${_stampFile}"
         DEPENDS ${_absFileNames}
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         COMMENT "Running ${ARG_TARGET} post-command"
         VERBATIM )
      # Make the given target call this custom command:
      set_property( TARGET ${ARG_TARGET} APPEND PROPERTY
         INSTALLED_FILES ${_stampFile} )
      # Clean up.
      unset( _stampFile )
   else()
      message( WARNING
         "Could not locate POST_BUILD_CMD: ${ARG_POST_BUILD_CMD}" )
   endif()

   # Clean up.
   unset( _files )
   unset( _absFileNames )

endfunction( _atlas_create_post_build_cmd )


# This is a generic function for installing practically any type of file
# from a package into both the build and the install areas. Behind the scenes
# it is used by most of the functions of this file.
#
# Usage: atlas_install_generic( dir/file1 dir/dir2...
#                               DESTINATION dir
#                               [BUILD_DESTINATION dir]
#                               [TYPENAME type]
#                               [EXECUTABLE]
#                               [PKGNAME_SUBDIR]
#                               [POST_BUILD_CMD command...] )
#
function( atlas_install_generic )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "EXECUTABLE;PKGNAME_SUBDIR"
      "TYPENAME;DESTINATION;BUILD_DESTINATION" "POST_BUILD_CMD" ${ARGN} )

   # If there are no file/directory names given to the function, return now:
   if( NOT ARG_UNPARSED_ARGUMENTS )
      message( WARNING "Function received no file/directory arguments" )
      return()
   endif()
   if( NOT ARG_DESTINATION )
      message( WARNING "No destination was specified" )
      return()
   endif()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Create an installation target for the package, for this type:
   set( _type "Generic" )
   if( ARG_TYPENAME )
      set( _type ${ARG_TYPENAME} )
   endif()
   set( _tgtName ${pkgName}${_type}Install )

   _atlas_create_install_target( ${_tgtName} )
   add_dependencies( Package_${pkgName} ${_tgtName} )
   set_property( TARGET ${_tgtName} PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${_tgtName} PROPERTY FOLDER ${pkgDir}/Internals )

   # Expand possible wildcards:
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   file( GLOB _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
      ${ARG_UNPARSED_ARGUMENTS} )
   unset( _extraFlags )

   if( NOT _files )
      message( SEND_ERROR "Installation of '${ARG_UNPARSED_ARGUMENTS}' "
         "does not match any files." )
      return()
   endif()

   atlas_group_source_files( ${_files} )

   # Decide what the build area destination should be:
   if( ARG_BUILD_DESTINATION )
      if( ARG_PKGNAME_SUBDIR )
         set( _buildDest ${ARG_BUILD_DESTINATION}/${pkgName} )
      else()
         set( _buildDest ${ARG_BUILD_DESTINATION} )
      endif()
   else()
      if( ARG_PKGNAME_SUBDIR )
         set( _buildDest
            ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/${ARG_DESTINATION}/${pkgName} )
      else()
         set( _buildDest
            ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/${ARG_DESTINATION} )
      endif()
   endif()

   # Decide what the installation area destination should be:
   if( ARG_PKGNAME_SUBDIR )
      set( _installDest ${ARG_DESTINATION}/${pkgName} )
   else()
      set( _installDest ${ARG_DESTINATION} )
   endif()

   # Now loop over all file names:
   foreach( _file ${_files} )
      # Set up its installation into the build area:
      file( RELATIVE_PATH _target
         ${_buildDest} ${CMAKE_CURRENT_SOURCE_DIR}/${_file} )
      get_filename_component( _filename ${_file} NAME )
      add_custom_command( OUTPUT ${_buildDest}/${_filename}
         COMMAND ${CMAKE_COMMAND} -E make_directory ${_buildDest}
         COMMAND ${CMAKE_COMMAND} -E create_symlink ${_target}
         ${_buildDest}/${_filename} )
      set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY
         ADDITIONAL_MAKE_CLEAN_FILES
         ${_buildDest}/${_filename} )
      # Add it to the installation target:
      set_property( TARGET ${_tgtName} APPEND PROPERTY
         INSTALLED_FILES ${_buildDest}/${_filename} )
      # Set up its installation into the install area:
      if( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_file} )
         install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
            DESTINATION ${_installDest}
            USE_SOURCE_PERMISSIONS
            PATTERN ".svn" EXCLUDE )
      else()
         # In case this turns out to be a symbolic link, install the actual
         # file that it points to, using the name of the symlink.
         get_filename_component( _realpath
            ${CMAKE_CURRENT_SOURCE_DIR}/${_file} REALPATH )
         if( ARG_EXECUTABLE )
            install( PROGRAMS ${_realpath}
               DESTINATION ${_installDest}
               RENAME ${_filename} )
         else()
            install( FILES ${_realpath}
               DESTINATION ${_installDest}
               RENAME ${_filename} )
         endif()
         # Only add real files as a source of the target. (So that they would
         # show up in an IDE.)
         set_property( TARGET ${_tgtName} APPEND PROPERTY
            SOURCES ${_file} )
      endif()
   endforeach()

   # Run post build command.
   if( ARG_POST_BUILD_CMD )
      _atlas_create_post_build_cmd( ${_files}
         POST_BUILD_CMD ${ARG_POST_BUILD_CMD}
         TARGET ${_tgtName} )
   endif()

endfunction( atlas_install_generic )

# Internal function used to find the first directory name inside of a path.
# So, let's say return "foo" for the "foo/bar/file.txt" path.
#
# Returns the full path if there aren't multiple components in the path.
#
# Usage: _atlas_find_first_dir_in_path( ${fileName} firstDir )
#
function( _atlas_find_first_dir_in_path path firstdirName )

   # Look for the first directory name with a loop:
   set( _firstdir "${path}" )
   set( _currentdir "${path}" )
   while( NOT "${_currentdir}" STREQUAL "" )
      set( _firstdir "${_currentdir}" )
      get_filename_component( _currentdir "${_firstdir}" PATH )
   endwhile()

   # Return the result:
   set( ${firstdirName} "${_firstdir}" PARENT_SCOPE )

endfunction()

# This function installs python modules from the package into the
# right place in both the build and the install directories.
#
# POST_BUILD_CMD can be used to specify a custom script (incl. arguments)
# executed after the bytecode compilation. The list of python files is appended
# to the given command string.
#
# By default all installed python modules will be attempted to be compiled
# during the build. A failure to compile a python file (due to syntax errors)
# will result in a build failure. This behaviour can be changed using the
# ATLAS_ALLOW_PYTHON_ERRORS global project option. If it is set to TRUE,
# the build will still print messages about python syntax errors, but won't
# stop because of them.
#
# Usage: atlas_install_python_modules( python/*.py
#                                      [POST_BUILD_CMD command...] )
#
function( atlas_install_python_modules )

   # Parse the arguments given to the function.
   cmake_parse_arguments( ARG "" "" "POST_BUILD_CMD" ${ARGN} )
   set( _args ${ARG_UNPARSED_ARGUMENTS} )

   # Call the generic function:
   # (we handle the POST_BUILD_CMD ourselves below)
   atlas_install_generic( ${_args}
      DESTINATION ${CMAKE_INSTALL_PYTHONDIR}
      BUILD_DESTINATION ${CMAKE_PYTHON_OUTPUT_DIRECTORY}
      TYPENAME Python
      PKGNAME_SUBDIR )

   # Compile the python code. To check if it has any syntax errors.

   # Find python (since this is slow, only do it if needed):
   if ( NOT Python_EXECUTABLE )
      find_package( Python COMPONENTS Interpreter QUIET )
      if( NOT Python_FOUND )
         # This is very weird, but okay...
         return()
      endif()
   endif()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package's directory:
   atlas_get_package_dir( pkgDir )

   # As a first thing, let's get a list of the files/directories specified:
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   file( GLOB _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
      ${_args} )
   unset( _extraFlags )

   # Option for making the errors found by the module compilation non-fatal to
   # the build.
   option( ATLAS_ALLOW_PYTHON_ERRORS
      "Do not stop the build when encountering Python syntax errors" FALSE )

   # Collect the python source file names:
   set( _sources )
   foreach( _file ${_files} )

      # Decide if it's a directory, or a simple file:
      if( IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_file}" )
         set( _extraFlags )
         if( ATLAS_ALWAYS_CHECK_WILDCARDS )
            list( APPEND _extraFlags CONFIGURE_DEPENDS )
         endif()
         file( GLOB_RECURSE _pyFiles ${_extraFlags}
            "${CMAKE_CURRENT_SOURCE_DIR}/${_file}/*.py" )
         unset( _extraFlags )
         list( APPEND _sources ${_pyFiles} )
      else()
         list( APPEND _sources "${CMAKE_CURRENT_SOURCE_DIR}/${_file}" )
      endif()

   endforeach()

   # Find script to compile python sources and set arguments:
   find_program( _pyCompile pyCompile.py
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _pyCompile )

   set( _compileaArgs )
   if( ATLAS_ALLOW_PYTHON_ERRORS )
      set( _compileArgs "--exit-zero" )
   endif()

   # Create a target for module compilation. We always compile all sources
   # as this is much faster than compiling each source file individually:
   set( _tgtName ${pkgName}PyCompile )
   add_dependencies( Package_${pkgName} ${_tgtName} )

   set( _stampFile
      "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/pyCompile.stamp" )
   add_custom_command( OUTPUT "${_stampFile}"
      COMMAND "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh"
      "${_pyCompile}" ${_compileArgs} ${_sources}
      COMMAND ${CMAKE_COMMAND} -E touch "${_stampFile}"
      DEPENDS "${_sources}"
      COMMENT "Compiling python modules"
      VERBATIM )

   add_custom_target( ${_tgtName} ALL DEPENDS ${_stampFile} )
   set_property( TARGET ${_tgtName} PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${_tgtName} PROPERTY FOLDER ${pkgDir}/Internals )

   # Handle POST_BUILD_CMD
   # First decide which checker to run (default or user defined):
   set( _postBuildCmd )
   if( ARG_POST_BUILD_CMD )
      set( _postBuildCmd ${ARG_POST_BUILD_CMD} )
   elseif( ${ATLAS_PYTHON_CHECKER_FOUND} )
      set( _postBuildCmd ${ATLAS_PYTHON_CHECKER} )
   endif()

   # Run the command on the list of sources from above
   if( _postBuildCmd )
      _atlas_create_post_build_cmd( ${_sources}
         POST_BUILD_CMD ${_postBuildCmd}
         TARGET ${pkgName}PythonInstall )
   endif()

endfunction( atlas_install_python_modules )

# This function installs data files from the packages into the data directory
# of the build/install area. It can be given wildcarded file names, in which
# case the code will try to look up all the specified files.
#
# Usage: atlas_install_data( data/*.root )
#
function( atlas_install_data )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_DATADIR}
      BUILD_DESTINATION ${CMAKE_DATA_OUTPUT_DIRECTORY}
      TYPENAME Data
      PKGNAME_SUBDIR )

endfunction( atlas_install_data )

# This function installs jobOptions from the package into the
# right place in both the build and the install directories.
#
# Usage: atlas_install_joboptions( share/*.py )
#
function( atlas_install_joboptions )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_JOBOPTDIR}
      BUILD_DESTINATION ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}
      TYPENAME JobOpt
      PKGNAME_SUBDIR )

endfunction( atlas_install_joboptions )

# This function installs documentation files from the package into the right
# place in both the build and install directories.
#
# Usage: atlas_install_docs( doc/*.html )
#
function( atlas_install_docs )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_DOCDIR}
      BUILD_DESTINATION ${CMAKE_DOC_OUTPUT_DIRECTORY}
      TYPENAME Doc
      PKGNAME_SUBDIR )

endfunction( atlas_install_docs )

# Function installing "executable files" during the build.
#
# Usage: atlas_install_runtime( share/*.py test/someFile.xml )
#
function( atlas_install_runtime )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_SHAREDIR}
      BUILD_DESTINATION ${CMAKE_SHARE_OUTPUT_DIRECTORY}
      TYPENAME Runtime
      EXECUTABLE )

endfunction( atlas_install_runtime )

# This function installs XML files from the package into the
# right place in both the build and the install directories.
#
# Usage: atlas_install_xmls( share/*.xml )
#
function( atlas_install_xmls )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_XMLDIR}
      BUILD_DESTINATION ${CMAKE_XML_OUTPUT_DIRECTORY}
      TYPENAME Xml
      PKGNAME_SUBDIR )

endfunction( atlas_install_xmls )

# This function installs executable script files from the package into the
# right place in both the build and the install directories.
#
# Usage: atlas_install_scripts( scripts/*.py )
#
function( atlas_install_scripts )

   # Call the generic function:
   atlas_install_generic( ${ARGN}
      DESTINATION ${CMAKE_INSTALL_BINDIR}
      BUILD_DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
      TYPENAME Scripts
      EXECUTABLE )

endfunction( atlas_install_scripts )

# Hide the private code from the outside:
unset( _atlas_create_install_target )
