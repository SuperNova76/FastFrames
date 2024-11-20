# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYTEST_PYTHON_PATH
#  PYTEST_BINARY_PATH
#  PYTEST_pytest_EXECUTABLE
#
# Can be steered by PYTEST_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pytest
   PYTHON_NAMES pytest/__init__.py _pytest/__init__.py pytest.py
   BINARY_NAMES pytest py.test
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pytest DEFAULT_MSG
   _PYTEST_PYTHON_PATH _PYTEST_BINARY_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pytest )
