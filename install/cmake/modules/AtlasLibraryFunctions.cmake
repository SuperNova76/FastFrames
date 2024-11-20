# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# This file collects the ATLAS CMake helper functions that set up the build and
# installation of all the different kinds of libraries that we create in offline
# and analysis releases.
#
# This file should not be included directly, but through AtlasFunctions.cmake.
#

# The main function for creating a standard library in the analysis release.
# It creates a library that other components can link against (or not...),
# and installs it in the proper directory.
#
# One must use either NO_PUBLIC_HEADERS, or PUBLIC_HEADERS with a list of
# directories that contain the public headers. Not specifying either of them
# is an error.
#
# By default the function creates a shared library. If no source files are
# given to the function, or if INTERFACE is specified, an INTERFACE library
# is set up. If MODULE is specified, a MODULE (unlinkable) library is set up.
# Only one of these flags may be specified at a given time.
#
# INCLUDE_DIRS can be used to specify a list of directories that need to
# be seen by the compiler to build the library. And by all children of the
# library to compile against it.
#
# LINK_LIBRARIES can be used to specify library targets, or physical library
# files that need to be linked against when building the library. The
# INCLUDE_DIRS and LINK_LIBRARIES properties of the linked library targets
# are used transitively during the build.
#
# Usage: atlas_add_library( libraryName source1.cxx source2.cxx...
#                           [NO_PUBLIC_HEADERS | PUBLIC_HEADERS HeaderDir...]
#                           [INTERFACE | SHARED | MODULE | OBJECT | STATIC]
#                           [INCLUDE_DIRS Include1...]
#                           [PRIVATE_INCLUDE_DIRS Include2...]
#                           [LINK_LIBRARIES Library1 Library2...]
#                           [PRIVATE_LINK_LIBRARIES Library3...]
#                           [DEFINITIONS -Done...]
#                           [PRIVATE_DEFINITIONS -Dtwo...] )
#
function( atlas_add_library libName )

   # Parse the options given to the function.
   set( _noParamArgs NO_PUBLIC_HEADERS INTERFACE SHARED MODULE OBJECT STATIC )
   set( _multiParamArgs PUBLIC_HEADERS INCLUDE_DIRS PRIVATE_INCLUDE_DIRS
      LINK_LIBRARIES PRIVATE_LINK_LIBRARIES DEFINITIONS PRIVATE_DEFINITIONS )
   cmake_parse_arguments( ARG "${_noParamArgs}" "" "${_multiParamArgs}"
      ${ARGN} )
   unset( _noParamArgs )
   unset( _multiParamArgs )

   # Get the package/subdirectory name.
   atlas_get_package_name( pkgName )

   # Get the package directory.
   atlas_get_package_dir( pkgDir )

   # A sanity check.
   if( NOT ARG_NO_PUBLIC_HEADERS AND NOT ARG_PUBLIC_HEADERS )
      message( WARNING "Package ${pkgName} doesn't declare public headers for "
         "library ${libName}" )
   endif()

   # Collect the source file names.
   set( _sources )
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   foreach( _source ${ARG_UNPARSED_ARGUMENTS} )
      if( IS_ABSOLUTE ${_source} )
         list( APPEND _sources ${_source} )
      else()
         file( GLOB _files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
            ${_source} )
         foreach( _file ${_files} )
            list( APPEND _sources ${_file} )
         endforeach()
      endif()
   endforeach()
   unset( _extraFlags )

   # Make sure that only one of the library types is specified.
   set( _libTypes ARG_INTERFACE ARG_SHARED ARG_MODULE ARG_OBJECT ARG_STATIC )
   list( LENGTH _libTypes _nLibTypes )
   math( EXPR _nLibTypes "${_nLibTypes} - 1" )
   set( _libTypeSpecified FALSE )
   foreach( _index RANGE ${_nLibTypes} )
      list( GET _libTypes ${_index} _testedType )
      set( _otherTypes ${_libTypes} )
      list( REMOVE_AT _otherTypes ${_index} )
      if( ${_testedType} )
         set( _libTypeSpecified TRUE )
         foreach( _otherType ${_otherTypes} )
            if( ${_otherType} )
               message( SEND_ERROR "Only one library type must be specified!" )
               return()
            endif()
         endforeach()
      endif()
      unset( _testedType )
      unset( _otherTypes )
   endforeach()

   # Check whether one of the library types is specified. If not, the rule is
   # that by default a library becomes SHARED if source files were declared for
   # it, and INTERFACE if not.
   if( NOT _libTypeSpecified )
      if( _sources )
         set( ARG_SHARED TRUE )
      else()
         set( ARG_INTERFACE TRUE )
      endif()
   endif()

   # Clean up.
   unset( _libTypes )
   unset( _nLibTypes )
   unset( _libTypeSpecified )

   # Set common compiler options.
   atlas_set_compiler_flags()

   # Put the files into source groups. So they would show up in a ~reasonable
   # way in an IDE like Xcode.
   atlas_group_source_files( ${_sources} )

   # Declare the library to CMake.
   if( ARG_INTERFACE )
      add_library( ${libName} INTERFACE )
      if( _sources )
         # When specifying the files one-by-one using a generator expression,
         # then the absolute path version seems to work with any CMake version.
         set( _headers )
         foreach( _header ${_sources} )
            list( APPEND _headers
               "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${_header}>" )
         endforeach()
         # Attach the sources to the interface library, so that they would show
         # up in an IDE.
         target_sources( ${libName} INTERFACE ${_headers} )
         # Clean up.
         unset( _headers )
      endif()
   elseif( ARG_SHARED )
      add_library( ${libName} SHARED ${_sources} )
   elseif( ARG_MODULE )
      add_library( ${libName} MODULE ${_sources} )
   elseif( ARG_STATIC )
      add_library( ${libName} STATIC ${_sources} )
   elseif( ARG_OBJECT )
      if( ${CMAKE_VERSION} VERSION_LESS 3.12 )
         message( WARNING "Proper object library support is not available in "
            "CMake <= 3.12 (You're using: ${CMAKE_VERSION})!" )
      endif()
      add_library( ${libName} OBJECT ${_sources} )
   else()
      message( FATAL_ERROR "Internal coding error found!" )
   endif()

   # Now set all of its properties.
   add_dependencies( Package_${pkgName} ${libName} )
   if( NOT ARG_INTERFACE )
      set_property( TARGET ${libName} PROPERTY LABELS ${pkgName} )
      set_property( TARGET ${libName} PROPERTY FOLDER ${pkgDir} )
   endif()
   if( ARG_LINK_LIBRARIES )
      list( REMOVE_DUPLICATES ARG_LINK_LIBRARIES )
      if( NOT ARG_INTERFACE )
         target_link_libraries( ${libName} PUBLIC ${ARG_LINK_LIBRARIES} )
      else()
         target_link_libraries( ${libName} INTERFACE ${ARG_LINK_LIBRARIES} )
      endif()
   endif()
   if( ARG_PRIVATE_LINK_LIBRARIES )
      list( REMOVE_DUPLICATES ARG_PRIVATE_LINK_LIBRARIES )
      if( NOT ARG_INTERFACE )
         target_link_libraries( ${libName} PRIVATE
            ${ARG_PRIVATE_LINK_LIBRARIES} )
      else()
         message( WARNING "Private link libraries are not meaningful for "
            "interface libraries" )
      endif()
   endif()
   if( ARG_DEFINITIONS )
      if( NOT ARG_INTERFACE )
         target_compile_definitions( ${libName} PUBLIC ${ARG_DEFINITIONS} )
      else()
         target_compile_definitions( ${libName} INTERFACE ${ARG_DEFINITIONS} )
      endif()
   endif()
   if( ARG_PRIVATE_DEFINITIONS )
      if( NOT ARG_INTERFACE )
         target_compile_definitions( ${libName} PRIVATE
            ${ARG_PRIVATE_DEFINITIONS} )
      else()
         message( WARNING "Private definitions are not meaningful for "
            "interface libraries" )
      endif()
   endif()
   if( NOT ARG_INTERFACE )
      if( ARG_NO_PUBLIC_HEADERS )
         target_include_directories( ${libName} SYSTEM BEFORE PRIVATE
            ${ARG_INCLUDE_DIRS} ${ARG_PRIVATE_INCLUDE_DIRS} )
         target_include_directories( ${libName} BEFORE PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
      else()
         target_include_directories( ${libName} SYSTEM BEFORE PUBLIC
            ${ARG_INCLUDE_DIRS}
            PRIVATE
            ${ARG_PRIVATE_INCLUDE_DIRS} )
         target_include_directories( ${libName} BEFORE PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:src/${pkgDir}> )
      endif()
   else()
      target_include_directories( ${libName} SYSTEM BEFORE INTERFACE
         ${ARG_INCLUDE_DIRS} )
      target_include_directories( ${libName} BEFORE INTERFACE
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:src/${pkgDir}> )
      if( ARG_PRIVATE_INCLUDE_DIRS )
         message( WARNING "Private include directories are not meaningful for "
            "interface libraries" )
      endif()
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

   # Build STATIC and OBJECT libraries with position independent code by
   # default, as in ATLAS we mainly need these libraries in such a way.
   if( ARG_STATIC OR ARG_OBJECT )
      set_target_properties( ${libName} PROPERTIES
         POSITION_INDEPENDENT_CODE ON )
   endif()

   # Install the header files of the library if they exist. And even wait for
   # the installation with the library building. So that dictionaries would
   # surely find their associated headers by the time they are built.
   if( ARG_PUBLIC_HEADERS )
      _atlas_install_headers( ${ARG_PUBLIC_HEADERS} )
      add_dependencies( "${libName}" "${pkgName}HeaderInstall" )
   endif()

   # In case we are building optimised libraries with debug info, and we have
   # objcopy available, detach the debug information into a separate library
   # file.
   if( ( "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" ) AND CMAKE_OBJCOPY
         AND ( ARG_SHARED OR ARG_MODULE ) )
      if( ARG_MODULE )
         set( _libFileName "${CMAKE_SHARED_MODULE_PREFIX}${libName}" )
         set( _libFileName "${_libFileName}${CMAKE_SHARED_MODULE_SUFFIX}" )
      elseif( ARG_SHARED )
         set( _libFileName "${CMAKE_SHARED_LIBRARY_PREFIX}${libName}" )
         set( _libFileName "${_libFileName}${CMAKE_SHARED_LIBRARY_SUFFIX}" )
      else()
         message( FATAL_ERROR "Internal coding error found!" )
      endif()
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

   # Make it impossible to compile/link code against MODULE (component)
   # libraries.
   if( ARG_MODULE )
      target_compile_options( ${libName} INTERFACE
         "__MUST_NOT_COMPILE_AGAINST_COMPONENT_LIBRARY_${libName}__" )
      target_link_libraries( ${libName} INTERFACE
         "__MUST_NOT_LINK_AGAINST_COMPONENT_LIBRARY_${libName}__" )
   endif()

   # Declare how to install the library.
   install( TARGETS ${libName}
      OPTIONAL
      EXPORT ${CMAKE_PROJECT_NAME}Targets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      OBJECTS DESTINATION . )
   set_property( GLOBAL PROPERTY ATLAS_EXPORTS ON )
   set_property( GLOBAL APPEND PROPERTY ATLAS_EXPORTED_TARGETS ${libName} )

endfunction( atlas_add_library )

# This function takes care of building a component library for Athena/Gaudi.
# First it builds a library in a fairly ordinary way, and then builds
# configurables for it.
#
# Usage: atlas_add_component( libraryName source1.cxx source2.cxx...
#                             [NOCLIDDB]
#                             [GENCONF_PRELOAD preload]
#                             [INCLUDE_DIRS Include1...]
#                             [LINK_LIBRARIES Library1 Library2...] )
#
function( atlas_add_component libName )

   # Parse the arguments.
   cmake_parse_arguments( ARG "NOCLIDDB" "GENCONF_PRELOAD"
      "LINK_LIBRARIES" ${ARGN} )

   # Get this package's name.
   atlas_get_package_name( pkgName )

   # Get the package's directory.
   atlas_get_package_dir( pkgDir )

   # Build a library using the athena_add_library function.
   if( Gaudi_FOUND )
      atlas_add_library( ${libName} ${ARG_UNPARSED_ARGUMENTS}
         NO_PUBLIC_HEADERS MODULE
         PRIVATE_LINK_LIBRARIES ${ARG_LINK_LIBRARIES}
         Gaudi::GaudiPluginService )
   else()
      # If Gaudi is not available, then just build the library,
      # and don't do anything else.
      atlas_add_library( ${libName} ${ARG_UNPARSED_ARGUMENTS}
         NO_PUBLIC_HEADERS MODULE
         PRIVATE_LINK_LIBRARIES ${ARG_LINK_LIBRARIES} )
      return()
   endif()

   # Generate a .components file from the library.
   atlas_generate_componentslist( ${libName} )

   # Generate configurables for the library.
   set( _configurable
      ${CMAKE_PYTHON_OUTPUT_DIRECTORY}/${pkgName}/${libName}Conf.py )
   set( _initPy
      ${CMAKE_PYTHON_OUTPUT_DIRECTORY}/${pkgName}/__init__.py )
   set( _confdbFile "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
   set( _confdbFile "${_confdbFile}${CMAKE_SHARED_MODULE_PREFIX}" )
   set( _confdbFile "${_confdbFile}${libName}.confdb" )
   set( _fullLibName "${CMAKE_SHARED_MODULE_PREFIX}${libName}" )
   set( _fullLibName "${_fullLibName}${CMAKE_SHARED_MODULE_SUFFIX}" )
   set( _outdir ${CMAKE_CURRENT_BINARY_DIR}/genConf )
   set( _confdb2File "${_outdir}/${libName}.confdb2_part" )
   add_custom_command( OUTPUT ${_configurable} ${_confdbFile} ${_confdb2File}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${_outdir}
      COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
      $<TARGET_FILE:Gaudi::genconf> -o ${_outdir} -p ${pkgName} --no-init
      -i ${_fullLibName}
      COMMAND ${CMAKE_COMMAND} -E copy ${_outdir}/${libName}.confdb
      ${_confdbFile}
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_PYTHON_OUTPUT_DIRECTORY}/${pkgName}
      COMMAND ${CMAKE_COMMAND} -E copy ${_outdir}/${libName}Conf.py
      ${_configurable}
      # This is a tricky one. The __init__.py file may actually not
      # be writable. Since it may have been linked in from the base
      # release. So just swallow the output code of this command with
      # this trick found online:
      COMMAND ${CMAKE_COMMAND} -E touch ${_initPy} || :
      # Make sure that the file exists. In case we use a version of
      # Gaudi that doesn't produce it.
      COMMAND ${CMAKE_COMMAND} -E touch ${_confdb2File}
      DEPENDS ${libName} )
   set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} APPEND PROPERTY
      ADDITIONAL_MAKE_CLEAN_FILES ${_initPy} ${_configurable} ${_confdbFile}
      ${_confdb2File} )
   add_custom_target( ${libName}Configurables ALL
      DEPENDS ${_configurable} ${_confdbFile} ${_confdb2File} )
   set_property( TARGET ${libName}Configurables PROPERTY FOLDER ${pkgDir} )
   add_dependencies( Package_${pkgName} ${libName}Configurables )
   install( FILES ${_configurable}
      DESTINATION ${CMAKE_INSTALL_PYTHONDIR}/${pkgName}
      OPTIONAL )
   install( CODE "execute_process( COMMAND \${CMAKE_COMMAND} -E touch
      \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_PYTHONDIR}/${pkgName}/__init__.py )" )

   # Set up the merging of the confdb files.
   set_property( GLOBAL APPEND PROPERTY ATLAS_CONFDB_FILES ${_confdbFile} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_CONFDB_TARGETS
      ${libName}Configurables )
   # Set up the merging of the confdb2 files.
   set_property( GLOBAL APPEND PROPERTY ATLAS_CONFDB2_FILES ${_confdb2File} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_CONFDB2_TARGETS
      ${libName}Configurables )

   # Generate a CLID DB file.
   if( NOT ARG_NOCLIDDB )
      atlas_generate_cliddb( ${libName} )
   endif()

