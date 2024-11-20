# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MULTIPROCESS_PYTHON_PATH
#
# Can be steered by MULTIPROCESS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME multiprocess
   PYTHON_NAMES multiprocess/__init__.py multiprocess.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( multiprocess DEFAULT_MSG
   _MULTIPROCESS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( multiprocess )
