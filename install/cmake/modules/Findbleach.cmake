# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  BLEACH_PYTHON_PATH
#
# Can be steered by BLEACH_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME bleach
   PYTHON_NAMES bleach/__init__.py bleach.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( bleach DEFAULT_MSG
   _BLEACH_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( bleach )