endfunction( atlas_add_component )

# This function generates a T/P converter library.
#
# T/P converter libraries are just simple (installed) libraries, with component
# lists getting generated for them. So this is mostly just a convenience
# function, calling on other functions to do the heavy lifting.
#
# Usage: atlas_add_tpcnv_library( libraryName source1.cxx source2.cxx...
#                                 PUBLIC_HEADERS HeaderDir...
#                                 [INCLUDE_DIRS Include1...]
#                                 [LINK_LIBRARIES Library1 Library2...] )
#
function( atlas_add_tpcnv_library libName )

   # Build the library:
   atlas_add_library( ${libName} ${ARGN} )

   # Generate a .components file for it:
   atlas_generate_componentslist( ${libName} )

endfunction( atlas_add_tpcnv_library )

# Generic function that creates both poolcnv and sercnv libraries during
# the build. Should not be used directly, but through one of the following
# functions:
#  - atlas_add_poolcnv_library
#  - atlas_add_sercnv_library
#
# The applicable parameters are defined for those functions.
#
function( _atlas_add_cnv_library libName )

   # We need Gaudi for this.
   if( NOT Gaudi_FOUND )
      message( WARNING "This function only works when Gaudi is used" )
      return()
   endif()

   # Parse the arguments.
   set( _singleParamArgs CNV_PFX SKELETON_PREFIX CONVERTER_PREFIX )
   set( _multiParamArgs FILES INCLUDE_DIRS LINK_LIBRARIES TYPES_WITH_NAMESPACE
      MULT_CHAN_TYPES )
   cmake_parse_arguments( ARG "" "${_singleParamArgs}" "${_multiParamArgs}"
      ${ARGN} )
   unset( _singleParamArgs )
   unset( _multiParamArgs )

   # Get this package's name.
   atlas_get_package_name( pkgName )
   # Get this package's directory.
   atlas_get_package_dir( pkgDir )

   # In the following let's create a file that gets updated every time any of
   # the arguments of this function is changed.

   # As a first step, let's generate a file with the values of all of the
   # arguments that this function received.
   set( _tempStampFile
      "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}.stamp.tmp" )
   file( WRITE ${_tempStampFile} "${ARG_CNV_PFX}" "${ARG_SKELETON_PREFIX}"
      "${ARG_CONVERTER_PREFIX}" "${ARG_FILES}" "${ARG_INCLUDE_DIRS}"
      "${ARG_LINK_LIBRARIES}" "${ARG_TYPES_WITH_NAMESPACE}"
      "${ARG_MULT_CHAN_TYPES}" )

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

   # Find the types that we need to make converter(s) for:
   set( _typeCache )
   foreach( _file ${ARG_FILES} )
      # Get the type name from the file name:
      get_filename_component( _type ${_file} NAME_WE )
      # Set the fully qualified type name like this as a start:
      set( _fullTypeName ${_type} )
      # Now check in the TYPES_WITH_NAMESPACE argument whether this type
      # has a namespace in front of it:
      set( _namespace " " )
      foreach( _typeName ${ARG_TYPES_WITH_NAMESPACE} )
         # Extract the name and namespace out of the full type name:
         if( "${_typeName}" MATCHES "^(.*)::([^:]+)$" )
            # And check if they match:
            if( "${CMAKE_MATCH_2}" STREQUAL "${_type}" )
               set( _namespace "${CMAKE_MATCH_1}" )
               set( _fullTypeName "${_typeName}" )
               break()
            endif()
         else()
            message( WARNING "Name specified for TYPES_WITH_NAMESPACE ("
               "${_typeName}) is not namespaced" )
            continue()
         endif()
      endforeach()
      # Now check if it's a multi channel type or not:
      set( _isMultiChan FALSE )
      foreach( _multi ${ARG_MULT_CHAN_TYPES} )
         if( "${_multi}" STREQUAL "${_fullTypeName}" )
            set( _isMultiChan TRUE )
            break()
         endif()
      endforeach()
      # Now store the results:
      list( APPEND _typeCache ${_type} ${_namespace} ${_file} ${_isMultiChan} )
   endforeach()
   # Clean up:
   unset( _type )
   unset( _fullTypeName )
   unset( _namespace )
   unset( _typeName )
   unset( _isMultiChan )

   # Find the merge command:
   find_program( _mergeFilesCmd mergeFiles.sh
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _mergeFilesCmd )

   # Find the skeleton files:
   find_file( _${ARG_SKELETON_PREFIX}CnvEntriesImpl
      ${ARG_SKELETON_PREFIX}Cnv_entries.cxx.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _${ARG_SKELETON_PREFIX}CnvSimple ${ARG_SKELETON_PREFIX}CnvSimple.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _${ARG_SKELETON_PREFIX}CnvMultiChan
      ${ARG_SKELETON_PREFIX}CnvMultiChan.h.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _${ARG_SKELETON_PREFIX}CnvItemListHeader
      ${ARG_SKELETON_PREFIX}CnvItemList_joboptionsHdr.py.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   find_file( _${ARG_SKELETON_PREFIX}CnvItemListItem
      ${ARG_SKELETON_PREFIX}CnvItemList_joboptions.py.in
      PATH_SUFFIXES skeletons PATHS ${CMAKE_MODULE_PATH} )
   mark_as_advanced( _${ARG_SKELETON_PREFIX}CnvEntriesImpl
      _${ARG_SKELETON_PREFIX}CnvSimple
      _${ARG_SKELETON_PREFIX}CnvMultiChan
      _${ARG_SKELETON_PREFIX}CnvItemListHeader
      _${ARG_SKELETON_PREFIX}CnvItemListItem )

   # Generate the headers of the files for the component generation:
   configure_file( ${_${ARG_SKELETON_PREFIX}CnvEntriesImpl}
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries_header.cxx
      @ONLY )

   # Now generate the files that will extend these skeletons:
   set( _entriesImplComponents )
   set( _typeCacheCopy ${_typeCache} )
   while( _typeCacheCopy )
      # The type and header name:
      list( GET _typeCacheCopy 0 _type )
      list( REMOVE_AT _typeCacheCopy 0 1 2 3 )
      # Describe how to create the files that will be appended to the
      # headers:
      add_custom_command( OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_${_type}_entries.cxx
         COMMAND ${CMAKE_COMMAND} -E echo
         "#include \"${ARG_CNV_PFX}${_type}${ARG_CONVERTER_PREFIX}Cnv.h\"" >
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_${_type}_entries.cxx
         COMMAND ${CMAKE_COMMAND} -E echo
         "DECLARE_CONVERTER( ${ARG_CNV_PFX}${_type}${ARG_CONVERTER_PREFIX}Cnv )" >>
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_${_type}_entries.cxx
         VERBATIM
         DEPENDS ${_stampFile} )
      # Remember the names of these files:
      list( APPEND _entriesImplComponents
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_${_type}_entries.cxx )
   endwhile()

   # Declare the rules merging these files:
   set( _sourcesListFile
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}Sources.txt )
   file( REMOVE ${_sourcesListFile} )
   foreach( _file
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries_header.cxx
         ${_entriesImplComponents} )
      file( APPEND ${_sourcesListFile} "${_file}\n" )
   endforeach()
   add_custom_command( OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries.cxx
      COMMAND ${_mergeFilesCmd}
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries.cxx
      ${_sourcesListFile}
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries_header.cxx
      ${_entriesImplComponents}
      ${_stampFile} )
   unset( _sourcesListFile )

   # Create a header for the ItemList file:
   if( _${ARG_SKELETON_PREFIX}CnvItemListHeader )
      configure_file( ${_${ARG_SKELETON_PREFIX}CnvItemListHeader}
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}ItemList_joboptionsHdr.py
         @ONLY )
      set( _itemListFiles
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}ItemList_joboptionsHdr.py )
   endif()

   # Find/generate the converters:
   set( _sources )
   set( _typeCacheCopy ${_typeCache} )
   while( _typeCacheCopy )
      # The type and header name:
      list( GET _typeCacheCopy 0 type )
      list( GET _typeCacheCopy 1 namespace )
      list( GET _typeCacheCopy 2 header )
      list( GET _typeCacheCopy 3 isMultiChan )
      list( REMOVE_AT _typeCacheCopy 0 1 2 3 )
      # Check if there are custom sources for this converter:
      if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/${ARG_CNV_PFX}${type}${ARG_CONVERTER_PREFIX}Cnv.cxx )
         list( APPEND _sources
            ${CMAKE_CURRENT_SOURCE_DIR}/src/${ARG_CNV_PFX}${type}${ARG_CONVERTER_PREFIX}Cnv.cxx )
      else()
         # No such converter is available, let's generate it:
         if( ${isMultiChan} )
            configure_file( ${_${ARG_SKELETON_PREFIX}CnvMultiChan}
               ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${ARG_CNV_PFX}${type}${ARG_CONVERTER_PREFIX}Cnv.h
               @ONLY )
         else()
            configure_file( ${_${ARG_SKELETON_PREFIX}CnvSimple}
               ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${ARG_CNV_PFX}${type}${ARG_CONVERTER_PREFIX}Cnv.h
               @ONLY )
         endif()
      endif()
      # Generate the ItemList file for this type:
      if( "${namespace}" STREQUAL " " )
         set( fullTypeName "${type}" )
      else()
         set( fullTypeName "${namespace}::${type}" )
      endif()
      if( _${ARG_SKELETON_PREFIX}CnvItemListItem )
         configure_file( ${_${ARG_SKELETON_PREFIX}CnvItemListItem}
            ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${pkgName}_${ARG_CNV_PFX}${type}_joboptions.py
            @ONLY )
         list( APPEND _itemListFiles
            ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${pkgName}_${ARG_CNV_PFX}${type}_joboptions.py )
      endif()
   endwhile()

   # Python code generated for the converter:
   set( _pythonFiles )

   # Merge the jobOptions files:
   if( _itemListFiles )
      set( _itemListsFileName
         ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${pkgName}ItemLists.txt )
      file( REMOVE ${_itemListsFileName} )
      foreach( _file ${_itemListFiles} )
         file( APPEND ${_itemListsFileName} "${_file}\n" )
      endforeach()
      add_custom_command( OUTPUT
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}ItemList_joboptions.py
         COMMAND ${CMAKE_COMMAND} -E make_directory
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}
         COMMAND ${_mergeFilesCmd}
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}ItemList_joboptions.py
         ${_itemListsFileName}
         DEPENDS ${_stampFile} )
      unset( _itemListsFileName )
      install( FILES
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}ItemList_joboptions.py
         DESTINATION ${CMAKE_INSTALL_JOBOPTDIR}/${pkgName} OPTIONAL )
      list( APPEND _pythonFiles
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}ItemList_joboptions.py )
   endif()

   # Generate a dummy main jobO file:
   if( "${ARG_SKELETON_PREFIX}" STREQUAL "Pool" )
      add_custom_command( OUTPUT
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}_joboptions.py
         COMMAND ${CMAKE_COMMAND} -E make_directory
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}
         COMMAND ${CMAKE_COMMAND} -E echo "# Dummy file" >
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}_joboptions.py
         VERBATIM )
      install( FILES
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}_joboptions.py
         DESTINATION ${CMAKE_INSTALL_JOBOPTDIR}/${pkgName} OPTIONAL )
      list( APPEND _pythonFiles
         ${CMAKE_JOBOPT_OUTPUT_DIRECTORY}/${pkgName}/${pkgName}_joboptions.py )
   endif()

   # Make sure that the python files are made:
   if( _pythonFiles )
      add_custom_target( ${pkgName}JobOptGen ALL SOURCES ${_pythonFiles} )
      add_dependencies( Package_${pkgName} ${pkgName}JobOptGen )
      set_property( TARGET ${pkgName}JobOptGen PROPERTY LABELS ${pkgName} )
      set_property( TARGET ${pkgName}JobOptGen PROPERTY FOLDER
         ${pkgDir}/Internals )
   endif()

   # Compile a library with the collected/generated sources:
   atlas_add_library( ${libName} ${_sources}
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${libName}_entries.cxx
      ${ARG_UNPARSED_ARGUMENTS}
      NO_PUBLIC_HEADERS MODULE
      INCLUDE_DIRS ${ARG_INCLUDE_DIRS}
      LINK_LIBRARIES ${ARG_LINK_LIBRARIES} Gaudi::GaudiKernel )
   target_include_directories( ${libName} PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> )

   # Generate a .components file from the library:
   atlas_generate_componentslist( ${libName} )

