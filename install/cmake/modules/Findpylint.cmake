# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYLINT_PYTHON_PATH
#  PYLINT_BINARY_PATH
#  PYLINT_pylint_EXECUTABLE
#
# Can be steered by PYLINT_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pylint
   PYTHON_NAMES pylint/__init__.py pylint.py
   BINARY_NAMES pylint
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pylint DEFAULT_MSG
   _PYLINT_PYTHON_PATH _PYLINT_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pylint )
