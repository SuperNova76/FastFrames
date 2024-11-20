# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  IMPORTLIB_METADATA_PYTHON_PATH
#
# Can be steered by IMPORTLIB_METADATA_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME importlib_metadata
   PYTHON_NAMES importlib_metadata/__init__.py importlib_metadata.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( importlib_metadata DEFAULT_MSG
   _IMPORTLIB_METADATA_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( importlib_metadata )