endfunction( _atlas_add_cnv_library )

# This function is used to generate POOL converter libraries in the offline
# build.
#
# Usage: atlas_add_poolcnv_library( EventInfoAthenaPool source1.cxx...
#           FILES xAODEventInfo/EventInfo.h...
#           [INCLUDE_DIRS Include1...]
#           [LINK_LIBRARIES Library1...]
#           [TYPES_WITH_NAMESPACE xAOD::EventInfo...]
#           [MULT_CHAN_TYPES My::SuperType...]
#           [CNV_PFX xAOD] )
#
function( atlas_add_poolcnv_library libName )

   # Call the generic function with the right arguments:
   _atlas_add_cnv_library( ${libName} ${ARGN} SKELETON_PREFIX Pool )

endfunction( atlas_add_poolcnv_library )

# This function is used to generate serialiser converter libraries in the
# offline build.
#
# Usage: atlas_add_sercnv_library( TrigMuonEventSerCnv source1.cxx...
#           FILES TrigMuonEvent/MuonFeature.h...
#           [INCLUDE_DIRS Include1...]
#           [LINK_LIBRARIES Library1...]
#           [TYPES_WITH_NAMESPACE xAOD::MuonContainer...]
#           [CNV_PFX xAOD] )
#
function( atlas_add_sercnv_library libName )

   # Call the generic function with the right arguments:
   _atlas_add_cnv_library( ${libName} ${ARGN} SKELETON_PREFIX Ser
      CONVERTER_PREFIX Ser )

