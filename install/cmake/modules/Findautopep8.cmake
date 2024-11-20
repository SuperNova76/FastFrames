# Copyright (C) 2002-2023 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  AUTOPEP8_PYTHON_PATH
#  AUTOPEP8_BINARY_PATH
#  AUTOPEP8_autopep8_EXECUTABLE
#
# Can be steered by AUTOPEP8_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME autopep8
   PYTHON_NAMES autopep8.py autopep8/__init__.py
   BINARY_NAMES autopep8
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( autopep8 DEFAULT_MSG
   _AUTOPEP8_BINARY_PATH _AUTOPEP8_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( autopep8 )
