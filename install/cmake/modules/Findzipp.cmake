# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  ZIPP_PYTHON_PATH
#
# Can be steered by ZIPP_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME zipp
   PYTHON_NAMES zipp/__init__.py zipp.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( zipp DEFAULT_MSG
   _ZIPP_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( zipp )
