# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  S3TRANSFER_PYTHON_PATH
#
# Can be steered by S3TRANSFER_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME s3transfer
   PYTHON_NAMES s3transfer/__init__.py s3transfer.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( s3transfer DEFAULT_MSG
   _S3TRANSFER_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( s3transfer )
