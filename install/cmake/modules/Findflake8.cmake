# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  FLAKE8_PYTHON_PATH
#  FLAKE8_BINARY_PATH
#  FLAKE8_flake8_EXECUTABLE
#
# Can be steered by FLAKE8_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME flake8
   PYTHON_NAMES flake8.py flake8/__init__.py
   BINARY_NAMES flake8
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( flake8 DEFAULT_MSG
   _FLAKE8_BINARY_PATH _FLAKE8_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( flake8 )
