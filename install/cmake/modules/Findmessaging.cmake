# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MESSAGING_PYTHON_PATH
#
# Can be steered by MESSAGING_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME messaging
   PYTHON_NAMES messaging/__init__.py messaging.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( messaging DEFAULT_MSG
   _MESSAGING_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( messaging )