endfunction( atlas_add_sercnv_library )

# Hide the internal function from outside users:
unset( _atlas_add_cnv_library )

# Function taking care of generating .components files from the built libraries.
# Which are in the end used by Athena to find components.
#
# Usage: atlas_generate_componentslist( MyLibrary )
#
function( atlas_generate_componentslist libName )

   # We need Gaudi for this:
   if( NOT Gaudi_FOUND )
      message( WARNING "This function only works when Gaudi is used" )
      return()
   endif()

   # Get this package's name:
   atlas_get_package_name( pkgName )

   # Get the package's directory:
   atlas_get_package_dir( pkgDir )

   # Generate a .components file from the library:
   set( _componentsfile "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/" )
   set( _componentsfile "${_componentsfile}${CMAKE_SHARED_MODULE_PREFIX}" )
   set( _componentsfile "${_componentsfile}${libName}.components" )
   set( _fullLibName "${CMAKE_SHARED_MODULE_PREFIX}${libName}" )
   set( _fullLibName "${_fullLibName}${CMAKE_SHARED_MODULE_SUFFIX}" )
   add_custom_command( OUTPUT ${_componentsfile}
      COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
      $<TARGET_FILE:Gaudi::listcomponents> --output ${_componentsfile}
      ${_fullLibName}
      DEPENDS ${libName} )
   add_custom_target( ${libName}ComponentsList ALL DEPENDS
      ${_componentsfile} ${_confdbfile} )
   add_dependencies( Package_${pkgName} ${libName}ComponentsList )
   set_property( TARGET ${libName}ComponentsList PROPERTY FOLDER ${pkgDir} )

   # Set up the merging of the components files:
   set_property( GLOBAL APPEND PROPERTY ATLAS_COMPONENTS_FILES
      ${_componentsfile} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_COMPONENTS_TARGETS
      ${libName}ComponentsList )

