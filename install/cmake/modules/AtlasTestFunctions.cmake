# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# File collecting CMake code for setting up (CTest) tests.
#

# This function takes care of internally of building the executable that would
# be run as a test. It should only be used by atlas_add_test(...) internally.
#
function( _atlas_add_compiled_test testName )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "" ""
      "SOURCES;INCLUDE_DIRS;LINK_LIBRARIES;DEFINITIONS" ${ARGN} )

   # Set common compiler options:
   atlas_set_compiler_flags()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Allow wildcards in the source names:
   set( _extraFlags )
   if( ATLAS_ALWAYS_CHECK_WILDCARDS )
      list( APPEND _extraFlags CONFIGURE_DEPENDS )
   endif()
   file( GLOB _sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${_extraFlags}
      ${ARG_SOURCES} )
   unset( _extraFlags )
   if( NOT _sources )
      message( WARNING "No sources available for test ${exeName}" )
      return()
   endif()

   # Put the files into source groups. So they would show up in a ~reasonable
   # way in an IDE like Xcode:
   atlas_group_source_files( ${_sources} )

   # The name of the executable target (remember that packages often use
   # very generic test names):
   set( exeName "${pkgName}_${testName}" )

   # Declare the executable:
   add_executable( ${exeName} EXCLUDE_FROM_ALL ${_sources} )

   # Set it's properties:
   add_dependencies( Package_${pkgName}_tests ${exeName} )
   set_property( TARGET ${exeName} PROPERTY LABELS ${pkgName} )
   set_property( TARGET ${exeName} PROPERTY FOLDER ${pkgDir}/Tests )
   set_property( TARGET ${exeName} PROPERTY OUTPUT_NAME "${testName}.exe" )
   set_property( TARGET ${exeName} PROPERTY
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test-bin" )
   foreach( _config DEBUG RELEASE MINSIZEREL RELWITHDEBINFO )
      set_property( TARGET ${exeName} PROPERTY
         RUNTIME_OUTPUT_DIRECTORY_${_config}
         "${CMAKE_CURRENT_BINARY_DIR}/test-bin" )
   endforeach()
   target_include_directories( ${exeName} PRIVATE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> )
   target_include_directories( ${exeName} SYSTEM PRIVATE
      ${ARG_INCLUDE_DIRS} )
   target_link_libraries( ${exeName} PRIVATE ${ARG_LINK_LIBRARIES} )
   target_compile_definitions( ${exeName} PRIVATE ${ARG_DEFINITIONS} )

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

endfunction( _atlas_add_compiled_test )

# This function takes care of internally setting up a simple script that calls
# the script specified by the user, with the arguments given in the
# configuration.
#
function( _atlas_add_script_test testName )

   # Parse the options given to the function:
   cmake_parse_arguments( ARG "" "" "SCRIPT" ${ARGN} )

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Create the test script installation target of the package if it
   # doesn't exist yet:
   if( NOT TARGET ${pkgName}TestScriptInstall )
      add_custom_target( ${pkgName}TestScriptInstall SOURCES
         $<TARGET_PROPERTY:${pkgName}TestScriptInstall,TEST_SCRIPTS> )
      add_dependencies( Package_${pkgName}_tests
         ${pkgName}TestScriptInstall )
      set_property( TARGET ${pkgName}TestScriptInstall
         PROPERTY LABELS ${pkgName} )
      set_property( TARGET ${pkgName}TestScriptInstall
         PROPERTY FOLDER ${pkgDir}/Internals )
   endif()

   # Set up a script ourselves, which executes the specified script, with all
   # of its possible arguments.

   # Get the first element of the list, which should itself always be a script
   # file name. All other possible elements would be the arguments for this
   # script.
   list( GET ARG_SCRIPT 0 _script )
   set( _args ${ARG_SCRIPT} )
   list( REMOVE_AT _args 0 )

   # Construct the command to put into our own script:
   if( IS_ABSOLUTE ${_script} OR
         ( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_script} ) OR
         ( IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_script} ) )
      # The IS_DIRECTORY check allows one to use "python" as the "script" name
      # in a package that has a python/ directory.
      set( _command "${_script}" )
   else()
      set( _command "${CMAKE_CURRENT_SOURCE_DIR}/${_script}" )
   endif()
   while( _args )
      list( GET _args 0 _arg )
      list( REMOVE_AT _args 0 )
      set( _command "${_command} ${_arg}" )
   endwhile()

   # Set up the creation of the script:
   add_custom_command( OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/test-bin/${testName}.exe
      COMMAND ${CMAKE_COMMAND} -E make_directory
      ${CMAKE_CURRENT_BINARY_DIR}/test-bin
      COMMAND ${CMAKE_COMMAND} -E echo "${_command}" >
      ${CMAKE_CURRENT_BINARY_DIR}/test-bin/${testName}.exe
      COMMAND chmod 755 ${CMAKE_CURRENT_BINARY_DIR}/test-bin/${testName}.exe
      VERBATIM )
   set_property( TARGET ${pkgName}TestScriptInstall APPEND PROPERTY
      TEST_SCRIPTS ${CMAKE_CURRENT_BINARY_DIR}/test-bin/${testName}.exe )

