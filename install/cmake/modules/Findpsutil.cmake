# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PSUTIL_PYTHON_PATH
#
# Can be steered by PSUTIL_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME psutil
   PYTHON_NAMES psutil/__init__.py psutil.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( psutil DEFAULT_MSG
  _PSUTIL_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( psutil )
