# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#   DECORATOR_PYTHON_PATH
#
# Can be steered by DECORATOR_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME decorator
   PYTHON_NAMES decorator.py decorator/__init__.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( decorator DEFAULT_MSG
   _DECORATOR_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( decorator )
