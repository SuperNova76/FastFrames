# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#  
# Sets:
#  SCIPY_PYTHON_PATH
#
# Can be steered by SCIPY_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME scipy
   PYTHON_NAMES scipy/__init__.py scipy.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( scipy DEFAULT_MSG
   _SCIPY_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( scipy )


