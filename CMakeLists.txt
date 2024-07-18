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
find_package( Boost COMPONENTS python REQUIRED )
# hack for homebrew installation and potentially some other platforms
if( EXISTS "${Boost_INCLUDE_DIR}/root" )
   set( ROOT_EXTRA_INCLUDE_DIR "${Boost_INCLUDE_DIR}/root" )
   string( REPLACE "/include/" "/lib/" ROOT_EXTRA_LIB_DIR "${ROOT_EXTRA_INCLUDE_DIR}" )
   list( APPEND CMAKE_INCLUDE_PATH "${ROOT_EXTRA_INCLUDE_DIR}" )
   list( APPEND CMAKE_LIBRARY_PATH "${ROOT_EXTRA_LIB_DIR}" )
endif()
# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)
find_package( Python3 COMPONENTS Development REQUIRED )
find_package( LCG QUIET )
find_package( onnxruntime QUIET )

include_directories(${CMAKE_SOURCE_DIR})
# ------------------------------------------------
# The actual FastFrames library
# ------------------------------------------------

file(GLOB SOURCES "Root/*.cc" )
file(GLOB LIB_HEADERS "FastFrames/*.h" )

# ------------------------------------------------
# ONNXWrapper conditional compilation
# ------------------------------------------------

# Define a variable for the ONNX module path. Allow it to be set from the command line.
set(ONNXRUNTIME_ROOTDIR "" CACHE PATH "Path to the ONNX Runtime installation directory")

# Check if onnxruntime was found
if(onnxruntime_FOUND OR NOT ONNXRUNTIME_ROOTDIR STREQUAL "")
   if (onnxruntime_FOUND)
      message(STATUS "onnxruntime found at: ${onnxruntime_DIR}")
   endif()
   message(STATUS "ONNX Runtime found - Enabling ONNXWrapper")
   if (NOT ${onnxruntime_FOUND})
      message(STATUS "ONNXRUNTIME_ROOTDIR: ${ONNXRUNTIME_ROOTDIR}")
      set(ONNXRUNTIME_INCLUDE_DIRS ${ONNXRUNTIME_ROOTDIR}/include)
      set(ONNXRUNTIME_LIBRARIES ${ONNXRUNTIME_ROOTDIR}/lib/libonnxruntime.so)
   endif()

   # This is necessary for Mac installations because
   # the onnxruntime executable can be found without the paths being set
   if ((NOT DEFINED ONNXRUNTIME_INCLUDE_DIRS) AND (NOT DEFINED ONNXRUNTIME_LIBRARIES))
      message(STATUS "ONNXRUNTIME_ROOTDIR not set, need to search for ONNXRUNTIME_INCLUDE_DIRS and ONNXRUNTIME_LIBRARIES")
      find_path(ONNXRUNTIME_INCLUDE_DIRS
         NAMES onnxruntime_c_api.h
         HINTS ${onnxruntime_DIR}/../include
               /usr/local/include
               /opt/homebrew/include/onnxruntime
               /usr/include
      )
      message(STATUS "Found ONNXRUNTIME_INCLUDE_DIRS: ${ONNXRUNTIME_INCLUDE_DIRS}")

      find_library(ONNXRUNTIME_LIBRARIES
         NAMES onnxruntime libonnxruntime
         HINTS ${onnxruntime_DIR}/../lib
               /opt/homebrew/include/onnxruntime
               /opt/homebrew/lib
               /opt/homebrew/Cellar/
      )
      message(STATUS "Found ONNXRUNTIME_LIBRARIES: ${ONNXRUNTIME_LIBRARIES}")
   endif()

   message(STATUS "ONNXRUNTIME_INCLUDE_DIRS: ${ONNXRUNTIME_INCLUDE_DIRS}")
   message(STATUS "ONNXRUNTIME_LIBRARIES: ${ONNXRUNTIME_LIBRARIES}")
   add_definitions(-DONNXRUNTIME_AVAILABLE)
else()
   message(STATUS "ONNX Runtime not found - Disabling ONNXWrapper")

   # Remove ONNXWrapper source files from the lists
   #list(REMOVE_ITEM LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/FastFrames/ONNXWrapper.h )
   #list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Root/ONNXWrapper.cc )
endif()

# Build the shared library.
add_library( FastFrames SHARED ${LIB_HEADERS} ${SOURCES} )
set_target_properties(FastFrames PROPERTIES SUFFIX ".so")
target_include_directories( FastFrames PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> ${ONNXRUNTIME_INCLUDE_DIRS} )
target_include_directories(FastFrames SYSTEM PUBLIC ${ONNXRUNTIME_INCLUDE_DIRS} )
target_link_libraries( FastFrames PUBLIC ROOT::Core ROOT::Hist ROOT::RIO ROOT::Tree ROOT::ROOTDataFrame ROOT::ROOTVecOps ${ONNXRUNTIME_LIBRARIES} )
set_property( TARGET FastFrames
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )

# Build the Python module for C++ logger
add_library( cppLogger SHARED python_wrapper/utils/Logger.cxx )
set_target_properties(cppLogger PROPERTIES SUFFIX ".so")
target_link_libraries( cppLogger PRIVATE Python3::Python )
set_target_properties( cppLogger PROPERTIES
   PREFIX ""
   OUTPUT_NAME "cppLogger" )

# Build the Python module for config reader
add_library( ConfigReaderCpp SHARED python_wrapper/utils/ConfigReaderCpp.cxx ${LIB_HEADERS} )
set_target_properties(ConfigReaderCpp PROPERTIES SUFFIX ".so")
target_link_libraries( ConfigReaderCpp PRIVATE Python3::Python ${Boost_LIBRARIES} FastFrames )
set_target_properties( ConfigReaderCpp PROPERTIES
   PREFIX ""
   OUTPUT_NAME "ConfigReaderCpp" )


set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
file(APPEND ${SETUP} "export LD_LIBRARY_PATH=\${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib\n")
file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_CURRENT_SOURCE_DIR}/python\n")

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
  target_link_libraries( ${name} FastFrames)
  install( TARGETS ${name}
    EXPORT FastFramesTargets
    RUNTIME DESTINATION bin )
endmacro( FastFrames_add_executable )

FastFrames_add_executable( fast-frames.exe util/fast-frames.cc )

ROOT_GENERATE_DICTIONARY(FastFrames_dict FastFrames/MainFrame.h MODULE FastFrames LINKDEF Root/LinkDef.h)
# needed as ROOT_GENERATE_DICTIONARY does not support system includes
target_compile_options(FastFrames_dict PUBLIC -Wno-shadow)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}_rdict.pcm ${CMAKE_CURRENT_BINARY_DIR}/lib/lib${PROJECT_NAME}.rootmap DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
