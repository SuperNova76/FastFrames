# Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
#
# Module setting up pip from the LCG release for the runtime environment.
#
# Sets:
#   - PIP_BINARY_PATH
#   - PIP_PYTHON_PATH
#   - PIP_pip_EXECUTABLE
#
# Can be steered by PIP_LCGROOT.
#

# The LCG include(s):
include( LCGFunctions )

# Find it.
lcg_python_external_module( NAME pip
   PYTHON_NAMES site.py pip/__init__.py pip.py
   BINARY_NAMES pip
   BINARY_SUFFIXES bin )

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( pip DEFAULT_MSG
   _PIP_BINARY_PATH _PIP_PYTHON_PATH )

# Set up the RPM dependency:
lcg_need_rpm( pip )
