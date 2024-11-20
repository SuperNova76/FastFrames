# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  BACKCALL_PYTHON_PATH
#
# Can be steered by BACKCALL_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME backcall
   PYTHON_NAMES backcall/__init__.py backcall.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( backcall DEFAULT_MSG
   _BACKCALL_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( backcall )
