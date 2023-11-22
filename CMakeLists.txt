# Set the minimum CMake version required to build the project.
cmake_minimum_required( VERSION 3.6 )

# Silence some warnings on macOS with new CMake versions.
if( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
   cmake_policy( SET CMP0068 NEW )
endif()


set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Path to the install directory")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Set the project's name and version.
project( FastFrames )

# Set up the "C++ version" to use.
set( CMAKE_CXX_STANDARD_REQUIRED 17 CACHE STRING
   "Minimum C++ standard required for the build" )
set( CMAKE_CXX_STANDARD 17 CACHE STRING
   "C++ standard to use for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL
   "(Dis)allow using compiler extensions" )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the install locations for libraries and binaries.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib )  # Needed by ROOT_GENERATE_DICTIONARY()

set(FastFrames_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/include" CACHE STRING "Path to the includes")

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3 -g" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Silence boost warnings
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_definitions(-DBOOST_ALLOW_DEPRECATED_HEADERS)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" OFF)

if (BUILD_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
      # set input and output files
      set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
      set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

      # request to configure the file
      configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
      message("Doxygen build started")

      # note the option ALL which allows to build the docs together with the application
      add_custom_target( doc_doxygen ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif (BUILD_DOC)

# ------------------------------------------------
# Dependencies and sub-libraries
# ------------------------------------------------

# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)
find_package( Python3 COMPONENTS Development REQUIRED )
find_package( Boost COMPONENTS python REQUIRED )

include_directories(${CMAKE_SOURCE_DIR})
# ------------------------------------------------
# The actual FastFrames library
# ------------------------------------------------

file(GLOB SOURCES "Root/*.cc" )
file(GLOB LIB_HEADERS "FastFrames/*.h" )

# Build the shared library.
add_library( FastFrames SHARED ${LIB_HEADERS} ${SOURCES} )
target_include_directories( FastFrames PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> $<INSTALL_INTERFACE:> ${ROOT_INCLUDE_DIRS} )
target_link_libraries( FastFrames ${ROOT_LIBRARIES} )
set_property( TARGET FastFrames
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )

# Build the Python module for C++ logger
add_library( cppLogger SHARED python_wrapper/utils/Logger.cxx )
target_link_libraries( cppLogger PRIVATE Python3::Python ${Python3_LIBRARIES} )
set_target_properties( cppLogger PROPERTIES
   PREFIX ""
   OUTPUT_NAME "cppLogger" )

# Build the Python module for config reader
add_library( ConfigReaderCpp SHARED python_wrapper/utils/ConfigReaderCpp.cxx ${LIB_HEADERS} )
target_link_libraries( ConfigReaderCpp PRIVATE Python3::Python ${Python3_LIBRARIES} ${Boost_LIBRARIES} FastFrames )
set_target_properties( ConfigReaderCpp PROPERTIES
   PREFIX ""
   OUTPUT_NAME "ConfigReaderCpp" )


set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib\n")

# Install the libraries.
install( TARGETS FastFrames ConfigReaderCpp cppLogger
   EXPORT FastFramesTargets
   ARCHIVE DESTINATION lib
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include/FastFrames )

# Export the targets.
install( EXPORT FastFramesTargets
   FILE FastFramesTargets.cmake
   NAMESPACE FastFrames::
   DESTINATION lib/cmake/FastFrames )

# Create a Config file for the package.
include(CMakePackageConfigHelpers)
configure_package_config_file(
   "${CMAKE_CURRENT_LIST_DIR}/FastFramesConfig.cmake.in"
   "${CMAKE_CURRENT_BINARY_DIR}/FastFramesConfig.cmake"
   INSTALL_DESTINATION lib/cmake/FastFrames
   PATH_VARS CMAKE_INSTALL_PREFIX
)

# Install the Config file.
install( FILES "${CMAKE_CURRENT_BINARY_DIR}/FastFramesConfig.cmake"
   DESTINATION lib/cmake/FastFrames )


# ------------------------------------------------
# FastFrames executables
# ------------------------------------------------

# Helper macro for building the project's executables.
macro( FastFrames_add_executable name )
  add_executable( ${name} ${ARGN} )
  target_include_directories( ${name} PUBLIC ${ROOT_INCLUDE_DIRS} )
  target_link_libraries( ${name} FastFrames ${ROOT_LIBRARIES} )
  install( TARGETS ${name}
    EXPORT FastFramesTargets
    RUNTIME DESTINATION bin )
endmacro( FastFrames_add_executable )

FastFrames_add_executable( fast-frames.exe util/fast-frames.cc )

ROOT_GENERATE_DICTIONARY(FastFrames_dict FastFrames/MainFrame.h MODULE FastFrames LINKDEF Root/LinkDef.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}.rootmap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
