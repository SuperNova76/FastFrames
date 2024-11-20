# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  PANDAS_PYTHON_PATH
#
# Can be steered by PANDAS_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pandas
   PYTHON_NAMES pandas/__init__.py pandas.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pandas DEFAULT_MSG
   _PANDAS_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( pandas )
