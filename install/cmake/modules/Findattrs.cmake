# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  ATTRS_PYTHON_PATH
#
# Can be steered by ATTRS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME attrs
   PYTHON_NAMES attr/__init__.py attr.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( attrs DEFAULT_MSG
   _ATTRS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( attrs )
