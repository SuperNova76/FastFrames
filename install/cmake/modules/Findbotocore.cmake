# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  BOTOCORE_PYTHON_PATH
#
# Can be steered by BOTOCORE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME botocore
   PYTHON_NAMES botocore/__init__.py botocore.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( botocore DEFAULT_MSG
   _BOTOCORE_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( botocore )
