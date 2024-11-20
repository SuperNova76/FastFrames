# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  MATPLOTLIB_PYTHON_PATH
#
# Can be steered by MATPLOTLIB_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME matplotlib
   PYTHON_NAMES matplotlib/__init__.py matplotlib.py )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( matplotlib DEFAULT_MSG
   _MATPLOTLIB_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( matplotlib )
