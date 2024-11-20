# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  FUTURE_BINARY_PATH
#  FUTURE_PYTHON_PATH
#  FUTURE_futurize_EXECUTABLE
#
# Can be steered by FUTURE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME future
   PYTHON_NAMES future/__init__.py future.py
   BINARY_NAMES futurize
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( future DEFAULT_MSG
   _FUTURE_PYTHON_PATH _FUTURE_BINARY_PATH )

# Set up the RPM dependency:
lcg_need_rpm( future )
