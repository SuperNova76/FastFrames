# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  URLLIB3_PYTHON_PATH
#
# Can be steered by URLLIB3_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME urllib3
   PYTHON_NAMES urllib3/__init__.py urllib3.py )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( urllib3 DEFAULT_MSG
   _URLLIB3_PYTHON_PATH )

# Set up the RPM dependency:
lcg_need_rpm( urllib3 )
