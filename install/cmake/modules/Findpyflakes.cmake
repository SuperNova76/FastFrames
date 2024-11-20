# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYFLAKES_PYTHON_PATH
#  PYFLAKES_BINARY_PATH
#  PYFLAKES_pyflakes_EXECUTABLE
#
# Can be steered by PYFLAKES_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pyflakes
   MODULE_NAME pyflakes
   PYTHON_NAMES pyflakes/__init__.py
   BINARY_NAMES pyflakes
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pyflakes
   VERSION_VAR PYFLAKES_VERSION
   HANDLE_VERSION_RANGE
   REQUIRED_VARS _PYFLAKES_BINARY_PATH _PYFLAKES_PYTHON_PATH PYFLAKES_VERSION )

# Set up the RPM dependency.
lcg_need_rpm( pyflakes )
