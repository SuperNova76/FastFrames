# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYCODESTYLE_PYTHON_PATH
#  PYCODESTYLE_BINARY_PATH
#  PYCODESTYLE_pycodestyle_EXECUTABLE
#
# Can be steered by PYCODESTYLE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pycodestyle
   MODULE_NAME pycodestyle
   PYTHON_NAMES pycodestyle.py pycodestyle/__init__.py
   BINARY_NAMES pycodestyle
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pycodestyle
   VERSION_VAR PYCODESTYLE_VERSION
   HANDLE_VERSION_RANGE
   REQUIRED_VARS _PYCODESTYLE_BINARY_PATH _PYCODESTYLE_PYTHON_PATH
   PYCODESTYLE_VERSION )

# Set up the RPM dependency.
lcg_need_rpm( pycodestyle )
