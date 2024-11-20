# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#   CX_ORACLE_PYTHON_PATH
#
# Can be steered by CX_ORACLE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# We need to know the version of Python being used.
find_package( Python COMPONENTS Interpreter )

# Find it.
lcg_python_external_module( NAME cx_Oracle
   PYTHON_NAMES cx_Oracle${CMAKE_SHARED_LIBRARY_SUFFIX}
   cx_Oracle.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu${CMAKE_SHARED_LIBRARY_SUFFIX}
   cx_Oracle.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}m-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu${CMAKE_SHARED_LIBRARY_SUFFIX} )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cx_Oracle DEFAULT_MSG
   _CX_ORACLE_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( cx_oracle )
