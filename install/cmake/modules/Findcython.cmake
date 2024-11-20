# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  CYTHON_PYTHON_PATH
#  CYTHON_BINARY_PATH
#  CYTHON_cython_EXECUTABLE
#
# Can be steered by CYTHON_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME cython
   PYTHON_NAMES cython.py Cython/__init__.py
   BINARY_NAMES cython
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cython DEFAULT_MSG
   _CYTHON_PYTHON_PATH _CYTHON_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( cython )