endfunction( atlas_generate_componentslist )

# Function generating CLID files for the libraries. It takes a library compiled
# in some way (it doesn't necessarily have to be a component library), and
# generates a CLID DB file from it.
#
# Usage: atlas_generate_cliddb( MyLibrary )
#
function( atlas_generate_cliddb libName )

   # Don't do anything here in standalone build mode:
   if( NOT Gaudi_FOUND )
      return()
   endif()

   # Get this package's name:
   atlas_get_package_name( pkgName )

   # Get the package's directory:
   atlas_get_package_dir( pkgDir )

   # The name of the output CLID file:
   set( _clidFile ${CMAKE_CURRENT_BINARY_DIR}/${libName}_clid.db )

   # Generate the file:
   add_custom_command( OUTPUT ${_clidFile}
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_SHARE_OUTPUT_DIRECTORY}
      COMMAND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atlas_build_run.sh
      genCLIDDB -p ${libName} -o ${_clidFile}
      DEPENDS genCLIDDB CLIDComps ${libName} )

   # Create a target generating the file:
   add_custom_target( ${libName}ClidGen ALL DEPENDS ${_clidFile} )
   add_dependencies( Package_${pkgName} ${libName}ClidGen )
   set_property( TARGET ${libName}ClidGen PROPERTY FOLDER ${pkgDir} )

   # Set up the merging of the CLID files:
   set_property( GLOBAL APPEND PROPERTY ATLAS_CLID_FILES ${_clidFile} )
   set_property( GLOBAL APPEND PROPERTY ATLAS_CLID_TARGETS ${libName}ClidGen )

endfunction( atlas_generate_cliddb )
