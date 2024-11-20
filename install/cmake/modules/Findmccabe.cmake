# Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MCCABE_PYTHON_PATH
#
# Can be steered by MCCABE_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME mccabe
   MODULE_NAME mccabe
   PYTHON_NAMES mccabe.py mccabe/__init__.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( mccabe
   VERSION_VAR MCCABE_VERSION
   HANDLE_VERSION_RANGE
   REQUIRED_VARS _MCCABE_PYTHON_PATH MCCABE_VERSION )

# Set up the RPM dependency.
lcg_need_rpm( mccabe )
