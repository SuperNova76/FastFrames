# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# - Try to find CORAL
# Defines:
#
#  CORAL_FOUND
#  CORAL_INCLUDE_DIR
#  CORAL_INCLUDE_DIRS
#  CORAL_<component>_LIBRARY
#  CORAL_<component>_FOUND
#  CORAL_LIBRARIES
#  CORAL_LIBRARY_DIRS
#  CORAL_PYTHON_PATH
#  CORAL_BINARY_PATH
#  CORAL_coralServer_EXECUTABLE
#  CORAL_coralEchoServer_EXECUTABLE
#
# Can be steered by CORAL_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module.
lcg_external_module( NAME CORAL
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES RelationalAccess/ConnectionService.h
   LIBRARY_SUFFIXES lib LIBRARY_PREFIX lcg_
   COMPULSORY_COMPONENTS CoralBase )

# Find the python code and the executables.
lcg_python_external_module( NAME CORAL
   PYTHON_NAMES coral.py
   PYTHON_SUFFIXES python
   BINARY_NAMES coralServer coralEchoServer
   BINARY_SUFFIXES bin )

# Massage the python path.
list( APPEND CORAL_PYTHON_PATH ${CORAL_LIBRARY_DIRS} )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CORAL DEFAULT_MSG CORAL_INCLUDE_DIR
   CORAL_LIBRARIES _CORAL_PYTHON_PATH _CORAL_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( CORAL )
