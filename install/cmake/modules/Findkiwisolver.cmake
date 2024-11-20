# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#   KIWISOLVER_PYTHON_PATH
#
# Can be steered by KIWISOLVER_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME kiwisolver
   PYTHON_NAMES "kiwisolver${CMAKE_SHARED_LIBRARY_SUFFIX}"
   "kiwisolver.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu${CMAKE_SHARED_LIBRARY_SUFFIX}"
   "kiwisolver.cpython-${Python_VERSION_MAJOR}${Python_VERSION_MINOR}m-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu${CMAKE_SHARED_LIBRARY_SUFFIX}" )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( kiwisolver DEFAULT_MSG
   _KIWISOLVER_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( kiwisolver )
