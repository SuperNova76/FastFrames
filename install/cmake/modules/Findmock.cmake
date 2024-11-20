# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MOCK_PYTHON_PATH
#
# Can be steered by MOCK_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME mock
   PYTHON_NAMES mock/__init__.py mock.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( mock DEFAULT_MSG
   _MOCK_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( mock )
