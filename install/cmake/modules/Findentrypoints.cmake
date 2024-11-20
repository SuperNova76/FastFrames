# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  ENTRYPOINTS_PYTHON_PATH
#
# Can be steered by ENTRYPOINTS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME entrypoints
   PYTHON_NAMES entrypoints/__init__.py entrypoints.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( entrypoints DEFAULT_MSG
   _ENTRYPOINTS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( entrypoints )
