# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# This file collects the ATLAS CMake helper functions that set up the build and
# installation of ROOT dictionaries in an offline or analysis build.
#
# This file should not be included directly, but through AtlasFunctions.cmake.
#

# The list manipulation generator expressions need CMake 3.15+.
cmake_minimum_required( VERSION 3.15 )

# Policy setting(s).
if( POLICY CMP0116 )
   cmake_policy( SET CMP0116 NEW )
endif()

# Function generating a Reflex dictionary library that is linked against the
# library holding the object code of the types described by the library.
#
# The OPTIONS argument is just ignored for now. As none of its uses seems
# legitim in the code at the moment...
#
# Usage: atlas_add_dictionary( LibName Lib/LibDict.h Lib/selection.xml
#                              [INCLUDE_DIRS Include1...]
#                              [LINK_LIBRARIES Library1 Library2...]
#                              [EXTRA_FILES Root/dict/*.cxx]
#                              [NAVIGABLES type1...]
#                              [DATA_LINKS type2...]
#                              [ELEMENT_LINKS type3...]
#                              [ELEMENT_LINK_VECTORS type4...]
#                              [NO_ROOTMAP_MERGE]
#                              [OPTIONS option1 option2...] )
#
function( atlas_add_dictionary libName libHeader libSelection )

   # A convenience variable:
   set( _argDefs INCLUDE_DIRS LINK_LIBRARIES EXTRA_FILES NAVIGABLES DATA_LINKS
      ELEMENT_LINKS ELEMENT_LINK_VECTORS OPTIONS )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "NO_ROOTMAP_MERGE" "" "${_argDefs}" ${ARGN} )

   # Warn the users about the ELEMENT_LINK_VECTORS arguments:
   if( ARG_ELEMENT_LINK_VECTORS )
      message( WARNING "ELEMENT_LINK_VECTORS is depreacted. "
         "Please use ELEMENT_LINKS instead." )
   endif()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # In the following let's create a file that gets updated every time any of
   # the arguments of this function is changed.

   # As a first step, let's generate a file with the values of all of the
   # arguments that this function received.
   set( _tempStampFile
      "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}.stamp.tmp" )
   file( WRITE ${_tempStampFile} "${ARG_INCLUDE_DIRS}" "${ARG_LINK_LIBRARIES}"
      "${ARG_EXTRA_FILES}" "${ARG_NAVIGABLES}" "${ARG_DATA_LINKS}"
      "${ARG_ELEMENT_LINKS}" "${ARG_ELEMENT_LINK_VECTORS}" "${ARG_OPTIONS}" )

   # Check if this is a different list than what we had built before.
   set( _stampFile
      "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}.stamp.txt" )
   if( EXISTS ${_stampFile} )
      # Check if the content of the two files is the same.
      execute_process( COMMAND ${CMAKE_COMMAND} -E compare_files
         ${_tempStampFile} ${_stampFile}
         RESULT_VARIABLE _fileCompResult
         OUTPUT_QUIET ERROR_QUIET )
      if( NOT ${_fileCompResult} EQUAL 0 )
         # The files differ, overwrite the file with the new one.
         execute_process( COMMAND ${CMAKE_COMMAND} -E rename
            ${_tempStampFile} ${_stampFile} )
      else()
         # The files are the same, just remove the new file.
         file( REMOVE ${_tempStampFile} )
      endif()
      # Clean up:
      unset( _fileCompResult )
   else()
      # Apparently there was no previous configuration/build, so let's make this
      # simple...
      execute_process( COMMAND ${CMAKE_COMMAND} -E rename
         ${_tempStampFile} ${_stampFile} )
   endif()

   # Clean up:
   unset( _tempStampFile )

   # Set up a dummy custom command that "generates" this stamp file. Just to
   # make Ninja happy. Note that this file actually gets created during the
   # CMake configuration. But CMake's Ninja generator is not clever enough to
   # figure out that we're referring to already existing files here...
   add_custom_command( OUTPUT "${_stampFile}"
      COMMAND ${CMAKE_COMMAND} -E true )

   # Set common compiler options:
   atlas_set_compiler_flags()

   # Header file and selection file components:
   set( _headerComponents )
   set( _selectionComponents )

   # Find the skeleton files:
   find_file( _dataLinkHeader DataLinkDict.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _dataLinkSelection DataLink_selection.xml.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _elementLinkHeader ElementLinkDict.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _elementLinkSelection ElementLink_selection.xml.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _elementLinkVectorHeader ElementLinkVectorDict.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _elementLinkVectorSelection ElementLinkVector_selection.xml.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _navigableHeader NavigableDict.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _navigableSelection Navigable_selection.xml.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _dataLinkHeader _dataLinkSelection _elementLinkHeader
      _elementLinkSelection _elementLinkVectorHeader _elementLinkVectorSelection
      _navigableHeader _navigableSelection )

   # Find the merge commands:
   find_program( _mergeFilesCmd mergeFiles.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   find_program( _mergeSelectionsCmd mergeSelections.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _mergeFilesCmd _mergeSelectionsCmd )

   # If the user asked for additional DataLink<T> types to generate dictionaries
   # for, set these up now. Note that types listed as navigables, are also used
   # here.
   foreach( type ${ARG_DATA_LINKS} ${ARG_NAVIGABLES} )
      # Sanitise the type name:
      string( REPLACE ":" "_" typeSanitised ${type} )
      string( REPLACE "<" "_" typeSanitised ${typeSanitised} )
      string( REPLACE ">" "_" typeSanitised ${typeSanitised} )
      string( REPLACE " " "_" typeSanitised ${typeSanitised} )
      # Generate the header file:
      configure_file( ${_dataLinkHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}.h @ONLY )
      # Generate the selection file:
      configure_file( ${_dataLinkSelection}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}_selection.xml
         @ONLY )
      # And now remember their names:
      list( APPEND _headerComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}.h )
      list( APPEND _selectionComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}_selection.xml )
   endforeach()

   # If the user asked for additional ElementLink<T> types to generate
   # dictionaries for, set these up now. Note that types listed as navigables,
   # are also used here.
   foreach( type ${ARG_ELEMENT_LINKS} ${ARG_NAVIGABLES} )
      # Sanitise the type name:
      string( REPLACE ":" "_" typeSanitised ${type} )
      string( REPLACE "<" "_" typeSanitised ${typeSanitised} )
      string( REPLACE ">" "_" typeSanitised ${typeSanitised} )
      string( REPLACE " " "_" typeSanitised ${typeSanitised} )
      # Generate the header files:
      configure_file( ${_dataLinkHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}.h @ONLY )
      configure_file( ${_elementLinkHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLink${typeSanitised}.h @ONLY )
      configure_file( ${_elementLinkVectorHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLinkVector${typeSanitised}.h @ONLY )
      # Generate the selection file:
      configure_file( ${_dataLinkSelection}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}_selection.xml
         @ONLY )
      configure_file( ${_elementLinkSelection}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLink${typeSanitised}_selection.xml
         @ONLY )
      configure_file( ${_elementLinkVectorSelection}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLinkVector${typeSanitised}_selection.xml
         @ONLY )
      # And now remember their names:
      list( APPEND _headerComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}.h
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLink${typeSanitised}.h
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLinkVector${typeSanitised}.h )
      list( APPEND _selectionComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DataLink${typeSanitised}_selection.xml
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLink${typeSanitised}_selection.xml
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ElementLinkVector${typeSanitised}_selection.xml )
   endforeach()

   # If the user asked for additional Navigable<T> types to generate dictionaries
   # for, set these up now:
   foreach( type ${ARG_NAVIGABLES} )
      # Sanitise the type name:
      string( REPLACE ":" "_" typeSanitised ${type} )
      string( REPLACE "<" "_" typeSanitised ${typeSanitised} )
      string( REPLACE ">" "_" typeSanitised ${typeSanitised} )
      string( REPLACE " " "_" typeSanitised ${typeSanitised} )
      # Generate the header file:
      configure_file( ${_navigableHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Navigable${typeSanitised}.h
         @ONLY )
      # Generate the selection file:
      configure_file( ${_navigableSelection}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Navigable${typeSanitised}_selection.xml
         @ONLY )
      # And now remember their names:
      list( APPEND _headerComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Navigable${typeSanitised}.h )
      list( APPEND _selectionComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Navigable${typeSanitised}_selection.xml )
   endforeach()

   # Prepare the header file:
   set( _dictHeader ${CMAKE_CURRENT_SOURCE_DIR}/${libHeader} )
   set( _originalDictHeader )
   if( _headerComponents )
      # Remove the possible duplicates:
      list( REMOVE_DUPLICATES _headerComponents )
      # The name of the file to produce:
      get_filename_component( _headerName ${libHeader} NAME )
      set( _dictHeader
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_headerName} )
      set( _originalDictHeader ORIGINAL_HEADER
         ${CMAKE_CURRENT_SOURCE_DIR}/${libHeader} )
      unset( _headerName )
      # Generate a text file with the names of the files to merge:
      set( _listFileName
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}Headers.txt )
      file( REMOVE ${_listFileName} )
      foreach( _file ${CMAKE_CURRENT_SOURCE_DIR}/${libHeader}
            ${_headerComponents} )
         file( APPEND ${_listFileName} "${_file}\n" )
      endforeach()
      # Describe how to produce this merged header:
      add_custom_command( OUTPUT ${_dictHeader}
         COMMAND ${_mergeFilesCmd} ${_dictHeader} ${_listFileName}
         DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${libHeader} ${_headerComponents}
         ${_stampFile} )
      # Clean up:
      unset( _listFileName )
   endif()

   # Prepare the selection file:
   set( _dictSelection ${CMAKE_CURRENT_SOURCE_DIR}/${libSelection} )
   if( _selectionComponents )
      # Remove the possible duplicates:
      list( REMOVE_DUPLICATES _selectionComponents )
      # The name of the file to produce:
      get_filename_component( _selectionName ${libSelection} NAME )
      set( _dictSelection
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_selectionName} )
      unset( _selectionName )
      # Describe how to produce this merged selection file:
      add_custom_command( OUTPUT ${_dictSelection}
         COMMAND ${_mergeSelectionsCmd} ${_dictSelection}
         ${CMAKE_CURRENT_SOURCE_DIR}/${libSelection}
         ${_selectionComponents}
         DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${libSelection}
         ${_selectionComponents} ${_stampFile} )
   endif()

   # Add the directory holding the header file to the header search path,
   # if it is different from the header directory.
   set( _extraInclude )
   get_filename_component( _headerDir ${libHeader} PATH )
   get_filename_component( _headerDirName ${_headerDir} NAME )
   get_filename_component( _headerDir ${CMAKE_CURRENT_SOURCE_DIR}/${_headerDir}
      ABSOLUTE )
   if( NOT ${_headerDirName} STREQUAL ${pkgName} )
      set( _extraInclude $<BUILD_INTERFACE:${_headerDir}> )
   endif()
   unset( _headerDir )
   unset( _headerDirName )

   # Set up the options(s) for propagation:
   set( _rootmapOpt )
   if( ARG_NO_ROOTMAP_MERGE )
      set( _rootmapOpt NO_ROOTMAP_MERGE )
   endif()

   # Generate a dictionary file:
   atlas_generate_reflex_dictionary( _reflexSource ${libName}
      HEADER ${_dictHeader} SELECTION ${_dictSelection}
      LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
      INCLUDE_DIRS ${ARG_INCLUDE_DIRS}
      ${_rootmapOpt} ${_originalDictHeader} )

   # The library needs to be linked to the core ROOT libraries:
   find_package( ROOT QUIET COMPONENTS Core )

   # Resolve possible wildcards in the extra source file names:
   set( _sources )
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   foreach( _extra ${ARG_EXTRA_FILES} )
      file( GLOB _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
         ${_extra} )
      foreach( _file ${_files} )
         list( APPEND _sources ${_file} )
      endforeach()
   endforeach()
   unset( _extraFlags )

   # Put the files into source groups. So they would show up in a ~reasonable
   # way in an IDE like Xcode:
   atlas_group_source_files( ${_sources} ${libHeader} ${libSelection} )

   # Create a library from all the sources:
   add_library( ${libName} SHARED ${_reflexSource} ${_sources} ${libHeader}
      ${libSelection} )

   # Set up its properties:
   add_dependencies( ${libName} ${libName}DictGen )
   add_dependencies( Package_${pkgName} ${libName} )
   set_property( TARGET ${libName} PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${libName} PROPERTY FOLDER ${pkgDir} )
   target_include_directories( ${libName} PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      ${_extraInclude} )
   target_include_directories( ${libName} SYSTEM PRIVATE
      ${ARG_INCLUDE_DIRS}
      ${ROOT_INCLUDE_DIRS} )
   unset( _extraInclude )
   if( NOT APPLE )
      # Turn off the --as-needed flag in this ugly way. As CMake doesn't seem
      # to provide any more elegant way of overriding the default linker flags
      # library-by-library.
      target_link_libraries( ${libName} "-Wl,--no-as-needed" )
   endif()
   target_link_libraries( ${libName} ${ROOT_LIBRARIES} ${ARG_LINK_LIBRARIES} )

   # In case we are building optimised libraries with debug info, and we have
   # objcopy available, detach the debug information into a separate library
   # file.
   if( "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" AND CMAKE_OBJCOPY )
      set( _libFileName "${CMAKE_SHARED_LIBRARY_PREFIX}${libName}" )
      set( _libFileName "${_libFileName}${CMAKE_SHARED_LIBRARY_SUFFIX}" )
      add_custom_command( TARGET ${libName} POST_BUILD
         COMMAND ${CMAKE_OBJCOPY} --only-keep-debug ${_libFileName}
         ${_libFileName}.dbg
         COMMAND ${CMAKE_OBJCOPY} --strip-debug ${_libFileName}
         COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=${_libFileName}.dbg
         ${_libFileName}
         WORKING_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
         COMMENT
         "Detaching debug info of ${_libFileName} into ${_libFileName}.dbg" )
      install( FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${_libFileName}.dbg
         DESTINATION ${CMAKE_INSTALL_LIBDIR}
         COMPONENT "Debug"
         OPTIONAL )
   endif()

   # On MacOS X make sure that the dictionary library is soft linked
   # with a .so postfix as well. Because genreflex can't make rootmap
   # files that would find libraries with a .dylib extension. And we
   # must create the dictionary as a shared library, not a module
   # library.
   if( APPLE )
      set( _linkname "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
      set( _linkname "${_linkname}${CMAKE_SHARED_MODULE_PREFIX}${libName}" )
      set( _linkname "${_linkname}${CMAKE_SHARED_MODULE_SUFFIX}" )
      add_custom_target( ${libName}Link
         ALL SOURCES ${_linkname} )
      add_dependencies( ${libName}Link ${libName} )
      add_dependencies( Package_${pkgName} ${libName}Link )
      set_property( TARGET ${libName}Link PROPERTY LABELS ${pkgName} )
      set_property( TARGET ${libName}Link PROPERTY FOLDER ${pkgDir}/Internals )
      add_custom_command( OUTPUT ${_linkname}
         COMMAND ${CMAKE_COMMAND} -E create_symlink
         ${CMAKE_SHARED_LIBRARY_PREFIX}${libName}${CMAKE_SHARED_LIBRARY_SUFFIX}
         ${_linkname} )
      set_directory_properties( PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
         ${_linkname} )
      install( FILES ${_linkname}
         DESTINATION ${CMAKE_INSTALL_LIBDIR} )
   endif()

   # Declare how to install the library:
   install( TARGETS ${libName}
      DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL )

endfunction( atlas_add_dictionary )

# This function can be used by the packages to generate a CINT dictionary
# source file, that can be built into a "normal" library in the package.
#
# In case the user only specifies a LinkDef.h file without any other header
# file names, the code attempts to extract the relevant header file names
# from the LinkDef.h file itself. But this should just be used as a stopgap
# solution until all RootCore packages are properly migrated to the new
# build system.
#
# The EXTERNAL_PACKAGES option can be used to define packages that the
# dictionary generation depends on. Typically this will just be ROOT.
#
# Usage: atlas_add_root_dictionary( mainLibName dictFileNameVar
#                                   [ROOT_HEADERS Header1.h LinkDef.h]
#                                   [EXTERNAL_PACKAGES ROOT]
#                                   [INCLUDE_PATHS /some/path...] )
#
function( atlas_add_root_dictionary libName dictfile )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "" ""
      "ROOT_HEADERS;EXTERNAL_PACKAGES;INCLUDE_PATHS" ${ARGN} )

   # Some sanity checks:
   if( NOT ARG_ROOT_HEADERS )
      message( SEND_ERROR "No ROOT_HEADERS option specified!" )
      return()
   endif()

   # Set common compiler options:
   atlas_set_compiler_flags()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Find ROOT, as we need to get the dictionary generator's location from it:
   find_package( ROOT COMPONENTS Core QUIET )

   # Allow wildcarded expressions in the ROOT_HEADERS argument.
   set( _headers )
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   foreach( _header ${ARG_ROOT_HEADERS} )
      file( GLOB _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
         "${_header}" )
      list( APPEND _headers ${_files} )
      unset( _files )
   endforeach()
   unset( _extraFlags )

   # The name of the files that we'll be generating:
   set( dictsource "${CMAKE_CURRENT_BINARY_DIR}" )
   set( dictsource "${dictsource}${CMAKE_FILES_DIRECTORY}" )
   set( dictsource "${dictsource}/${libName}CintDict.cxx" )
   set( pcm_name "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
   set( pcm_name "${pcm_name}${CMAKE_SHARED_LIBRARY_PREFIX}" )
   set( pcm_name "${pcm_name}${libName}_rdict.pcm" )
   set( rootmap_name "${CMAKE_CURRENT_BINARY_DIR}" )
   set( rootmap_name "${rootmap_name}${CMAKE_FILES_DIRECTORY}" )
   set( rootmap_name "${rootmap_name}/${libName}.dsomap" )

   # The library's physical name:
   set( library_name
      ${CMAKE_SHARED_LIBRARY_PREFIX}${libName}${CMAKE_SHARED_LIBRARY_SUFFIX} )

   # Get the current directory's compiler definitions. But remove the
   # PACKAGE_VERSION stuff from in there.
   get_directory_property( definitions COMPILE_DEFINITIONS )
   list( REMOVE_DUPLICATES definitions )

   # Get the compile definitions from the main library:
   list( APPEND definitions
      "$<TARGET_PROPERTY:${libName},INTERFACE_COMPILE_DEFINITIONS>" )

   # Unfortunately at this point the escape characters are gone from the
   # definitions. Most notably for the PACKAGE_VERSION macro. So let's add
   # the escapes back.
   set( defCopy ${definitions} )
   set( definitions )
   foreach( _def ${defCopy} )
      string( REPLACE "\"" "\\\"" _newDef ${_def} )
      list( APPEND definitions ${_newDef} )
   endforeach()

   # Assemble the "regular" include directories.
   set( incdirs "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                "$<TARGET_PROPERTY:${libName},INCLUDE_DIRECTORIES>" )

   # Get the directory's include directories.
   get_directory_property( dirincdirs INCLUDE_DIRECTORIES )
   list( APPEND incdirs &{dirincdirs} )
   unset( dirincdirs )

   # Assemble the "system" include directories.
   set( systincdirs
      "$<TARGET_PROPERTY:${libName},INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>" )
   # Look for extra packages if necessary, and set up their include
   # paths as "system" include directories.
   foreach( _extpkg ${ARG_EXTERNAL_PACKAGES} )
      find_package( ${_extpkg} )
      string( TOUPPER ${_extpkg} _pkgup )
      if( ${_pkgup}_FOUND )
         list( APPEND systincdirs "${${_pkgup}_INCLUDES}" )
      endif()
   endforeach()

   # Exclude /usr/include from the include paths. With "-I" it is simply
   # unnecessary, but with "-isystem" it is outright destructive.
   set( incdirs
      "$<FILTER:${incdirs},EXCLUDE,^(/usr|/usr/local)?/include$>" )
   set( systincdirs
      "$<FILTER:${systincdirs},EXCLUDE,^(/usr|/usr/local)?/include$>" )

   # The full command to generate the root dictionary:
   set( cmd "set -e\n" )
   set( cmd "${cmd}tmpdir=`mktemp -d make${libName}CintDict.XXXXXX`\n" )
   set( cmd "${cmd}cd \${tmpdir}\n" )
   set( cmd "${cmd}\"${ROOT_rootcling_EXECUTABLE}\" -f ${libName}CintDict.cxx" )
   set( cmd "${cmd} -s ${library_name}" )
   set( cmd "${cmd} -rml ${library_name} -rmf ${libName}.dsomap" )
   set( cmd "${cmd} $<$<BOOL:${definitions}>:-D>$<JOIN:${definitions}, -D>" )
   set( cmd "${cmd} $<$<BOOL:${incdirs}>:-I\">$<JOIN:$<REMOVE_DUPLICATES:${incdirs}>,\" -I\">$<$<BOOL:${incdirs}>:\">" )
   set( cmd "${cmd} $<$<BOOL:${systincdirs}>:-isystem \">$<JOIN:$<REMOVE_DUPLICATES:${systincdirs}>,\" -isystem \">$<$<BOOL:${systincdirs}>:\">" )
   set( cmd "${cmd} \"$<JOIN:${_headers},\" \">\"\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy ${libName}CintDict.cxx " )
   set( cmd "${cmd} \"${dictsource}\"\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy ${libName}.dsomap" )
   set( cmd "${cmd} \"${rootmap_name}\"\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy" )
   set( cmd "${cmd} ${CMAKE_SHARED_LIBRARY_PREFIX}${libName}_rdict.pcm" )
   set( cmd "${cmd} \"${pcm_name}\"\n" )

   # Generate a script that will run rootcling. This is necessary because
   # there are some bugs currently (3.3.2) in how add_custom_command handles
   # generator expressions. But file(GENERATE...) does do this correctly.
   set( scriptName "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}" )
   set( scriptName "${scriptName}/make${libName}CintDict.sh" )
   file( GENERATE OUTPUT ${scriptName}
      CONTENT "${cmd}" )

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

   # Set up proper C++ dependencies for the local headers:
#   set( implicitHeaders )
#   foreach( header ${localHeaders} )
#      list( APPEND implicitHeaders CXX ${CMAKE_CURRENT_SOURCE_DIR}/${header} )
#   endforeach()

   # Call the generated script to create the dictionary files:
   add_custom_command( OUTPUT ${dictsource} ${pcm_name} ${rootmap_name}
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
      ${BASH_EXECUTABLE} ${scriptName}
      DEPENDS ${_headers}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY} )
#      IMPLICIT_DEPENDS ${implicitHeaders} )
   set_property( SOURCE ${dictsource} ${pcm_name} ${rootmap_name}
      PROPERTY LABELS ${pkgName} )

   # Install the generated auxiliary files:
   install( FILES ${pcm_name}
      DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL )

   # Set up the merging of the rootmap files:
   set_property( GLOBAL APPEND PROPERTY ATLAS_ROOTMAP_FILES ${rootmap_name} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_ROOTMAP_TARGETS ${libName} )

   # Set the return argument:
   set( ${dictfile} ${dictsource} PARENT_SCOPE )

endfunction( atlas_add_root_dictionary )

# Function used instead of REFLEX_GENERATE_DICTIONARY when building a
# Reflex library, to overcome the shortcomings of the ROOT built-in
# function.
#
# This is a fairly complicated function. It needs to figure out the include
# paths required for the dictionary generation based on the include directories
# specified for the current directory, and the properties of the libraries
# that the dictionary library will eventually be linked against. (Doing the
# latter with generator expressions.)
#
# Usage: atlas_generate_reflex_dictionary( dictFileNameVar
#                                          dictNamePrefix
#                                          HEADER <pkgName>/<pkgName>Dict.h
#                                          SELECTION <pkgName>/selection.xml
#                                          [LINK_LIBRARIES Library1...]
#                                          [INCLUDE_DIRS Include1...]
#                                          [ORIGINAL_HEADER <header.h>]
#                                          [NO_ROOTMAP_MERGE] )
#
function( atlas_generate_reflex_dictionary dictfile dictname )

   # Parse all options:
   cmake_parse_arguments( ARG "NO_ROOTMAP_MERGE"
      "HEADER;SELECTION;ORIGINAL_HEADER"
      "LINK_LIBRARIES;INCLUDE_DIRS" ${ARGN} )

   # A security check:
   if( NOT ARG_HEADER )
      message( FATAL_ERROR
         "No header file specified using the HEADER option" )
   endif()
   if( NOT ARG_SELECTION )
      message( FATAL_ERROR
         "No selection file specified using the SELECTION option" )
   endif()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Find ROOT, as we need to get the dictionary generator's location from it:
   find_package( ROOT COMPONENTS Core QUIET )

   # The name of the files that we'll be generating:
   set( dictsource "${CMAKE_CURRENT_BINARY_DIR}" )
   set( dictsource "${dictsource}${CMAKE_FILES_DIRECTORY}" )
   set( dictsource "${dictsource}/${dictname}ReflexDict.cxx" )
   set( pcm_name "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
   set( pcm_name "${pcm_name}${CMAKE_SHARED_LIBRARY_PREFIX}" )
   set( pcm_name "${pcm_name}${dictname}_rdict.pcm" )
   set( rootmap_name_rel "${pkgDir}${CMAKE_FILES_DIRECTORY}/${dictname}.dsomap" )
   set( rootmap_name "${CMAKE_BINARY_DIR}/${rootmap_name_rel}" )

   # Decide what name to use for the rootmap file in the dependency file.
   # As it happens, CMake just prefers a different scheme with different
   # versions...
   if( "${CMAKE_VERSION}" VERSION_LESS "3.20" )
      set( rootmap_name_dep "${rootmap_name_rel}" )
   else()
      set( rootmap_name_dep "${rootmap_name}" )
   endif()

   # The library's physical name:
   if( APPLE )
      set( library_name "${CMAKE_SHARED_MODULE_PREFIX}${dictname}" )
      set( library_name "${library_name}${CMAKE_SHARED_MODULE_SUFFIX}" )
   else()
      set( library_name "${CMAKE_SHARED_LIBRARY_PREFIX}${dictname}" )
      set( library_name "${library_name}${CMAKE_SHARED_LIBRARY_SUFFIX}" )
   endif()

   # Get the current directory's compiler definitions. But remove the
   # PACKAGE_VERSION stuff from in there.
   get_directory_property( definitions COMPILE_DEFINITIONS )
   list( REMOVE_DUPLICATES definitions )

   # Simply always use the definition needed to build dictionaries for Eigen
   # classes. As the definition doesn't hurt when Eigen is not used anyway...
   list( APPEND definitions EIGEN_DONT_VECTORIZE )

   # Get the compile definitions from the dictionary library:
   list( APPEND definitions
      "$<TARGET_PROPERTY:${dictname},INTERFACE_COMPILE_DEFINITIONS>" )

   # Unfortunately at this point the escape characters are gone from the
   # definitions. Most notably for the PACKAGE_VERSION macro. So let's add
   # the escapes back.
   set( defCopy ${definitions} )
   set( definitions )
   foreach( _def ${defCopy} )
      string( REPLACE "\"" "\\\"" _newDef ${_def} )
      list( APPEND definitions ${_newDef} )
   endforeach()
   unset( defCopy )
   unset( _newDef )

   # Get the include directories:
   get_directory_property( incdirs INCLUDE_DIRECTORIES )
   # Add the package directory to it:
   list( INSERT incdirs 0 "${CMAKE_CURRENT_SOURCE_DIR}" )
   # Add the binary directory to it. In case auto-generated headers were put
   # there.
   list( APPEND incdirs "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}" )
   # Add the dictionary library's include directories to it:
   list( APPEND incdirs "$<TARGET_PROPERTY:${dictname},INCLUDE_DIRECTORIES>" )
   # Now assemble the "system" include directories.
   set( systincdirs )
   # Start with all the explicitly defined header directories.
   if( ARG_INCLUDE_DIRS )
      list( APPEND systincdirs ${ARG_INCLUDE_DIRS} )
   endif()
   # Add the system include directories set up on the library.
   list( APPEND systincdirs
      "$<TARGET_PROPERTY:${dictname},INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>" )

   # Exclude /usr/include from the include paths. With "-I" it is simply
   # unnecessary, but with "-isystem" it is outright destructive.
   set( incdirs
      "$<FILTER:${incdirs},EXCLUDE,^(/usr|/usr/local)?/include$>" )
   set( systincdirs
      "$<FILTER:${systincdirs},EXCLUDE,^(/usr|/usr/local)?/include$>" )

   # The name of the generated dependency file:
   set( depfile "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}" )
   set( depfile "${depfile}/${dictname}ReflexDict.d" )

   # Decide if a dependency file could/should be generated/used.
   set( _useDepfileDefault FALSE )
   if( ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR
            "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" ) AND
         "${CMAKE_GENERATOR}" STREQUAL "Ninja" )
      set( _useDepfileDefault TRUE )
   endif()
   set( ATLAS_DICT_USE_DEPFILE ${_useDepfileDefault} CACHE BOOL
      "Whether a .d file should be set up / used with Ninja" )
   mark_as_advanced( ATLAS_DICT_USE_DEPFILE )

   # The full command to generate the reflex dictionary:
   set( cmd "set -e\n" )
   set( cmd "${cmd}tmpdir=`mktemp -d make${dictname}ReflexDict.XXXXXX`\n" )
   set( cmd "${cmd}cd \${tmpdir}\n" )
   if( ATLAS_DICT_USE_DEPFILE )
      set( cmd "${cmd}${CMAKE_CXX_COMPILER}" )
      if( NOT "${CMAKE_CXX_STANDARD}" STREQUAL "" )
         set( cmd "${cmd} -std=c++${CMAKE_CXX_STANDARD}" )
      endif()
      set( cmd "${cmd} $<$<BOOL:${definitions}>:-D>$<JOIN:${definitions}, -D>" )
      set( cmd "${cmd} $<$<BOOL:${incdirs}>:-I\">$<JOIN:$<REMOVE_DUPLICATES:${incdirs}>,\" -I\">$<$<BOOL:${incdirs}>:\">" )
      set( cmd "${cmd} $<$<BOOL:${systincdirs}>:-isystem \">$<JOIN:$<REMOVE_DUPLICATES:${systincdirs}>,\" -isystem \">$<$<BOOL:${systincdirs}>:\">" )
      if( CMAKE_OSX_SYSROOT )
         set( cmd "${cmd} -isysroot ${CMAKE_OSX_SYSROOT}" )
      endif()
      set( cmd "${cmd} -MM -MT ${rootmap_name_dep} -MF ${dictname}.d" )
      set( cmd "${cmd} -x c++ ${ARG_HEADER}\n" )
   endif()
   set( cmd "${cmd}\"${ROOT_genreflex_EXECUTABLE}\" ${ARG_HEADER}" )
   set( cmd "${cmd} -o ${dictname}ReflexDict.cxx" )
   set( cmd "${cmd} --noIncludePaths" )
   set( cmd "${cmd} --rootmap=${dictname}.dsomap" )
   set( cmd "${cmd} --rootmap-lib=${library_name}" )
   set( cmd "${cmd} --library=${library_name}" )
   set( cmd "${cmd} --select=\"${ARG_SELECTION}\"" )
   set( cmd "${cmd} $<$<BOOL:${definitions}>:-D>$<JOIN:${definitions}, -D>" )
   set( cmd "${cmd} $<$<BOOL:${incdirs}>:-I\">$<JOIN:$<REMOVE_DUPLICATES:${incdirs}>,\" -I\">$<$<BOOL:${incdirs}>:\">" )
   set( cmd "${cmd} $<$<BOOL:${systincdirs}>:-isystem \">$<JOIN:$<REMOVE_DUPLICATES:${systincdirs}>,\" -isystem \">$<$<BOOL:${systincdirs}>:\">\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy ${dictname}ReflexDict.cxx" )
   set( cmd "${cmd} \"${dictsource}\"\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy ${dictname}.dsomap" )
   set( cmd "${cmd} \"${rootmap_name}\"\n" )
   set( cmd "${cmd}${CMAKE_COMMAND} -E copy" )
   set( cmd "${cmd} ${CMAKE_SHARED_LIBRARY_PREFIX}${dictname}_rdict.pcm" )
   set( cmd "${cmd} \"${pcm_name}\"\n" )
   if( ATLAS_DICT_USE_DEPFILE )
      set( cmd "${cmd}${CMAKE_COMMAND} -E compare_files ${dictname}.d" )
      set( cmd "${cmd} \"${depfile}\" ||" )
      set( cmd "${cmd} ${CMAKE_COMMAND} -E copy ${dictname}.d" )
      set( cmd "${cmd} \"${depfile}\"\n" )
   endif()

   # Generate a script that will run genreflex. This is necessary because
   # there are some bugs currently (3.3.2) in how add_custom_command handles
   # generator expressions. But file(GENERATE...) does do this correctly.
   set( scriptName "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}" )
   set( scriptName "${scriptName}/make${dictname}ReflexDict.sh" )
   file( GENERATE OUTPUT ${scriptName}
      CONTENT "${cmd}" )

   # Set up how to handle the dependencies on (re-)generating the dictionary.
   set( extraArgs )
   if( ATLAS_DICT_USE_DEPFILE )
      set( extraArgs DEPFILE "${depfile}" )
   else()
      if( ARG_ORIGINAL_HEADER )
         set( extraArgs IMPLICIT_DEPENDS CXX "${ARG_HEADER}"
            CXX "${ARG_ORIGINAL_HEADER}" )
      else()
         set( extraArgs IMPLICIT_DEPENDS CXX "${ARG_HEADER}" )
      endif()
   endif()

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

   # Call the generated script to create the dictionary files:
   add_custom_command( OUTPUT ${rootmap_name}
      BYPRODUCTS ${dictsource} ${pcm_name}
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
      ${BASH_EXECUTABLE} ${scriptName}
      DEPENDS ${ARG_HEADER} ${ARG_ORIGINAL_HEADER} ${ARG_SELECTION}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}
      ${extraArgs}
      COMMENT "Generating ${dictname}ReflexDict.cxx" )
   set_property( SOURCE ${dictsource} ${pcm_name} ${rootmap_name}
      PROPERTY LABELS ${pkgName} )

   # Set up a custom target that triggers the generation of the dictionary:
   add_custom_target( ${dictname}DictGen DEPENDS ${rootmap_name} )
   set_property( TARGET ${dictname}DictGen PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${dictname}DictGen PROPERTY FOLDER ${pkgDir}/Internals )

   # Set the INCLUDE_DIRECTORIES property on it. To allow the "implicit"
   # dependency generator code to find the headers referred to by the
   # dictionary header.
   set_property( TARGET ${dictname}DictGen
      PROPERTY INCLUDE_DIRECTORIES ${incdirs} )

   # Install the generated auxiliary files:
   install( FILES ${pcm_name}
      DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL )

   # Set up the merging of the rootmap files:
   if( NOT ARG_NO_ROOTMAP_MERGE )
      set_property( GLOBAL APPEND PROPERTY ATLAS_ROOTMAP_FILES
         ${rootmap_name} )
      set_property( GLOBAL APPEND PROPERTY ATLAS_ROOTMAP_TARGETS
         ${dictname}DictGen )
   endif()

   # Set the return argument:
   set( ${dictfile} ${dictsource} PARENT_SCOPE )

endfunction( atlas_generate_reflex_dictionary )
