# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PYPARSING_PYTHON_PATH
#
# Can be steered by PYPARSING_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pyparsing
   PYTHON_NAMES pyparsing/__init__.py pyparsing.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pyparsing DEFAULT_MSG
   _PYPARSING_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pyparsing )
