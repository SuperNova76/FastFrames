# Set the minimum CMake version required to build the project.
cmake_minimum_required( VERSION 3.1 )

# Silence some warnings on macOS with new CMake versions.
if( NOT ${CMAKE_VERSION} VERSION_LESS 3.9 )
   cmake_policy( SET CMP0068 NEW )
endif()

set(CMAKE_INSTALL_PREFIX "" CACHE PATH "Path to the install directory")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Set the project's name and version.
project( CustomFrame )

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

# Set the warning flag(s) to use.
set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -pedantic -O3 -g" )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Silence boost warnings
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)
add_definitions(-DBOOST_ALLOW_DEPRECATED_HEADERS)

# Add ROOT system directory and require ROOT.
find_package( ROOT 6.28.00 REQUIRED COMPONENTS Core Hist RIO Tree ROOTDataFrame ROOTVecOps)

find_package( FastFrames REQUIRED)

# ------------------------------------------------
# The actual FastFrames library
# ------------------------------------------------

file(GLOB SOURCES "Root/*.cc")
file(GLOB LIB_HEADERS_MINE "CustomFrame/*.h")

# Build the shared library.
add_library( CustomFrame SHARED ${LIB_HEADERS} ${SOURCES} )
target_include_directories( CustomFrame PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}> $<INSTALL_INTERFACE:> ${FastFrames_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} )
target_link_libraries( CustomFrame ${FastFrames_LIBRARIES} ${ROOT_LIBRARIES} )
set_property( TARGET CustomFrame
   PROPERTY PUBLIC_HEADER ${LIB_HEADERS} )
target_include_directories(CustomFrame PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# Install the libraries.
install( TARGETS CustomFrame
   EXPORT CustomFrameTargets
   ARCHIVE DESTINATION lib
   LIBRARY DESTINATION lib
   PUBLIC_HEADER DESTINATION include/CustomFrame )

ROOT_GENERATE_DICTIONARY(CustomFrame_dict CustomFrame/CustomFrame.h MODULE CustomFrame LINKDEF Root/LinkDef.h)
