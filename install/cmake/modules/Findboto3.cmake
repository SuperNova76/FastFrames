# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  BOTO3_PYTHON_PATH
#
# Can be steered by BOTO3_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME boto3
   PYTHON_NAMES boto3/__init__.py boto3.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( boto3 DEFAULT_MSG
   _BOTO3_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( boto3 )
