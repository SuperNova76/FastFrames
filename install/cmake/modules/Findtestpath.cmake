# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  TESTPATH_PYTHON_PATH
#
# Can be steered by TESTPATH_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME testpath
   PYTHON_NAMES testpath/__init__.py testpath.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( testpath DEFAULT_MSG
   _TESTPATH_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( testpath )
