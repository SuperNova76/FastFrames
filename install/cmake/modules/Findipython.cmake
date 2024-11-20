# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Sets:
#  IPYTHON_PYTHON_PATH
#  IPYTHON_BINARY_PATH
#  IPYTHON_ipython_EXECUTABLE
#  IPYTHON_ipython2_EXECUTABLE
#
# Can be steered by IPYTHON_LCGROOT.
#

# The LCG include(s).
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME ipython
   PYTHON_NAMES IPython/__init__.py
   BINARY_NAMES ipython ipython2
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( ipython DEFAULT_MSG
   _IPYTHON_BINARY_PATH _IPYTHON_PYTHON_PATH )

# Set up the RPM dependency.
lcg_need_rpm( ipython )
