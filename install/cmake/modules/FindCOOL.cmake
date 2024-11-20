# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  COOL_FOUND
#  COOL_INCLUDE_DIR
#  COOL_INCLUDE_DIRS
#  COOL_<component>_LIBRARY
#  COOL_<component>_FOUND
#  COOL_LIBRARIES
#  COOL_LIBRARY_DIRS
#  COOL_PYTHON_PATH
#  COOL_BINARY_PATH
#  COOL_coolReplicateDB_EXECUTABLE
#  COOL_coolAuthentication_EXECUTABLE
#
# The script can be guided using COOL_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Declare the external module.
lcg_external_module( NAME COOL
   INCLUDE_SUFFIXES include
   INCLUDE_NAMES CoolKernel/IDatabase.h
   LIBRARY_SUFFIXES lib LIBRARY_PREFIX lcg_
   COMPULSORY_COMPONENTS CoolKernel CoolApplication )

# Find the python code and the executables.
lcg_python_external_module( NAME COOL
   PYTHON_NAMES PyCool/__init__.py PyCoolTool.py
   PYTHON_SUFFIXES python
   BINARY_NAMES coolReplicateDB coolAuthentication
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( COOL DEFAULT_MSG COOL_INCLUDE_DIR
   COOL_LIBRARIES _COOL_PYTHON_PATH _COOL_BINARY_PATH )

# Additional environment variable(s):
set( COOL_ENVIRONMENT
   SET COOL_DISABLE_CORALCONNECTIONPOOLCLEANUP "YES" )

# Set up the RPM dependency:
lcg_need_rpm( COOL )
