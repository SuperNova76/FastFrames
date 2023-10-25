# Set the minimum CMake version required to build the project.
cmake_minimum_required( VERSION 3.1 )

# Silence some warnings on macOS with new CMake versions.
if( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
   cmake_policy( SET CMP0068 NEW )
endif()

# Set the project's name and version.
project( FastFrames )

# Set up the "C++ version" to use.
set( CMAKE_CXX_STANDARD_REQUIRED 17 CACHE STRING
   "Minimum C++ standard required for the build" )
set( CMAKE_CXX_STANDARD 20 CACHE STRING
   "C++ standard to use for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL
   "(Dis)allow using compiler extensions" )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the install locations for libraries and binaries.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_INSTALL_LIBDIR ${CMAKE_BINARY_DIR}/lib )  # Needed by ROOT_GENERATE_DICTIONARY()

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ------------------------------------------------
# Dependencies and sub-libraries
# ------------------------------------------------

# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)
find_package( Python3 COMPONENTS Development REQUIRED )

# ------------------------------------------------
# The actual FastFrames library
# ------------------------------------------------

# Public header files for the shared/static library.
set( lib_headers
  FastFrames/Binning.h
  FastFrames/ConfigSetting.h
  FastFrames/Logger.h
  FastFrames/MainFrame.h
  FastFrames/Metadata.h
  FastFrames/MetadataManager.h
  FastFrames/Region.h
  FastFrames/Sample.h
  FastFrames/Systematic.h
  FastFrames/UniqueSampleID.h
  FastFrames/Variable.h )

# Source files for the shared/static library.
set( lib_sources
  Root/Binning.cc
  Root/ConfigSetting.cc
  Root/MainFrame.cc
  Root/Metadata.cc
  Root/MetadataManager.cc
  Root/Region.cc
  Root/Sample.cc
  Root/Systematic.cc
  Root/UniqueSampleID.cc
  Root/Variable.cc )

# Build the shared library.
add_library( FastFrames SHARED ${lib_headers} ${lib_sources} )
target_include_directories( FastFrames PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> $<INSTALL_INTERFACE:> ${ROOT_INCLUDE_DIRS} )
target_link_libraries( FastFrames ${ROOT_LIBRARIES} )
set_property( TARGET FastFrames
   PROPERTY PUBLIC_HEADER ${lib_headers} )
target_include_directories(FastFrames PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# Install the libraries.
install( TARGETS FastFrames
   EXPORT FastFramesTargets
   ARCHIVE DESTINATION lib
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include/FastFrames )


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

# Build the Python module for C++ logger
add_library( cppLogger SHARED python_wrapper/utils/Logger.cxx )
target_link_libraries( cppLogger PRIVATE Python3::Python ${Python3_LIBRARIES} )
set_target_properties( cppLogger PROPERTIES
   PREFIX ""
   OUTPUT_NAME "cppLogger" )
