# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PLUGGY_PYTHON_PATH
#
# Can be steered by PLUGGY_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pluggy
   PYTHON_NAMES pluggy/__init__.py pluggy.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pluggy DEFAULT_MSG
   _PLUGGY_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pluggy )
