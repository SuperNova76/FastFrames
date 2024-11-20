# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  REQUESTS_PYTHON_PATH
#
# Can be steered by REQUESTS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME requests
   PYTHON_NAMES requests/__init__.py requests.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( requests DEFAULT_MSG
   _REQUESTS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( requests )