endfunction( _atlas_add_script_test )

# This function can be used to declare unit tests in a package. It can be
# called in two ways. Either:
#
#   atlas_add_test( TestName SOURCES test/source1.cxx...
#                   [INCLUDE_DIRS Dir1...]
#                   [LINK_LIBRARIES Library1...]
#                   [DEFINITIONS VAR1...]
#                   [NOEXEC]
#                   ... )
#
# Or like:
#
#   atlas_add_test( TestName SCRIPT test/script.sh [arg1...]
#                   ... )
#
# Additional arguments supported by both versions are:
#
#                   [LOG_SELECT_PATTERN patterns]
#                   [LOG_IGNORE_PATTERN patterns]
#                   [PRE_EXEC_SCRIPT script]
#                   [POST_EXEC_SCRIPT script]
#                   [ENVIRONMENT env...]
#                   [DEPENDS test1...]
#                   [LABELS label1...]
#                   [PROPERTIES <name> <value>...]
#                   [PRIVATE_WORKING_DIRECTORY]
#
function( atlas_add_test testName )

   # Parse the options given to the function:
   set( _booleanArgs NOEXEC PRIVATE_WORKING_DIRECTORY )
   set( _singleParamArgs SCRIPT LOG_SELECT_PATTERN LOG_IGNORE_PATTERN
      PRE_EXEC_SCRIPT POST_EXEC_SCRIPT )
   set( _multiParamArgs SOURCES DEPENDS LABELS PROPERTIES ENVIRONMENT )
   cmake_parse_arguments( ARG "${_booleanArgs}" "${_singleParamArgs}"
      "${_multiParamArgs}" ${ARGN} )

   # A sanity check:
   if( ( ( "${ARG_SOURCES}" STREQUAL "" ) AND
         ( "${ARG_SCRIPT}" STREQUAL "" ) ) OR
       ( ( NOT "${ARG_SOURCES}" STREQUAL "" ) AND
         ( NOT "${ARG_SCRIPT}" STREQUAL "" ) ) )
      message( SEND_ERROR "Script must be called with either SOURCES or "
         "SCRIPT in the arguments" )
      return()
   endif()

   # Set common compiler options:
   atlas_set_compiler_flags()

   # Get the package/subdirectory name:
   atlas_get_package_name( pkgName )

   # Get the package directory:
   atlas_get_package_dir( pkgDir )

   # Create the "tests target" of the package if it doesn't exist yet:
   if( NOT TARGET Package_${pkgName}_tests )
      add_custom_target( Package_${pkgName}_tests )
      add_dependencies( atlas_tests Package_${pkgName}_tests )
      if( ATLAS_ALWAYS_BUILD_TESTS )
         add_dependencies( Package_${pkgName} Package_${pkgName}_tests )
      endif()
      set_property( TARGET Package_${pkgName}_tests PROPERTY
         FOLDER ${pkgDir}/Internals )
   endif()

   # If the user gave source files, let's build a test executable:
   if( NOT "${ARG_SOURCES}" STREQUAL "" )

      _atlas_add_compiled_test( ${testName}
         SOURCES ${ARG_SOURCES}
         ${ARG_UNPARSED_ARGUMENTS} )

   elseif( NOT "${ARG_SCRIPT}" STREQUAL "" )

      _atlas_add_script_test( ${testName}
         SCRIPT ${ARG_SCRIPT}
         ${ARG_UNPARSED_ARGUMENTS} )

   else()
      message( FATAL_ERROR "Internal logic error detected!" )
   endif()

   # If the test is only to be set up, but not to be executed, stop here:
   if( ARG_NOEXEC )
      return()
   endif()

   # Create the directory in which the unit tests will be executed from.
   set( _workDir ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/unitTestRun )
   set( _preExecs )
   if ( ARG_PRIVATE_WORKING_DIRECTORY )
      set( _workDir ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/unitTestRun_${testName} )
      # Private working directories are always cleaned via the test's pre-exec:
      list( APPEND _preExecs "rm -rf ${_workDir}/*" )
   endif()

   file( MAKE_DIRECTORY ${_workDir} )

   # Set up the variables for the unit test executor script:
   if( ARG_PRE_EXEC_SCRIPT )
      list( APPEND _preExecs ${ARG_PRE_EXEC_SCRIPT} )
   endif()

   if( NOT _preExecs )
      set( PRE_EXEC_SCRIPT "# No pre-exec necessary" )
   else()
      list( JOIN _preExecs ";" PRE_EXEC_SCRIPT )
   endif()

   if( ARG_POST_EXEC_SCRIPT )
      set( POST_EXEC_SCRIPT ${ARG_POST_EXEC_SCRIPT} )
   else()
      set( PARAMS )
      if ( ARG_LOG_SELECT_PATTERN )
         set( PARAMS "${PARAMS} -s '${ARG_LOG_SELECT_PATTERN}'" )
      endif()
      if ( ARG_LOG_IGNORE_PATTERN )
         set( PARAMS "${PARAMS} -i '${ARG_LOG_IGNORE_PATTERN}'" )
      endif()
      set( POST_EXEC_SCRIPT "if type post.sh >/dev/null 2>&1; then
    post.sh ${testName} ${PARAMS}
else
    exit $testStatus
fi" )
   endif()

   # Decide where to take bash from:
   if( APPLE )
      # atlas_project(...) should take care of putting it here:
      set( BASH_EXECUTABLE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bash" )
   else()
      # Just take it from its default location:
      find_program( BASH_EXECUTABLE bash )
   endif()

   # Generate a test script that will run this unit test in the
   # correct environment:
   find_file( _utSkeleton unit_test_executor.sh.in
      PATH_SUFFIXES scripts PATHS ${CMAKE_MODULE_PATH} )
   configure_file( ${_utSkeleton}
      ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${testName}.sh @ONLY )
   mark_as_advanced( _utSkeleton )
   unset( _utSkeleton )

   # And run this script as the unit test:
   add_test( NAME ${pkgName}_${testName}_ctest
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${testName}.sh
      WORKING_DIRECTORY ${_workDir} )

   # Figure out the default timeout for tests.
   set( _defaultTimeout 120 )
   if( NOT "$ENV{ATLAS_DEFAULT_TEST_TIMEOUT}" STREQUAL "" )
      set( _defaultTimeout "$ENV{ATLAS_DEFAULT_TEST_TIMEOUT}" )
   endif()
   set( ATLAS_DEFAULT_TEST_TIMEOUT "${_defaultTimeout}" CACHE STRING
      "Default timeout (in seconds) for the ATLAS CTest tests" )

   # Set its properties:
   if( ARG_DEPENDS )
      # We are taking care of the test name mangling here:
      foreach( _test ${ARG_DEPENDS} )
         set_property( TEST ${pkgName}_${testName}_ctest
            APPEND PROPERTY DEPENDS "${pkgName}_${_test}_ctest" )
      endforeach()
   endif()

   set( _labels "${pkgName}" )
   if( ARG_LABELS )
      list( APPEND _labels ${ARG_LABELS} )
   endif()
   set_tests_properties( ${pkgName}_${testName}_ctest PROPERTIES
      LABELS "${_labels}"
      TIMEOUT "${ATLAS_DEFAULT_TEST_TIMEOUT}" )

   if( ARG_ENVIRONMENT )
      set_property( TEST ${pkgName}_${testName}_ctest PROPERTY
         ENVIRONMENT "${ARG_ENVIRONMENT}" )
   endif()

   if( ARG_PROPERTIES )
      set_tests_properties( ${pkgName}_${testName}_ctest PROPERTIES
         ${ARG_PROPERTIES} )
   endif()

endfunction( atlas_add_test )
