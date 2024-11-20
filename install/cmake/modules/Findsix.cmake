# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  SIX_PYTHON_PATH
#
# Can be steered by SIX_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME six
   PYTHON_NAMES six/__init__.py six.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( six DEFAULT_MSG
   _SIX_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( six )
