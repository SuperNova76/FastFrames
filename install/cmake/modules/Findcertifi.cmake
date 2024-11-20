# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  CERTIFI_PYTHON_PATH
#
# Can be steered by CERTIFI_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME certifi
   PYTHON_NAMES certifi/__init__.py certifi.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( certifi DEFAULT_MSG
   _CERTIFI_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( certifi )
