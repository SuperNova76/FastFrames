# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  VCVERSIONER_PYTHON_PATH
#
# Can be steered by VCVERSIONER_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME vcversioner
   PYTHON_NAMES vcversioner/__init__.py vcversioner.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( vcversioner DEFAULT_MSG
   _VCVERSIONER_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( vcversioner )
